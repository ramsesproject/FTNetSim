#ifndef __GRIDFTP_SERVER_H__
#define __GRIDFTP_SERVER_H__
#include <queue>
#include <tuple>
#include <ns3/simulator.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/core-module.h>
#include <ns3/csma-module.h>
#include <ns3/applications-module.h>

#include "globus.h"

using namespace ns3;
using namespace std;
typedef struct
{
    u_int64_t nBytes2Send;
    unsigned char *recvBuf;
    u_int32_t recvBufLen;                        // validate bytes in recv buffer
    Ptr<Socket> socket;                          // consider as unique ID for a client
    Address addr;
    bool isCtrl;                                 // client/connection is a ctrl channel
}gridftpClientInfo;

class GridftpServerApp : public Application
{
public:
    GridftpServerApp(u_int32_t id);
    GridftpServerApp(Ptr< Node > node);
    virtual ~GridftpServerApp(void);
    string srcName, dstName;
private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
    u_int32_t id;
    Ptr<Socket> listenCtrlSocket, listenDataSocket;      // socket for control channel 
    u_int32_t ctrlPort, dataPort;
    std::map<Ptr<Socket>, gridftpClientInfo> clientsTable;
protected:
    void initialize();
    void handleCtrlReq(Ptr<Packet> msg, Ptr<Socket> socket);

    void sendCallbackCtrl(Ptr<Socket> socket, uint32_t nbytes);
    void sendCallbackData(Ptr<Socket> socket, uint32_t nbytes);

    void handleRecvData(Ptr<Socket> socket);
    void handleRecvCtrl(Ptr<Socket> socket);

    void responseCallbackCtrl(Ptr<Packet> msg, Ptr<Socket> socket);

    void handlePeerClose(Ptr<Socket>);
    void handlePeerError(Ptr<Socket>);

    void handleAcceptData(Ptr<Socket> socket, const Address& from);
    void handleAcceptCtrl(Ptr<Socket> socket, const Address& from);
    Time serviceTime();
    uint64_t nInService;
    uint64_t maxConcurrence;     // maximum number of request allowed in processing
    std::queue< std::tuple<Ptr<Packet>, Ptr<Socket> > > reqFifo;
};

#endif