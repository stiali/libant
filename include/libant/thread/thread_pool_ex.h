#ifndef LIBANT_INCLUDE_LIBANT_THREAD_THREAD_POOL_EX_H_
#define LIBANT_INCLUDE_LIBANT_THREAD_THREAD_POOL_EX_H_

#include <cassert>
#include <functional>
#include <list>
#include <unordered_set>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace ant {

/**
 * ThreadPoolEx can run any kind of task, including function, functor, lambda, etc.
 * The only limitation is that the task must return std::shared_ptr<AbsOutput> as it's result.
 */
class ThreadPoolEx {
public:
    /**
     * To make use of ThreadPoolEx, a task must return std::shared_ptr<AbsOutput>
     */
    class AbsOutput {
    public:
        virtual ~AbsOutput()
        {
        }
    };

private:
    class Worker;

    class AbsExecutor {
    public:
        virtual ~AbsExecutor()
        {
        }

        virtual void Exec(ThreadPoolEx&) = 0;
    };

    template<typename Function>
    class Executor : public AbsExecutor {
    public:
        Executor(Function&& func)
            : func_(func)
        {
        }

        virtual void Exec(ThreadPoolEx& pool);

    private:
        Function func_;
    };

public:
    /**
     * Construct a ThreadPoolEx object.
     * @param minThreadNum
     * @param maxThreadNum
     */
    ThreadPoolEx(int minThreadNum, int maxThreadNum)
        : maxThreadNum_(maxThreadNum)
        , freeWorkerNum_(0)
        , stop_(false)
    {
        assert(minThreadNum <= maxThreadNum);
        for (int i = 0; i != minThreadNum; ++i) {
            createWorker(true);
        }
    }

    ~ThreadPoolEx()
    {
        Stop();
    }

    /**
     * Use ThreadPool to run a task.
     * @tparam Function
     * @tparam Args
     * @param task
     * @param args
     */
    template<typename Function, typename... Args>
    void Run(Function&& task, Args&&... args)
    {
        auto func = std::bind(task, args...);
        auto executor = new Executor<decltype(func)>(move(func));

        taskQueueLock_.lock();
        if (!stop_) {
            if (freeWorkerNum_ <= taskQueue_.size() && allWorkers_.size() < maxThreadNum_) {
                createWorker(false);
            }
            taskQueue_.emplace_back(executor);
            executor = nullptr;
            taskQueueCond_.notify_one();
        }
        taskQueueLock_.unlock();

        delete executor;
    }

    /**
     * Get result of a finished task.
     * @param waitMs waiting time in milliseconds. 0 means don't wait, > 0 means wait for the specified amount of time, < 0 means wait until a result is available.
     * @return shared pointer to the result on success, nullptr on failure.
     */
    std::shared_ptr<AbsOutput> GetJobOutput(int waitMs = 0);

    /**
     * Stop ThreadPoolEx. It blocks until all the tasks are finished.
     */
    void Stop();

private:
    ThreadPoolEx(const ThreadPoolEx&) = delete;
    ThreadPoolEx& operator=(const ThreadPoolEx&) = delete;

    void createWorker(bool keepInPool);

    void pushResult(std::shared_ptr<AbsOutput>&& result)
    {
        taskQueueLock_.lock();
        outQueue_.emplace_back(result);
        taskQueueLock_.unlock();
        outQueueCond_.notify_one();
    }

private:
    const size_t maxThreadNum_;

    std::mutex taskQueueLock_;
    std::condition_variable taskQueueCond_;
    std::list<AbsExecutor*> taskQueue_;
    std::condition_variable outQueueCond_;
    std::list<std::shared_ptr<AbsOutput>> outQueue_;
    std::unordered_set<Worker*> allWorkers_;
    volatile size_t freeWorkerNum_;
    volatile bool stop_;
};

template<typename Function>
void ThreadPoolEx::Executor<Function>::Exec(ThreadPoolEx& pool)
{
    pool.pushResult(func_());
}

class ThreadPoolEx::Worker {
public:
    Worker(ThreadPoolEx& thrpool, bool keepInPool = true)
        : pool_(thrpool)
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

private:
    void run();

private:
    ThreadPoolEx& pool_;
    bool keepInPool_;
    std::thread* thr_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_THREAD_THREAD_POOL_EX_H_
