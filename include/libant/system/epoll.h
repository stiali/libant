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

#ifndef LIBANT_INCLUDE_LIBANT_SYSTEM_EPOLL_H_
#define LIBANT_INCLUDE_LIBANT_SYSTEM_EPOLL_H_

#include <cassert>
#include <cerrno>
#include <cstring>

#include <queue>
#include <stdexcept>
#include <string>

#include <sys/epoll.h>

namespace ant {

class EventPoll {
private:
    // forbid copy and assignment
    EventPoll(const EventPoll&) = delete;
    EventPoll& operator=(const EventPoll&) = delete;

    class CallbackBase {
    public:
        virtual void Execute() = 0;
        virtual ~CallbackBase(){};
    };

    class CallbackV : public CallbackBase {
    public:
        typedef void (*Method)();

    public:
        CallbackV(Method method)
        {
            m_func = method;
        }

        void Execute()
        {
            m_func();
        }

    private:
        Method m_func;
    };

    class Callback0 : public CallbackBase {
    public:
        typedef void (*Method)(int);

    public:
        Callback0(Method method, int fd)
        {
            m_func = method;
            m_fd = fd;
        }

        void Execute()
        {
            m_func(m_fd);
        }

    private:
        Method m_func;
        int m_fd;
    };

    template<typename P1>
    class Callback1 : public CallbackBase {
    public:
        typedef void (*Method)(int, P1&);

    public:
        Callback1(Method method, int fd, const P1& p1)
        {
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
        }

        void Execute()
        {
            m_func(m_fd, m_p1);
        }

    private:
        Method m_func;
        int m_fd;
        P1 m_p1;
    };

    template<typename P1, typename P2>
    class Callback2 : public CallbackBase {
    public:
        typedef void (*Method)(int, P1&, P2&);

    public:
        Callback2(Method method, int fd, const P1& p1, const P2& p2)
        {
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
            m_p2 = p2;
        }

        void Execute()
        {
            m_func(m_fd, m_p1, m_p2);
        }

    private:
        Method m_func;
        int m_fd;
        P1 m_p1;
        P2 m_p2;
    };

    template<typename P1, typename P2, typename P3>
    class Callback3 : public CallbackBase {
    public:
        typedef void (*Method)(int, P1&, P2&, P3&);

    public:
        Callback3(Method method, int fd, const P1& p1, const P2& p2, const P3& p3)
        {
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
            m_p2 = p2;
            m_p3 = p3;
        }

        void Execute()
        {
            m_func(m_fd, m_p1, m_p2, m_p3);
        }

    private:
        Method m_func;
        int m_fd;
        P1 m_p1;
        P2 m_p2;
        P3 m_p3;
    };

    template<typename P1, typename P2, typename P3, typename P4>
    class Callback4 : public CallbackBase {
    public:
        typedef void (*Method)(int, P1&, P2&, P3&, P4&);

    public:
        Callback4(Method method, int fd, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
        {
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
            m_p2 = p2;
            m_p3 = p3;
            m_p4 = p4;
        }

        void Execute()
        {
            m_func(m_fd, m_p1, m_p2, m_p3, m_p4);
        }

    private:
        Method m_func;
        int m_fd;
        P1 m_p1;
        P2 m_p2;
        P3 m_p3;
        P4 m_p4;
    };

    template<typename Class>
    class CallbackPV : public CallbackBase {
    public:
        typedef void (Class::*Method)();

    public:
        CallbackPV(Class* class_instance, Method method)
        {
            m_obj = class_instance;
            m_func = method;
        }

        void Execute()
        {
            (m_obj->*m_func)();
        }

    private:
        Class* m_obj;
        Method m_func;
    };

    template<typename Class>
    class CallbackP0 : public CallbackBase {
    public:
        typedef void (Class::*Method)(int);

    public:
        CallbackP0(Class* class_instance, Method method, int fd)
        {
            m_obj = class_instance;
            m_func = method;
            m_fd = fd;
        }

        void Execute()
        {
            (m_obj->*m_func)(m_fd);
        }

