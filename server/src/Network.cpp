/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Network.cpp
*/

#include "Network.hpp"

namespace server {
    Network::Network(int port, int maxClients): _port(port != 0 ? port : 8081), _maxClients(maxClients != 0 ? maxClients : 4) {
        if (fillSocket() == 84 || fillAddr() == 84 || bindSocket() == 84) {
            throw std::invalid_argument("Error: Network creation failed");
        }
        std::cout << "Network created on port " << _port << " with " << _maxClients << " max clients" << std::endl;
    }

    Network::~Network() {
        #ifdef __linux__
        #endif
        #ifdef _WIN64
            closesocket(_fd);
        #endif
    }

    int Network::fillSocket() {
        int opt = 1;

        _fd = _maxClients;
        #ifdef __linux__
            _fd = socket(AF_INET, SOCK_DGRAM, 0);
            if (_fd == -1) {
                std::cerr << "Error: socket creation failed" << std::endl;
                return(84);
            }
            if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
                std::cerr << "Error: socket options failed" << std::endl;
                return(84);
            }
        #endif
        #ifdef _WIN64
            int iResult = 0;

            iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
            if (iResult != NO_ERROR) {
                std::cerr << "Error: WSAStartup failed" << std::endl;
                return(84);
            }
            _fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (_fd == INVALID_SOCKET) {
                std::cerr << "Error: socket creation failed" << std::endl;
                closesocket(_fd);
                WSACleanup();
                return(84);
            }
            if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(int)) == -1) {
                std::cerr << "Error: socket options failed" << WSAGetLastError() << std::endl;
                return(84);
            }

            unsigned read_timeout_ms = 10;
            setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&read_timeout_ms), sizeof(read_timeout_ms));
        #endif
        return 0;
    }

    int Network::fillAddr() {
        std::memset(&_addr, 0, sizeof(_addr));
        std::memset(&_clientAddr, 0, sizeof(_clientAddr));

        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(_port);
        _addr.sin_addr.s_addr = INADDR_ANY;
        _clientAddrLen = sizeof(_clientAddr);
        std::cout << "Server IP: " << inet_ntoa(_addr.sin_addr) << std::endl;
        std::cout << "Server port: " << ntohs(_addr.sin_port) << std::endl;
        return 0;
    }

    int Network::bindSocket() {
        #ifdef __linux__
            if (bind(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1) {
                std::cerr << "Error: socket binding failed" << std::endl;
                return 84;
            }
            return 0;
        #endif
        #ifdef _WIN64
            if (bind(_fd, (SOCKADDR *)&_addr, sizeof(_addr)) == -1) {
                std::cerr << "Error: socket binding failed" << std::endl;
                return 84;
            }
            return 0;
        #endif
    }
}
