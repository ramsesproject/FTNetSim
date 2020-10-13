#include "gridftp-client.h"
namespace ns3 {
  NS_LOG_COMPONENT_DEFINE ("GridFTPClient");
}
using namespace ns3;
/*
 ***************************************************************************************************
 * func   name: 
 * description: initialize gridft client instance
 * parameters :
 *             none
 * return: none
 ***************************************************************************************************
 */
 GridftpClientApp::GridftpClientApp(uint16_t ctrl_port, uint16_t data_port, string server_ip, 
                                    u_int32_t id, u_int32_t n_tcp){
    GridftpClientApp::ctrlPort = ctrl_port;
    GridftpClientApp::dataPort = data_port;
    GridftpClientApp::serverAddr = Ipv4Address(server_ip.c_str());
    GridftpClientApp::id = id;
    GridftpClientApp::nTCPStream = n_tcp;
 }

GridftpClientApp::GridftpClientApp(u_int32_t id){
     GridftpClientApp::id = id;
 }
 /*
 ***************************************************************************************************
 * func   name: handleCtrlRecv
 * description: callback when controll has new packet to read
 * parameters :
 *             none
 * return: none
 ***************************************************************************************************
 */
void GridftpClientApp::handleCtrlRecv(Ptr<Socket> socket){
    Ptr<Packet> msg = socket->Recv();
    if(msg == NULL){
        std::cout << "no packet avaliable to receive!" << std::endl;
        return;
    }
    Address addr;
    socket->GetPeerName (addr);
    InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (addr);
    NS_LOG_DEBUG( Simulator::Now ().GetSeconds () << " GridFTP Client for [" << srcName << " => " << dstName << "], with IP = " << 
              GetNode()->GetObject<Ipv4> () -> GetAddress (1, 0).GetLocal () << 
              " received a packet with " << msg->GetSize() << " bytes from Ctrl Channel " << iaddr.GetIpv4 () << ":" << 
              iaddr.GetPort () << std::endl); 
}

void GridftpClientApp::handleDataRecv(Ptr<Socket> socket){
    Ptr<Packet> msg = socket->Recv();
    if(msg == NULL){
        std::cout << "no packet avaliable to receive!" << std::endl;
        return;
    }
    Address addr;
    socket->GetPeerName (addr);
    InetSocketAddress iaddr = InetSocketAddress::ConvertFrom (addr);
    NS_LOG_DEBUG( Simulator::Now ().GetSeconds () << " Client with IP = " << GetNode()->GetObject<Ipv4> () -> GetAddress (1, 0).GetLocal () << 
              " received a packet with " << msg->GetSize() << " bytes from Data Channel " << iaddr.GetIpv4 () << ":" << iaddr.GetPort () << std::endl); 
    workerDataRecv[socket].recv_byte += msg->GetSize();
}
 /*
 ***************************************************************************************************
 * func   name: StartApplication
 * description: The StartApplication method is called at the start time specified by Start
 * parameters :
 *             none
 * return: none
 ***************************************************************************************************
 */
void GridftpClientApp::StartApplication(){
    NS_LOG_DEBUG( Simulator::Now ().GetSeconds () << " GridFTP Client for [" << srcName << " => " << dstName << "], " << id <<
    " started, my Ipv4Address: " << GetNode()->GetObject<Ipv4> () -> GetAddress (1, 0).GetLocal () << std::endl);
    ctrlSocket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId ());
    ctrlSocket->SetRecvCallback(MakeCallback(&GridftpClientApp::handleCtrlRecv, this));
    ctrlSocket->Bind();
    ctrlSocket->Connect(InetSocketAddress(GridftpClientApp::serverAddr, ctrlPort));    
    //ctrlSocket->SetCloseCallbacks(MakeCallback(&GridftpClientApp::handlePeerClose, this), MakeCallback(&GridftpClientApp::handlePeerError, this));
    // set up data channel TCP connection
    for(int i=0; i<nTCPStream; i++){
        NS_LOG_DEBUG( Simulator::Now ().GetSeconds () << " GridFTP Client for [" << srcName << " => " << dstName << "], " << id << 
        " try to connect to " << serverAddr << ":" << dataPort << std::endl);
        Ptr<Socket> socket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId ());
        socket->SetRecvCallback(MakeCallback(&GridftpClientApp::handleDataRecv, this));
        socket->Bind();
        socket->Connect(InetSocketAddress(GridftpClientApp::serverAddr, dataPort));         
        workerInfo worker{socket, 0, 0};
        workerDataRecv[socket] = worker;
    }
    Simulator::Schedule (Seconds(.5) , &GridftpClientApp::periodic1Sec, this, 0);
}

uint64_t GridftpClientApp::sumRecvBytes(){
    uint64_t ret = 0;
    for (auto it=workerDataRecv.begin(); it!=workerDataRecv.end(); it++){
        ret += it -> second.recv_byte;
    }
    return ret;
}

void GridftpClientApp::clearRecvBuffer(){
    for (auto it=workerDataRecv.begin(); it!=workerDataRecv.end(); it++){
        it -> second.recv_byte = 0;
        it -> second.state = 0;
    }
}


void GridftpClientApp::periodic1Sec(uint64_t last_nbytes){
    NS_LOG_FUNCTION (this << last_nbytes);
    uint64_t sum_now = sumRecvBytes();
    uint64_t diff    = sum_now - last_nbytes;
    NS_LOG_INFO( Simulator::Now ().GetSeconds () << " GridFTP Client for [" << srcName << " => " << dstName << "], " << 
    id << ": transfer rate: " << 8.0 * diff / (1<<30) << " Gbps" << std::endl);
    Simulator::Schedule (Seconds(1) , &GridftpClientApp::periodic1Sec, this, sum_now);
}

/*
 ***************************************************************************************************
 * func   name: StopApplication
 * description: The StopApplication method is called at the stop time specified by Stop
 * parameters :
 *             none
 * return: none
 ***************************************************************************************************
 */
void GridftpClientApp::StopApplication(){
    NS_LOG_DEBUG(Simulator::Now ().GetSeconds () << " GridFTP Client for [" << srcName << " => " << dstName << "], " << id 
    << " stopped, data channel received " << (double)sumRecvBytes()/(1<<30) << " GiB "
    << 8.0*sumRecvBytes() / Simulator::Now().GetSeconds() / (1<<30) << "Gbps" << std::endl);  
    //NS_LOG_UNCOND ("DTN: " << id << " stopped");
}