    private:
        Class* m_obj;
        Method m_func;
        int m_fd;
    };

    template<typename Class, typename P1>
    class CallbackP1 : public CallbackBase {
    public:
        typedef void (Class::*Method)(int, P1&);

    public:
        CallbackP1(Class* class_instance, Method method, int fd, const P1& p1)
        {
            m_obj = class_instance;
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
        }

        void Execute()
        {
            (m_obj->*m_func)(m_fd, m_p1);
        }

    private:
        Class* m_obj;
        Method m_func;
        int m_fd;
        P1 m_p1;
    };

    template<typename Class, typename P1, typename P2>
    class CallbackP2 : public CallbackBase {
    public:
        typedef void (Class::*Method)(int, P1&, P2&);

    public:
        CallbackP2(Class* class_instance, Method method, int fd, const P1& p1, const P2& p2)
        {
            m_obj = class_instance;
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
            m_p2 = p2;
        }

        void Execute()
        {
            (m_obj->*m_func)(m_fd, m_p1, m_p2);
        }

    private:
        Class* m_obj;
        Method m_func;
        int m_fd;
        P1 m_p1;
        P2 m_p2;
    };

    template<class Class, typename P1, typename P2, typename P3>
    class CallbackP3 : public CallbackBase {
    public:
        typedef void (Class::*Method)(int, P1&, P2&, P3&);

    public:
        CallbackP3(Class* class_instance, Method method, int fd, const P1& p1, const P2& p2, const P3& p3)
        {
            m_obj = class_instance;
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
            m_p2 = p2;
            m_p3 = p3;
        }

        void Execute()
        {
            (m_obj->*m_func)(m_fd, m_p1, m_p2, m_p3);
        }

    private:
        Class* m_obj;
        Method m_func;
        int m_fd;
        P1 m_p1;
        P2 m_p2;
        P3 m_p3;
    };

    template<class Class, typename P1, typename P2, typename P3, typename P4>
    class CallbackP4 : public CallbackBase {
    public:
        typedef void (Class::*Method)(int, P1&, P2&, P3&, P4&);

    public:
        CallbackP4(Class* class_instance, Method method, int fd, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
        {
            m_obj = class_instance;
            m_func = method;
            m_fd = fd;
            m_p1 = p1;
            m_p2 = p2;
            m_p3 = p3;
            m_p4 = p4;
        }

        void Execute()
        {
            (m_obj->*m_func)(m_fd, m_p1, m_p2, m_p3, m_p4);
        }

    private:
        Class* m_obj;
        Method m_func;
        int m_fd;
        P1 m_p1;
        P2 m_p2;
        P3 m_p3;
        P4 m_p4;
    };

    struct EventHandler {
    public:
        EventHandler()
        {
            in_use = false;
        }

    public:
        bool in_use;
        bool use_et;
        CallbackBase* in;
        CallbackBase* out;
    };

public:
    enum EventType {
        EventIn = EPOLLIN,
        EventOut = EPOLLOUT,
        EventIO = EventIn | EventOut,

        // internal use only
        event_err = EPOLLERR,
        event_hup = EPOLLHUP,
        event_rdhup = EPOLLRDHUP,
        event_et = EPOLLET
    };

public:
    EventPoll(int maxfd = 20000, int timeout = -1);
    ~EventPoll();

    void AddPlugin(void (*plugin)())
    {
        assert(m_plugin == 0);

        m_plugin = new CallbackV(plugin);
    }

    template<typename Class>
    void AddPlugin(Class& obj, void (Class::*plugin)())
    {
        assert(m_plugin == 0);

        m_plugin = new CallbackPV<Class>(&obj, plugin);
    }

