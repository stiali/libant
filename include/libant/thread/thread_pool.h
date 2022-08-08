/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2022 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef LIBANT_INCLUDE_LIBANT_THREAD_THREAD_POOL_H_
#define LIBANT_INCLUDE_LIBANT_THREAD_THREAD_POOL_H_

#include <cassert>
#include <atomic>
#include <functional>
#include <list>
#include <unordered_set>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <libant/system/signal.h>

namespace ant {

/**
 * A thread pool to run Job
 *
 * @tparam Job the lifetime of a Job object is within the same thread. The Job class must implement 2 methods:
 *   - 1. 'JobOutput Process(JobInput task)' to process the task and return the result
 *   - 2. 'void SetConfig(JobConfig i) or void SetConfig(const JobConfig& i) or void SetConfig(JobConfig&& i)' to reset configurations
 * @tparam JobConfig
 * @tparam JobInput
 * @tparam JobOutput
 */
template<typename Job, typename JobConfig, typename JobInput, typename JobOutput>
class ThreadPool {
public:
    /**
     * Construct a ThreadPool to run Jobs
     * @param minThreadNum
     * @param maxThreadNum
     * @param cfg
     */
    ThreadPool(int minThreadNum, int maxThreadNum, const JobConfig* cfg = nullptr)
        : maxThreadNum_(maxThreadNum)
        , freeWorkerNum_(0)
        , stop_(false)
    {
        assert(minThreadNum <= maxThreadNum);
        if (cfg) {
            jobCfg_ = *cfg;
        }

        for (int i = 0; i != minThreadNum; ++i) {
            createWorker(true);
        }
    }

    ~ThreadPool()
    {
        Stop();
    }

    /**
     * Use ThreadPool to run a task.
     * @param task
     */
    void Run(JobInput task)
    {
        dataQueueMtx_.lock();
        if (!stop_) {
            if (freeWorkerNum_ <= dataQueue_.size() && allWorkers_.size() < maxThreadNum_) {
                createWorker(false);
            }
            dataQueue_.emplace_back(task);
            dataQueueCond_.notify_one();
        }
        dataQueueMtx_.unlock();
    }

    /**
     * Get result of a finished task.
     * @param waitMs waiting time in milliseconds. 0 means don't wait, > 0 means wait for the specified amount of time, < 0 means wait until a result is available.
     * @return true on success, false on failure
     */
    std::pair<JobOutput, bool> GetJobOutput(int waitMs = 0)
    {
        std::unique_lock<std::mutex> lck(dataQueueMtx_);
        if (!stop_) {
            if (waitMs < 0) {
                while (outQueue_.empty()) {
                    outQueueCond_.wait(lck);
                    if (stop_) {
                        break;
                    }
                }
            } else if (waitMs > 0) {
                if (outQueue_.empty()) {
                    outQueueCond_.wait_for(lck, std::chrono::milliseconds(waitMs));
                }
            }
        }

        if (!outQueue_.empty()) {
            auto output = outQueue_.front();
            outQueue_.pop_front();
            return std::make_pair(output, true);
        }
        return std::make_pair(JobOutput(), false);
    }

    /**
     * Get number of unfinished tasks.
     * @return number of unfinished tasks.
     */
    size_t GetPendingTaskCount()
    {
        dataQueueMtx_.lock();
        size_t n = dataQueue_.size() + allWorkers_.size() - freeWorkerNum_;
        dataQueueMtx_.unlock();
        return n;
    }

    /**
     * Stop the ThreadPool. It blocks until all the tasks are finished.
     */
    void Stop()
    {
        dataQueueMtx_.lock();
        if (!stop_) {
            stop_ = true;
            dataQueueMtx_.unlock();
            dataQueueCond_.notify_all();
            outQueueCond_.notify_all();

            for (auto worker : allWorkers_) {
                delete worker;
            }
            allWorkers_.clear();
            freeWorkerNum_ = 0;
        } else {
            dataQueueMtx_.unlock();
        }
    }

