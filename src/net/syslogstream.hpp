#pragma once

#include <mutex>
#include <experimental/type_traits>
#include "std/extension.hpp"
#include "net/sender.hpp"

namespace net {
namespace syslog {

    enum class facility : int
    {
        user   = 1,
        local0 = 16,
        local1 = 17,
        local2 = 18,
        local3 = 19,
        local4 = 20,
        local5 = 21,
        local6 = 22,
        local7 = 23
    };

    enum class severity : int
    {
         emergency = 0,
         alert     = 1,
         critical  = 2,
         error     = 3,
         warning   = 4,
         notice    = 5,
         info      = 6,
         debug     = 7
    };

    std::string gethostname();

    int getpid();

    const auto hostname = gethostname();

    const auto pid = getpid();

} // namespace syslog

class syslogstream : public oendpointstream
{
public:

    syslogstream(oendpointstream&& s) :
        oendpointstream{std::move(s)},
        m_facility{syslog::facility::user},
        m_severity{syslog::severity::debug},
        m_level{syslog::severity::debug},
        m_tag{"syslogstream"}
    {}

    void redirect(std::ostream& os)
    {
        rdbuf(os.rdbuf());
    }

    void facility(syslog::facility f)
    {
        m_facility = f;
    }

    void level(syslog::severity s)
    {
        m_level = s;
    }

    void tag(const std::string& tag)
    {
        m_tag = tag;
    }

    void severity(syslog::severity s)
    {
        m_mutex.lock();
        m_severity = s;
    }

    void header()
    {
        // <PRI> Feb 22 21:12 localhost syslog[2112]:
        using namespace std;
        using namespace std::chrono;

        if(m_level >= m_severity)
        {
            const auto timestamp = convert(system_clock::now());
            static_cast<oendpointstream&>(*this)
                    << '<' << priority(m_facility, m_severity) << '>'             // <PRI>
                    << setw(3) << to_string(get<months>(timestamp))       << ' '  // TIMESTAMP
                    << setw(2) << setfill(' ') << get<days>(timestamp)    << ' '
                    << setw(2) << setfill('0') << get<hours>(timestamp)   << ':'
                    << setw(2) << setfill('0') << get<minutes>(timestamp) << ':'
                    << setw(2) << setfill('0') << get<seconds>(timestamp) << ' '
                    << syslog::hostname                                   << ' '  // HOSTNAME
                    << m_tag << '[' << syslog::pid << ']' << ':'          << ' '; // TAG[PID]:
        }
    }

    auto& operator<< (syslogstream& (*pf)(syslogstream&))
    {
        (*pf)(*this);
        return *this;
    }

    template <typename T,
              typename = std::enable_if_t<!std::experimental::is_pointer_v<T>>>
    auto& operator<< (const T& type)
    {
        if(m_level >= m_severity)
            static_cast<oendpointstream&>(*this) << type;
        return *this;
    }

    auto& operator<< (const char* str)
    {
        if(m_level >= m_severity)
            static_cast<oendpointstream&>(*this) << str;
        return *this;
    }

    auto& flush()
    {
        if(m_level >= m_severity)
            put('\n').flush();
        m_mutex.unlock();
        return *this;
    }

private:

    int priority(syslog::facility f, syslog::severity s)
    {
        auto p = 8;
        p *= static_cast<int>(f);
        p += static_cast<int>(s);
        return p;
    }

    syslog::facility m_facility;

    syslog::severity m_severity;

    syslog::severity m_level;

    std::string m_tag;

    std::mutex m_mutex;
};

inline auto& error(syslogstream& sl)
{
    sl.severity(syslog::severity::error);
    sl.header();
    return sl;
}

inline auto& warning(syslogstream& sl)
{
    sl.severity(syslog::severity::warning);
    sl.header();
    return sl;
}

inline auto& notice(syslogstream& sl)
{
    sl.severity(syslog::severity::notice);
    sl.header();
    return sl;
}

inline auto& info(syslogstream& sl)
{
    sl.severity(syslog::severity::info);
    sl.header();
    return sl;
}

inline auto& debug(syslogstream& sl)
{
    sl.severity(syslog::severity::debug);
    sl.header();
    return sl;
}

inline auto& flush(syslogstream& sl)
{
    sl.flush();
    return sl;
}

static syslogstream slog{distribute("localhost","syslog")};

} // namespace net
