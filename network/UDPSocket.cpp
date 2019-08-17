#include "network/UDPSocket.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <string.h> // memset
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring> // strerror

void UDPSocket::open(std::string hostname, int port_no) {
    std::string port_str = std::to_string(port_no);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    int r(getaddrinfo(hostname.c_str(), port_str.c_str(), &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL)
    {
        throw socket_runtime_error(("invalid address or port: \"" + hostname + ":" + port_str + "\"").c_str());
    }
    sock = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(sock == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw socket_runtime_error(("could not create socket for: \"" + hostname + ":" + port_str + "\"").c_str());
    }
}

void UDPSocket::send(std::vector<uint8_t> raw_message) {
    auto x = sendto(sock, raw_message.data(), raw_message.size(), 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);
    if (x == -1) {
        std::cout << "send failed: " << std::strerror(errno) << '\n';
    } else {
        //std::cout << "Sent " << x << " bytes\n";
    }
}

std::vector<uint8_t> UDPSocket::recv() {
    std::vector<uint8_t> buffer(1024);

    //std::cout << "Trying to receive\n";
    auto n_bytes = recvfrom(sock,buffer.data(),buffer.size(),0, f_addrinfo->ai_addr, &f_addrinfo->ai_addrlen);

    if (n_bytes != -1) {
        buffer.resize(n_bytes);
    } else {
        throw socket_runtime_error("Failed to recv from socket");
    }
    
    return buffer;
}

UDPSocket::~UDPSocket() {
    freeaddrinfo(f_addrinfo);
    close(sock);
}
