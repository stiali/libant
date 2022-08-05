#include <sstream>

#include <unistd.h>

#include <libant/system/epoll.h>

using namespace std;

namespace ant {

EventPoll::EventPoll(int maxfd, int timeout)
{
    m_epfd = epoll_create(maxfd);
    if (m_epfd == -1) {
        throw runtime_error(string("epoll_create failed: ") + strerror(errno));
    }

    m_timeout = timeout;
    m_ev_num = 0;
    m_maxfd = maxfd;
    m_ev_hdlrs = new EventHandler[maxfd];
    m_avail_evs = new epoll_event[maxfd];
    m_plugin = 0;
}

EventPoll::~EventPoll()
{
    for (int i = 0; i != m_maxfd; ++i) {
        EventHandler* evhdlr = &m_ev_hdlrs[i];
        if (evhdlr->in_use) {
            delete evhdlr->in;
            delete evhdlr->out;
        }
    }

    delete[] m_ev_hdlrs;
    delete[] m_avail_evs;
    delete m_plugin;
    close(m_epfd);
}

void EventPoll::Dispatch()
{
    while (m_ev_num) {
        int ev_num = epoll_wait(m_epfd, m_avail_evs, m_ev_num, m_timeout);
        if ((ev_num >= 0) || (errno == EINTR)) {
            // dispatch all the ready events reported by epoll
            dispatch_ready_events(ev_num);
            // redispatch all the ready but not yet finish processing 'EventIn' events
            redispatch_in_events();
            // extra user code
            if (m_plugin) {
                m_plugin->Execute();
            }
        } else {
            throw runtime_error(string("epoll_wait failed: ") + strerror(errno));
        }
    }
}

//--------------------------------------------------
// private methods
//
void EventPoll::add_event(int fd, EventType ev_type, CallbackBase* in, CallbackBase* out, bool use_et)
{
    assert((fd < m_maxfd) && (fd > -1) && !m_ev_hdlrs[fd].in_use);

    // add to epoll
    if (epoll_control(EPOLL_CTL_ADD, fd, ev_type, use_et) == -1) {
        delete in;
        delete out;
        throw runtime_error(string("epoll_ctl (EPOLL_CTL_ADD) failed: ") + strerror(errno));
    }
    ++m_ev_num;

    // add to event handler
    EventHandler* evhdlr = &m_ev_hdlrs[fd];
    evhdlr->in_use = true;
    evhdlr->use_et = use_et;
    evhdlr->in = in;
    evhdlr->out = out;
}

int EventPoll::epoll_control(int op, int fd, EventType ev_type, bool use_et)
{
    epoll_event ev;
    ev.data.ptr = &m_ev_hdlrs[fd];
    ev.events = ev_type | event_rdhup;
    if (use_et) {
        ev.events |= event_et;
    }

    for (;;) {
        if (epoll_ctl(m_epfd, op, fd, &ev) == 0) {
            break;
        } else if (errno != EINTR) {
            return -1;
        }
    }

    return 0;
}

void EventPoll::dispatch_ready_events(int ev_num)
{
    for (int i = 0; i < ev_num; ++i) {
        epoll_event* ev = &m_avail_evs[i];
        EventHandler* evhdlr = reinterpret_cast<EventHandler*>(ev->data.ptr);
        // events that had been removed should be ignored
        if (!evhdlr->in_use) {
            continue;
        }

        // EPOLLIN: for read
        if (ev->events & EventIn) {
            evhdlr->in->Execute();
            if (!evhdlr->in_use) {
                continue;
            }
        }
        // EPOLLOUT: for write. The user code should add this event only when necessary
        if (ev->events & EventOut) {
            evhdlr->out->Execute();
            if (!evhdlr->in_use) {
                continue;
            }
        }
        // EPOLLRDHUP: If a client send some data to a server and than close the connection
        //					  immediately, the server will receive RDHUP and IN at the same time.
        //					  Under ET mode, this can make a sockfd into CLOSE_WAIT state.
        // EPOLLHUP: When close of a fd is detected (ie, after receiving a RST segment:
        //				   the client has closed the socket, and the server has performed
        //				   one write on the closed socket.)
        // After receiving EPOLLRDHUP or EPOLLHUP, we can still read data from the fd until
        // read() returns 0 indicating EOF is reached. So, we should alway call read on receving
        // this kind of events to aquire the remaining data and/or EOF. (Linux-2.6.18)
        if (ev->events & (event_rdhup | event_hup)) {
            evhdlr->in->Execute();
            continue;
        }

        // TODO: EPOLLERR: for read & write? Test it! Modify (or remove) it latter!
        if ((ev->events & event_err) && !(ev->events & (EventIn | event_hup | EventOut | event_rdhup))) {
            ostringstream oss;
            oss << "Unexpected Event Happend: fd=" << evhdlr - &m_ev_hdlrs[0] << "event=" << hex << ev->events;
            throw runtime_error(oss.str());
        }
    }
}

void EventPoll::redispatch_in_events()
{
    while (m_read_evs.size()) {
        EventHandler* evhdlr = m_read_evs.front();
        m_read_evs.pop();

        if (evhdlr->in_use) {
            evhdlr->in->Execute();
        }
    }
}

} // namespace ant
