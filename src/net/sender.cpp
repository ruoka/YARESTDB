#include <system_error>
#include "net/sender.hpp"
#include "net/address_info.hpp"
#include "net/endpointbuf.hpp"

namespace net {

oendpointstream sender::distribute()
{
    return net::distribute(m_group, m_service_or_port);
}

oendpointstream distribute(const std::string& group, const std::string& service_or_port, unsigned ttl)
{
    const net::address_info distribution_address{group, service_or_port, SOCK_DGRAM};
    for(const auto& address : distribution_address)
    {
        net::socket s{address.ai_family, address.ai_socktype, address.ai_protocol};
        if(!s)
            continue;

        auto t2l = ttl;
        auto status = net::setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &t2l, sizeof t2l);
        if(status < 0)
            continue;

        status = net::connect(s, address.ai_addr, address.ai_addrlen);
        if(status < 0)
            continue;

        return new endpointbuf<udp_buffer_size>{std::move(s)};
    }

    throw std::system_error{errno, std::system_category()};
}

} // namespace net
