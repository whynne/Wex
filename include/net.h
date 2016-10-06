/*
#pragma once

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <fcntl.h>

#endif

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <cstring>
#include <list>

using std::string;
using std::cout;
using std::endl;

namespace wexnet
{
    const size_t MAX_PACKET_SIZE = 512;
    
    struct Packet
    {
        unsigned int protocol;
        unsigned int sequence;
        char data[MAX_PACKET_SIZE-8]; // (MAX_PACKET_SIZE - (size of other members))
    };
    

    class Address
    {
    public:
        Address();
        Address(string address, unsigned short port);
        Address(unsigned int address, unsigned short port);
        unsigned long getAddress();
        unsigned short getPort();
    private:
        unsigned long  _address;
        unsigned short _port;
    };
    
    class WSocket
    {
    private:
        int _handle;
    
    public:
        static void startup(); //Needs to be called once before using this class.
        int  close();
        bool bind(unsigned short port);
        int  connect(Address destination);
        int  listen(int backlog);
        int  send(string message);
        int  receive(string& buffer,int maxsize);
        bool sendTo(Address destination, std::string message);
        bool sendTo(Address destination, void* data, int size);
        int  receiveFrom(Address sender, void* data, int size);
        int  setNonBlocking();
        int  setBlocking();
        bool isOpen();
    
        WSocket* accept();
        WSocket(int type,int protocol);
        WSocket(int handle);
        WSocket();
    };


    
    enum Mode;
    enum State;
    
    class Connection
    {
    private:
        unsigned int _protocolid;
        float        _timeout;
    
        Mode    _mode; 
        State   _state;
        Address _address;
        WSocket _socket;
        float   _timeoutaccumulator;
        bool    _running;
        
        std::list<Packet> _packets;
    
    public:
        void stop();
        void update(float delta);
        bool sendPacket(Packet* packet);
        void recievePacket(Packet* packet);
        Connection(unsigned int protocolid,double timeout);
        Connection();
    };
    
    
    
    //The UDP session manager will use a single UDP socket to 
    //start and manage sessions with the server/clients.  Current
    //connections will be keyed in a map by IP (unsigned int)
    
    
    
    class UdpSessionManager
    {
    private:
        std::map<unsigned int, Connection> _connections; 
        WSocket _socket;   
        bool    _running;
        Mode    _mode;
        State   _state;

        Packet getNextPacket();
    
    public:
        bool beginSessions(int port);
        void listen();
        void connect();
        void updateSessions(double delta);
        UdpSessionManager();
    };
}
*/