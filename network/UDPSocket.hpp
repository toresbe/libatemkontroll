#pragma once
#include <string>
#include <vector>
#include <stdexcept>


class socket_runtime_error: public std::runtime_error
{
    public:
        socket_runtime_error(const char *w) : std::runtime_error(w) {}
};
class UDPSocket {
    private:
        int sock;
    struct addrinfo *   f_addrinfo;
    public:
    ~UDPSocket();
    void open(std::string hostname, int port_no = 9910);
    void send(std::vector<uint8_t> raw_message);
    std::vector<uint8_t> recv();
};