    void Add(int fd, EventType ev_type, void (*incb)(int fd), void (*outcb)(int fd), bool use_et = true)
    {
        CallbackBase* in = new Callback0(incb, fd);
        CallbackBase* out = new Callback0(outcb, fd);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename In1, typename Out1>
    void Add(int fd, EventType ev_type, void (*incb)(int fd, In1&), void (*outcb)(int fd, Out1&), const In1& in1 = In1(), const Out1& out1 = Out1(),
             bool use_et = true)
    {
        CallbackBase* in = new Callback1<In1>(incb, fd, in1);
        CallbackBase* out = new Callback1<Out1>(outcb, fd, out1);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename In1, typename In2, typename Out1, typename Out2>
    void Add(int fd, EventType ev_type, void (*incb)(int fd, In1&, In2&), void (*outcb)(int fd, Out1&, Out2&), const In1& in1 = In1(), const In2& in2 = In2(),
             const Out1& out1 = Out1(), const Out2& out2 = Out2(), bool use_et = true)
    {
        CallbackBase* in = new Callback2<In1, In2>(incb, fd, in1, in2);
        CallbackBase* out = new Callback2<Out1, Out2>(outcb, fd, out1, out2);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename In1, typename In2, typename In3, typename Out1, typename Out2, typename Out3>
    void Add(int fd, EventType ev_type, void (*incb)(int fd, In1&, In2&, In3&), void (*outcb)(int fd, Out1&, Out2&, Out3&), const In1& in1 = In1(),
             const In2& in2 = In2(), const In3& in3 = In3(), const Out1& out1 = Out1(), const Out2& out2 = Out2(), const Out3& out3 = Out3(),
             bool use_et = true)
    {
        CallbackBase* in = new Callback3<In1, In2, In3>(incb, fd, in1, in2, in3);
        CallbackBase* out = new Callback3<Out1, Out2, Out3>(outcb, fd, out1, out2, out3);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename In1, typename In2, typename In3, typename In4, typename Out1, typename Out2, typename Out3, typename Out4>
    void Add(int fd, EventType ev_type, void (*incb)(int fd, In1&, In2&, In3&, In4&), void (*outcb)(int fd, Out1&, Out2&, Out3&, Out4&), const In1& in1 = In1(),
             const In2& in2 = In2(), const In3& in3 = In3(), const In4& in4 = In4(), const Out1& out1 = Out1(), const Out2& out2 = Out2(),
             const Out3& out3 = Out3(), const Out4& out4 = Out4(), bool use_et = true)
    {
        CallbackBase* in = new Callback4<In1, In2, In3, In4>(incb, fd, in1, in2, in3, in4);
        CallbackBase* out = new Callback4<Out1, Out2, Out3, Out4>(outcb, fd, out1, out2, out3, out4);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename Class>
    void Add(int fd, EventType ev_type, Class& obj, void (Class::*incb)(), void (Class::*outcb)(), bool use_et = true)
    {
        CallbackBase* in = new CallbackPV<Class>(&obj, incb);
        CallbackBase* out = new CallbackPV<Class>(&obj, outcb);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename Class>
    void Add(int fd, EventType ev_type, Class& obj, void (Class::*incb)(int fd), void (Class::*outcb)(int fd), bool use_et = true)
    {
        CallbackBase* in = new CallbackP0<Class>(&obj, incb, fd);
        CallbackBase* out = new CallbackP0<Class>(&obj, outcb, fd);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename Class, typename In1, typename Out1>
    void Add(int fd, EventType ev_type, Class& obj, void (Class::*incb)(int fd, In1&), void (Class::*outcb)(int fd, Out1&), const In1& in1 = In1(),
             const Out1& out1 = Out1(), bool use_et = true)
    {
        CallbackBase* in = new CallbackP1<Class, In1>(&obj, incb, fd, in1);
        CallbackBase* out = new CallbackP1<Class, Out1>(&obj, outcb, fd, out1);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename Class, typename In1, typename In2, typename Out1, typename Out2>
    void Add(int fd, EventType ev_type, Class& obj, void (Class::*incb)(int fd, In1&, In2&), void (Class::*outcb)(int fd, Out1&, Out2&), const In1& in1 = In1(),
             const In2& in2 = In2(), const Out1& out1 = Out1(), const Out2& out2 = Out2(), bool use_et = true)
    {
        CallbackBase* in = new CallbackP2<Class, In1, In2>(&obj, fd, incb, in1, in2);
        CallbackBase* out = new CallbackP2<Class, Out1, Out2>(&obj, fd, outcb, out1, out2);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename Class, typename In1, typename In2, typename In3, typename Out1, typename Out2, typename Out3>
    void Add(int fd, EventType ev_type, Class& obj, void (Class::*incb)(int fd, In1&, In2&, In3&), void (Class::*outcb)(int fd, Out1&, Out2&, Out3&),
             const In1& in1 = In1(), const In2& in2 = In2(), const In3& in3 = In3(), const Out1& out1 = Out1(), const Out2& out2 = Out2(),
             const Out3& out3 = Out3(), bool use_et = true)
    {
        CallbackBase* in = new CallbackP3<Class, In1, In2, In3>(&obj, incb, fd, in1, in2, in3);
        CallbackBase* out = new CallbackP3<Class, Out1, Out2, Out3>(&obj, outcb, fd, out1, out2, out3);

        add_event(fd, ev_type, in, out, use_et);
    }

    template<typename Class, typename In1, typename In2, typename In3, typename In4, typename Out1, typename Out2, typename Out3, typename Out4>
    void Add(int fd, EventType ev_type, Class& obj, void (Class::*incb)(int fd, In1&, In2&, In3&, In4&),
             void (Class::*outcb)(int fd, Out1&, Out2&, Out3&, Out4&), const In1& in1 = In1(), const In2& in2 = In2(), const In3& in3 = In3(),
             const In4& in4 = In4(), const Out1& out1 = Out1(), const Out2& out2 = Out2(), const Out3& out3 = Out3(), const Out4& out4 = Out4(),
             bool use_et = true)
    {
        CallbackBase* in = new CallbackP4<Class, In1, In2, In3, In4>(&obj, incb, fd, in1, in2, in3, in4);
        CallbackBase* out = new CallbackP4<Class, Out1, Out2, Out3, Out4>(&obj, outcb, fd, out1, out2, out3, out4);

        add_event(fd, ev_type, in, out, use_et);
    }

    void Modify(int fd, EventType ev_type)
    {
        assert((fd < m_maxfd) && (fd > -1) && m_ev_hdlrs[fd].in_use);

        if (epoll_control(EPOLL_CTL_MOD, fd, ev_type, m_ev_hdlrs[fd].use_et) == -1) {
            throw std::runtime_error(std::string("epoll_ctl (EPOLL_CTL_MOD) failed: ") + strerror(errno));
        }
    }

    void Remove(int fd)
    {
        assert((fd < m_maxfd) && (fd > -1) && m_ev_hdlrs[fd].in_use);

        EventHandler* evhdlr = &m_ev_hdlrs[fd];
        evhdlr->in_use = false;
        delete evhdlr->in;
        delete evhdlr->out;
        --m_ev_num;

        epoll_control(EPOLL_CTL_DEL, fd, EventIn, false);
    }

    void ReportAgainIn(int fd)
    {
        assert((fd < m_maxfd) && (fd > -1) && m_ev_hdlrs[fd].in_use);

        m_read_evs.push(&m_ev_hdlrs[fd]);
    }

    void Dispatch();

private:
    void add_event(int fd, EventType ev_type, CallbackBase* in, CallbackBase* out, bool use_et);
    int epoll_control(int op, int fd, EventType ev_type, bool use_et);
    // dispatch all the ready events reported by epoll
    void dispatch_ready_events(int ev_num);
    // redispatch all the ready but not yet finish processing 'EventIn' events
    void redispatch_in_events();

private:
    /*! epoll fd */
    int m_epfd;
    /*! timeout (in millisecond) for epoll_wait */
    int m_timeout;
    /*! number of events added to epoll */
    int m_ev_num;
    /*! maximum fd */
    int m_maxfd;
    /*! hold callbacks of each fd (up to m_maxfd elements) */
    EventHandler* m_ev_hdlrs;
    /*! contains the available events */
    epoll_event* m_avail_evs;
    /*! contains events to report again for reading */
    std::queue<EventHandler*> m_read_evs;
    /*! extra user code */
    CallbackBase* m_plugin;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_SYSTEM_EPOLL_H_
