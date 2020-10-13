#include "gridftp-server.h"
/*
 ***************************************************************************************************
 * description: initialize metadetaserver instance
 ***************************************************************************************************
 */
void GridftpServerApp::initialize(void){
    ctrlPort = 2021;
    dataPort = 50000;
}

GridftpServerApp::GridftpServerApp(u_int32_t id){
    GridftpServerApp::id = id;
    GridftpServerApp::initialize();
}

GridftpServerApp::GridftpServerApp(Ptr< Node > node){
    GridftpServerApp::initialize();
    GridftpServerApp::SetNode(node);
}

GridftpServerApp::~GridftpServerApp(void){

}
/*
 ***************************************************************************************************
 * description: Notify application when space in transmit buffer is added.
 ***************************************************************************************************
 */
void GridftpServerApp::sendCallbackCtrl(Ptr<Socket> socket, uint32_t nbytes){
    int n;
    if(clientsTable[socket].nBytes2Send > 0){
        n = min(clientsTable[socket].nBytes2Send, (u_int64_t)nbytes);
        socket->Send(Create<Packet>(n));
        clientsTable[socket].nBytes2Send -= n;
    }
    NS_LOG_DEBUG(Simulator::Now().GetSeconds() << " sendCallbackCtrl called with nbytes = " << nbytes << " actually sent " << n << " bytes." << 
        " There are " << clientsTable[socket].nBytes2Send << " bytes need to be sent" << endl );
}

void GridftpServerApp::sendCallbackData(Ptr<Socket> socket, uint32_t nbytes){
    int n;
    if(clientsTable[socket].nBytes2Send > 0){
        n = min(clientsTable[socket].nBytes2Send, (u_int64_t)nbytes);
        socket->Send(Create<Packet>(n));
        clientsTable[socket].nBytes2Send -= n;
    }
    NS_LOG_DEBUG( Simulator::Now().GetSeconds() << " sendCallbackData called with nbytes = " << nbytes << " actually sent " << n << " bytes." << 
        " There are " << clientsTable[socket].nBytes2Send << " bytes need to be sent" << endl);
}
/*
 ***************************************************************************************************
 * description: this is the callback function when Object Storage server completed the request
 ***************************************************************************************************
 */
void GridftpServerApp::responseCallbackCtrl(Ptr<Packet> msg, Ptr<Socket> socket){
    if (reqFifo.empty()){
        nInService -= 1;
    }
    else{
        std::tuple<Ptr<Packet>, Ptr<Socket> > req = reqFifo.front();
        Simulator::Schedule (serviceTime() , &GridftpServerApp::responseCallbackCtrl, this, std::get<0>(req), std::get<1>(req));
        reqFifo.pop();
    }
}
/*
 ***************************************************************************************************
 * description: this function will be called when received a package from the control channel
 ***************************************************************************************************
 */
void GridftpServerApp::handleCtrlReq(Ptr<Packet> msg, Ptr<Socket> socket){
    if (nInService < maxConcurrence){
        Simulator::Schedule (serviceTime() , &GridftpServerApp::responseCallbackCtrl, this, msg, socket);
        nInService += 1;
    }
    else{
        reqFifo.push(std::make_tuple(msg, socket));
    }
}
/*
 ***************************************************************************************************
 * description: this is the callback function receive a packet from control channel socket
 ***************************************************************************************************
 */
void GridftpServerApp::handleRecvCtrl(Ptr<Socket> socket){
    Ptr<Packet> msg = socket->Recv();
    if(msg == NULL){
        return;
    }
    NS_LOG_DEBUG(Simulator::Now().GetSeconds () << " GridFTP Server for [" << srcName << " => " << dstName << "], " << 
        id << "with IP = " << GetNode()->GetObject<Ipv4> () -> GetAddress (1, 0).GetLocal () << 
              " received a packet with " << msg->GetSize() << " bytes " << std::endl); 
    clientsTable[socket].recvBufLen += msg->GetSize();
    NS_LOG_DEBUG(Simulator::Now ().GetSeconds () << " There are " << clientsTable[socket].recvBufLen << " bytes received from " << 
    clientsTable[socket].addr << " in the buffer now" << std::endl);
    GridftpServerApp::handleCtrlReq(msg, socket);
}

void GridftpServerApp::handleRecvData(Ptr<Socket> socket){
    Ptr<Packet> msg = socket->Recv();
}
/*
 ***************************************************************************************************
 * description: callback when tcp coonection get closed
 ***************************************************************************************************
 */
