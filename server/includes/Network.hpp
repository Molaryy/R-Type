/*
** EPITECH PROJECT, 2024
** B-CPP-500-PAR-5-2-bsrtype-arthaud.poupard
** File description:
** Network.hpp
*/

#ifdef __linux__
    #pragma once
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define OS "linux"
#endif

#ifdef _WIN64
    #pragma comment(lib, "Ws2_32.lib")
    #define NOGDI
    #define NOUSER
    #define MMNOSOUND
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <io.h>
    #include <process.h>
    #define OS "windows"
#endif

#include <string>
#include <iostream>
#include <cstring>

namespace server {
    class Network {
        public:
            Network(int port, int maxClients);
            ~Network();
            void run();
        private:
            int _port;
            unsigned int _maxClients;
            bool _isRunning = true;
            int _fd;
            #ifdef __linux__
                struct sockaddr_in _addr;
                struct sockaddr_in _clientAddr;
                socklen_t _clientAddrLen;
            #endif
            #ifdef _WIN64
                struct sockaddr_in _addr;
                struct sockaddr_in _clientAddr;
                socklen_t _clientAddrLen;
                WSADATA _wsaData;
            #endif
            int fillSocket();
            int fillAddr();
            int bindSocket();
    };
}
