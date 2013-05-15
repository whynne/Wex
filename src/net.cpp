/*
#include "net.h"

using namespace wexnet;

Address::Address()
{
    _address = 0;
    _port = 0;
}

Address::Address(string address,unsigned short port)
{
    _port = port;
    _address = 0;
    cout << "Resolving ip for " << address << "..." << endl;
    
    string portstring;
    addrinfo *resolvedip = 0;
    std::stringstream sin;
    sin << port;
    
    portstring = sin.str();
    if(getaddrinfo(address.c_str(), portstring.c_str(), 0, &resolvedip)!=0)
    {
        cout << "Failed to resolve ip for " << address << endl;
    }
    
    _address = ((sockaddr_in*)resolvedip->ai_addr)->sin_addr.s_addr;
    _address = ntohl(_address);
    
    cout << _address << endl;
}

Address::Address(unsigned int address,unsigned short port)
{
    _address = address;
    _port = port;
}


unsigned long Address::getAddress()
{    
    return _address;
}

unsigned short Address::getPort()
{
    return _port;
}

void WSocket::startup()
{
#if PLATFORM == PLATFORM_WINDOWS
    WSADATA wsadata;
    char requiredversion = 2;
    if(WSAStartup(MAKEWORD(requiredversion,0),&wsadata)==0)
    {
        if(LOBYTE(wsadata.wVersion) < requiredversion)
            exit(1);    
    }
    else
    {
        exit(1);
    }
#endif
}

int WSocket::close()
{
#if PLATFORM == PLATFORM_WINDOWS
    return closesocket(_handle);
    

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
    return ::close(_handle);
    
#endif
}

bool WSocket::bind(unsigned short port)
{
    sockaddr_in address;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(::bind(_handle,(sockaddr*)&address,sizeof(address)) < 0)
    {
        return false;
    }

    return true;
}

int WSocket::connect(Address destination)
{
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(destination.getAddress());
    address.sin_port = htons(destination.getPort());

    return ::connect(_handle,(sockaddr*)&address,(socklen_t)sizeof(address));
}

int WSocket::listen(int backlog)
{
    return ::listen(_handle,backlog);
}

WSocket* WSocket::accept()
{
    sockaddr remoteaddress;
    int remoteaddrlen;
    int remotehandle;
    
    #if PLATFORM == PLATFORM_WINDOWS
            typedef int socklen_t;
    #endif

    remoteaddrlen = sizeof(remoteaddress);
    remotehandle = ::accept(_handle,(sockaddr*)&remoteaddress,(socklen_t*)&remoteaddrlen);

    if(remotehandle <= 0)
        return 0;
    else
    {
        WSocket* newsocket = new WSocket;
        return newsocket;
    }
}

int WSocket::send(string message)
{
#if PLATFORM == PLATFORM_WINDOWS
    int result = ::send(_handle,(char*)message.c_str(),message.length(),0);
#elif PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_MAC
    int result = ::send(_handle,(void*)message.c_str(),message.length(),0);
#endif
    return result;
}


int WSocket::receive(string& buffer,int maxsize)
{
    char *buf = new char[maxsize];
    int i = recv(_handle,buf,maxsize-1,0);
    buf[i]='\0';
    buffer = string(buf);
    delete buf;
    return i;
}

bool WSocket::sendTo(Address destination, string message)
{
    if (_handle <= 0)
    {
        return false;
    }
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = destination.getAddress();
    address.sin_port = htons(destination.getPort());

    int sent_bytes = ::sendto(_handle,message.c_str(),sizeof(message.c_str()),0,(sockaddr*)&address,sizeof(destination));
    return (sent_bytes == sizeof(message.c_str()));
}

bool WSocket::sendTo(Address destination, void* data, int size)
{
    if (_handle <= 0)
    {
        return false;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = destination.getAddress();
    address.sin_port = htons(destination.getPort());

    int sent_bytes = ::sendto(_handle,(char*)&data,size,0,(sockaddr*)&address,sizeof(destination));
    return (sent_bytes == size);
}

int WSocket::receiveFrom(Address sender, void* data, int size)
{
    if(_handle == false)
        return 0;

    #if PLATFORM == PLATFORM_WINDOWS
            typedef int socklen_t;
    #endif

    sockaddr_in from;
    int fromlength = sizeof(from);
    int recieved_bytes = ::recvfrom(_handle,(char*)&data,size,0,(sockaddr*)&from,(socklen_t*)&fromlength);

    unsigned int address = ntohl(from.sin_addr.s_addr);
    unsigned short port = ntohs(from.sin_port);
    
    sender = Address(address,port);
    
    return recieved_bytes;
}

WSocket::WSocket(int type,int protocol)
{
    _handle = socket(AF_INET,type,protocol);
    if (_handle <= 0)
        cout << "WARNING: Socket not created!" << endl;
}

WSocket::WSocket(int handle)
{
    _handle = handle;
}

int WSocket::setNonBlocking()
{
#if PLATFORM == PLATFORM_WINDOWS

    DWORD nonblocking = 1;
    return ioctlsocket(_handle,FIONBIO,&nonblocking);

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

    unsigned int nonblocking = 1;
    return fcntl(_handle, F_SETFL, O_NONBLOCK, nonblocking);

#endif
}

int WSocket::setBlocking()
{
#if PLATFORM == PLATFORM_WINDOWS

    DWORD blocking = 0;
    return ioctlsocket(_handle,FIONBIO,&blocking);

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

    unsigned int blocking = 0;
    return fcntl(_handle, F_SETFL, O_NONBLOCK, blocking);

#endif
}

WSocket::WSocket()
{
}

/*

enum State
{
    DISCONNECTED,
    LISTENING,
    CONNECTING,
    CONNECTED,
    CONNECT_FAILURE
};

enum Mode
{
    SERVER,
    CLIENT,
    NONE
};

Connection::Connection(unsigned int protocolid,double timeout)
{
    _protocolid = protocolid;
    _timeout = timeout;
}

Connection::Connection()
{
    _protocolid = 0;
    _timeout = 0;
}

void Connection::stop()
{
}


void Connection::update(float delta)
{
    if(_running == false)
    {
        cout << "Warning: Attempting to update inactive connection." << endl;
        return;
    }

    //add the delta to the timeout accumulator
    _timeoutaccumulator += delta; 
    //if the accumulator exceeds the timeout threshold, stop the connection
    if(_timeoutaccumulator > _timeout)
    {
        
    }
}

bool Connection::sendPacket(Packet* packet)
{
    return true;
}

void Connection::recievePacket(Packet* packet)
{


    //reset the timeout accumulator since we recieved a packet
    _timeoutaccumulator = 0.0; 
}


bool UdpSessionManager::beginSessions(int port)
{
    _socket = WSocket(SOCK_DGRAM,IPPROTO_UDP);  // We need a UDP socket to accept incoming connections.
    if(!_socket.bind(port))                   // Bind it.  At this point, the socket can start recieving packets.
        return false;
    _socket.setNonBlocking();                 // Then set it to non-blocking so we don't wait on our recvfrom() calls
}

//Recieves and distributes all pendings packets and updates all connections

void UdpSessionManager::updateSessions(double delta)
{
    Address sender;
    char packetdata[MAX_PACKET_SIZE];
    typedef map<unsigned int,Connection> ConnectionMap;

    while(_socket.recvFrom(sender,packetdata,MAX_PACKET_SIZE)>0)
    {
        //If the IP is not on the map, make a new one.

        if(_connections.find(sender.getAddress()) == _connections.end() && _state == LISTENING)
        {
            _connections[sender.getAddress()] = Connection(IPPROTO_UDP,1.0);
        }

        //Send the packet to its destination connection
        
        Packet* recievedpacket = (Packet*)packetdata;
        _connections[sender.getAddress()].recievePacket(recievedpacket);
        

        //Iterate over and update all the connections.

        ConnectionMap::iterator pos;

        for(pos = _connections.begin();pos != _connections.end(); pos++)
        {
            pos->second.update(delta);
        }
    }
}
*/