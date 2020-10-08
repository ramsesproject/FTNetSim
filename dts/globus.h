#ifndef __GLOBUS_H__
#define __GLOBUS_H__
#include <queue>
#include <string>
#include <vector>

#include <ns3/simulator.h>
#include <ns3/ipv4-nix-vector-helper.h>

#include "gridftp-server.h"
#include "gridftp-client.h"

using namespace ns3;
using namespace std;

class globusTransfer : public Object
{
public:
    globusTransfer(const char * src, const char * dst, uint32_t cc, uint32_t para, uint32_t id);
    virtual ~globusTransfer(void);
    string srcName, dstName;                     // source/destination name, must use esnet site name, i.e., name esnet end routers
    uint32_t nCC, nPara;                         // concurrency, parallism
    uint32_t instID;                             // an unique id for globusTransfer
    vector<uint64_t> fileSize;                   // list of file size to be sent
    Ptr<Node> srcRouter, dstRouter;              // 
    NodeContainer srcDTN, dstDTN;                // server will always be installed on srcDTN(s)

    uint32_t nodeInit(uint32_t sys_id);
    void buildLANTop();
    void installAPP(float ts, float te);
};

extern map< string, Ptr<Node> > esnetRouter;
extern void getIPBase(char *out);

#endif