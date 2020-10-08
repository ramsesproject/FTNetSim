#ifndef __GRIDFTP_CLIENT_H__
#define __GRIDFTP_CLIENT_H__
#include <vector>
#include <string>
#include <ns3/nstime.h>
#include <ns3/simulator.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/applications-module.h>

using namespace ns3;
using namespace std;

// organize 'thread' that responses to data channel with server 
typedef struct {
    Ptr<Socket> sockets;                         
    uint8_t state;                               // 0->idle; 1->busy
    uint64_t recv_byte;                          // bytes received in the buffer
}workerInfo;

class GridftpClientApp : public Application
{
public:
    GridftpClientApp(uint16_t ctrl_port, uint16_t data_port, std::string server_ip, u_int32_t id, u_int32_t n_tcp);
    GridftpClientApp(u_int32_t id);
    //virtual ~GridftpClientApp(void);
    void handleCtrlRecv(Ptr<Socket> socket);
    void handleDataRecv(Ptr<Socket> socket);
    std::vector< Ptr<Socket> > dataChlSocket;
    Ptr<Socket> ctrlSocket;
    uint32_t parallelism;              // parallel data streaming
    uint32_t nPacket;                  // number of packets to be done for the current file
    Ipv4Address serverAddr; 
    string srcName, dstName;
private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
    void periodic1Sec(uint64_t last_nbytes);
    uint64_t sumRecvBytes();
    void clearRecvBuffer();
    u_int32_t id;
    uint16_t dataPort;
    uint16_t ctrlPort;
    uint16_t nTCPStream;
    map< Ptr<Socket>, workerInfo > workerDataRecv;
};

#endif