    /**
     * Reset JobConfig
     * @param cfg
     */
    void ResetJobConfig(const JobConfig& cfg)
    {
        setJobConfig(cfg);

        dataQueueMtx_.lock();
        if (!stop_) {
            for (auto it = allWorkers_.begin(); it != allWorkers_.end(); ++it) {
                (*it)->ResetJobConfig();
            }
        }
        dataQueueMtx_.unlock();
    }

private:
    class Worker;

private:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void createWorker(bool keepInPool)
    {
        auto worker = new Worker(*this, keepInPool);
        allWorkers_.emplace(worker);
        worker->Start();
    }

    void setJobConfig(const JobConfig& cfg)
    {
        std::lock_guard<std::mutex> lock(jobCfgLock_);
        jobCfg_ = cfg;
    }

    JobConfig getJobConfig() const
    {
        std::lock_guard<std::mutex> lock(jobCfgLock_);
        return jobCfg_;
    }

private:
    mutable std::mutex jobCfgLock_;
    JobConfig jobCfg_;

    const size_t maxThreadNum_;

    std::mutex dataQueueMtx_;
    std::condition_variable dataQueueCond_;
    std::list<JobInput> dataQueue_;
    std::condition_variable outQueueCond_;
    std::list<JobOutput> outQueue_;
    std::unordered_set<Worker*> allWorkers_;
    volatile size_t freeWorkerNum_;
    volatile bool stop_;
};

template<typename Job, typename JobConfig, typename JobInput, typename JobOutput>
class ThreadPool<Job, JobConfig, JobInput, JobOutput>::Worker {
public:
    Worker(ThreadPool<Job, JobConfig, JobInput, JobOutput>& thrpool, bool keepInPool = true)
        : pool_(thrpool)
        , reload_(false)
        , keepInPool_(keepInPool)
        , thr_(nullptr)
    {
    }

    ~Worker()
    {
        if (thr_) {
            thr_->join();
            delete thr_;
        }
    }

    void Start()
    {
        thr_ = new std::thread(std::bind(&Worker::run, this));
    }

    void ResetJobConfig()
    {
        reload_ = true;
    }

private:
    void run()
    {
        ThreadBlockAllSignals();
        job_.SetConfig(pool_.getJobConfig());

        std::unique_lock<std::mutex> lck(pool_.dataQueueMtx_);
        for (;;) {
            while (pool_.dataQueue_.empty()) {
                if (pool_.stop_) {
                    lck.unlock();
                    return;
                }

                ++(pool_.freeWorkerNum_);
                if (keepInPool_) {
                    pool_.dataQueueCond_.wait(lck);
                } else {
                    auto cv = pool_.dataQueueCond_.wait_for(lck, std::chrono::seconds(5));
                    if (cv == std::cv_status::timeout) {
                        if (!pool_.stop_) {
                            pool_.allWorkers_.erase(this);
                            --(pool_.freeWorkerNum_);
                            lck.unlock();
                            thr_->detach();
                            delete thr_;
                            thr_ = nullptr;
                            delete this;
                        } else {
                            lck.unlock();
                        }
                        return;
                    }
                }
                --(pool_.freeWorkerNum_);
            }

            auto task = pool_.dataQueue_.front();
            pool_.dataQueue_.pop_front();
            lck.unlock();

            if (needResetJobConfig()) {
                job_.SetConfig(pool_.getJobConfig());
            }

            auto result = job_.Process(task);
            lck.lock();
            pool_.outQueue_.emplace_back(result);
            pool_.outQueueCond_.notify_one();
        }
    }

    bool needResetJobConfig()
    {
        return reload_.exchange(false);
    }

private:
    ThreadPool<Job, JobConfig, JobInput, JobOutput>& pool_;
    std::atomic<bool> reload_;
    bool keepInPool_;
    Job job_;
    std::thread* thr_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_THREAD_THREAD_POOL_H_