void GridftpServerApp::handlePeerClose(Ptr<Socket>){
}
/*
 ***************************************************************************************************
 * description: callback when tcp coonection get error
 ***************************************************************************************************
 */
void GridftpServerApp::handlePeerError(Ptr<Socket>){
}
/*
 ***************************************************************************************************
 * description: callback when controll tcp socket accepts a new connection
 ***************************************************************************************************
 */
void GridftpServerApp::handleAcceptCtrl(Ptr<Socket> socket, const Address& from){
    InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (from);
    // initialize a new client instance
    NS_LOG_DEBUG(Simulator::Now().GetSeconds() << " GridFTP Server for [" << srcName << " => " << dstName << "], " << id << 
              " ctrl channel accepted a new connection from " << 
              iaddr.GetIpv4 () << ":" << iaddr.GetPort () << std::endl);

    gridftpClientInfo client;
    client.socket = socket;
    client.addr = from;
    client.recvBufLen = 0;
    client.nBytes2Send = 2811;
    client.isCtrl = true;
    
    // add client to client list
    clientsTable[socket] = client;
    
    socket->SetRecvCallback(MakeCallback(&GridftpServerApp::handleRecvCtrl, this));
    socket->SetSendCallback(MakeCallback(&GridftpServerApp::sendCallbackCtrl, this));
}

// data channel 
void GridftpServerApp::handleAcceptData(Ptr<Socket> socket, const Address& from){
    InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (from);
    NS_LOG_DEBUG(Simulator::Now().GetSeconds() << " GridFTP Server for [" << srcName << " => " << dstName << "], " << id << 
              " data channel accepted a new connection from " << iaddr.GetIpv4 () << ":" << iaddr.GetPort () << std::endl);
    // initialize a new client instance
    gridftpClientInfo client;
    client.socket = socket;
    client.addr = from;
    client.recvBufLen = 0;
    client.nBytes2Send = 1e9;
    client.isCtrl = false;
    
    clientsTable[socket] = client;
    
    socket->SetRecvCallback(MakeCallback(&GridftpServerApp::handleRecvData, this));
    socket->SetSendCallback(MakeCallback(&GridftpServerApp::sendCallbackData, this));

}
/*
 ***************************************************************************************************
 * description: this function gives the service time for one request, this should depends on server'
                hardware spec and current load
 ***************************************************************************************************
 */
Time GridftpServerApp::serviceTime(){
    double ts = .001;
    return Seconds(ts);
}

/*
 ***************************************************************************************************
 * description: The StartApplication method is called at the start time specified by Start
 ***************************************************************************************************
 */
void GridftpServerApp::StartApplication(){
    NS_LOG_DEBUG(Simulator::Now().GetSeconds() << " GridFTP Server for [" << srcName << " => " << dstName << "], " << id << 
              " started, my Ipv4Address: " << GetNode()->GetObject<Ipv4> () -> GetAddress (1, 0).GetLocal () 
              << ". ctrl channel is listening on " << ctrlPort << ", data channel is listening on " << dataPort << std::endl); 
    listenCtrlSocket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId ());
    listenCtrlSocket->Bind(InetSocketAddress(Ipv4Address::GetAny(), ctrlPort));
    listenCtrlSocket->Listen();
    listenCtrlSocket->SetAcceptCallback(MakeNullCallback<bool, Ptr<Socket>, const Address &>(), MakeCallback(&GridftpServerApp::handleAcceptCtrl, this));
    listenCtrlSocket->SetCloseCallbacks(MakeCallback(&GridftpServerApp::handlePeerClose, this), MakeCallback(&GridftpServerApp::handlePeerError, this));

    listenDataSocket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId ());
    listenDataSocket->Bind(InetSocketAddress(Ipv4Address::GetAny(), dataPort));
    listenDataSocket->Listen();
    listenDataSocket->SetAcceptCallback(MakeNullCallback<bool, Ptr<Socket>, const Address &>(), MakeCallback(&GridftpServerApp::handleAcceptData, this));
    listenDataSocket->SetCloseCallbacks(MakeCallback(&GridftpServerApp::handlePeerClose, this), MakeCallback(&GridftpServerApp::handlePeerError, this));
}

/*
 ***************************************************************************************************
 * description: The StopApplication method is called at the stop time specified by Stop
 ***************************************************************************************************
 */
void GridftpServerApp::StopApplication(){
    NS_LOG_DEBUG(Simulator::Now ().GetSeconds() << " GridFTP Server for [" << srcName << " => " << dstName << "], " << id << 
              " stopped" << std::endl); 
}