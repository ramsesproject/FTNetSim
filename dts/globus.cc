#include "globus.h"

globusTransfer::globusTransfer(const char *src, const char *dst, uint32_t cc, uint32_t para, uint32_t id){
    nCC     = cc;
    nPara   = para;
    srcName = src;
    dstName = dst;
    instID = id;
}

globusTransfer::~globusTransfer(){
    
}

uint32_t globusTransfer::nodeInit(uint32_t sys_id){
    for(int i=0; i<nCC; i++){
        srcDTN.Add(CreateObject<Node>(sys_id++));
    }

    srcRouter = CreateObject<Node> (sys_id++);
    dstRouter = CreateObject<Node> (sys_id++);

    for(int i=0; i<nCC; i++){
        dstDTN.Add(CreateObject<Node>(sys_id++));
    }
    return sys_id;
}

void globusTransfer::buildLANTop(){
    PointToPointHelper p2pLANsrc, esnetP2P, p2pLANdst;
    p2pLANsrc.SetDeviceAttribute("DataRate", StringValue ("10Gbps"));
    p2pLANsrc.SetChannelAttribute("Delay", StringValue("200us"));

    p2pLANdst.SetDeviceAttribute("DataRate", StringValue ("10Gbps"));
    p2pLANdst.SetChannelAttribute("Delay", StringValue("200us"));

    esnetP2P.SetDeviceAttribute ("DataRate", StringValue ("10Gbps"));
    esnetP2P.SetChannelAttribute("Delay",    StringValue ("3ms"));

    // net devices installed on dtn/router
    NetDeviceContainer srcDTNNetDC, dstDTNNetDC, routerNetDC;
    // connect ANL DTNs to its switch(router)
    for(int i = 0; i < nCC; i++){
        NetDeviceContainer link = p2pLANsrc.Install(srcDTN.Get(i), srcRouter);
        srcDTNNetDC.Add(link);
    }

    // net devices installed on esnet routers, connect esnet routers
    routerNetDC.Add(esnetP2P.Install(srcRouter, esnetRouter[srcName]) );
    routerNetDC.Add(esnetP2P.Install(esnetRouter[dstName], dstRouter) );

    // net devices installed on lbl dtn/switch, connect LBL DTNs to its switch
    for(int i = 0; i < nCC; i++){
        NetDeviceContainer link = p2pLANdst.Install(dstDTN.Get(i), dstRouter);
        dstDTNNetDC.Add(link);
    }
    // Install the L3 internet stack (TCP/IP)
    InternetStackHelper ns3IpStack;
    Ipv4NixVectorHelper nixRouting;
    ns3IpStack.SetRoutingHelper(nixRouting);
    ns3IpStack.Install (srcDTN);
    ns3IpStack.Install (dstDTN);
    ns3IpStack.Install (srcRouter);
    ns3IpStack.Install (dstRouter);

    Ipv4AddressHelper ipv4;
    char ipbase[15];

    getIPBase(ipbase);
    ipv4.SetBase (ipbase, "255.255.255.0");
    ipv4.Assign  (srcDTNNetDC);

    getIPBase(ipbase);
    ipv4.SetBase (ipbase, "255.255.255.0");
    ipv4.Assign  (dstDTNNetDC);

    getIPBase(ipbase);
    ipv4.SetBase (ipbase, "255.255.255.0", "0.0.0.200");
    ipv4.Assign  (routerNetDC);
}

void globusTransfer::installAPP(float ts, float te){
    for (int i=0; i<nCC; i++){
        Ptr<GridftpServerApp> app = CreateObject<GridftpServerApp>(i);
        app -> SetStartTime(Seconds(ts));
        app -> SetStopTime(Seconds(te));
        app -> srcName = srcName;
        app -> dstName = dstName;
        srcDTN.Get(i) -> AddApplication(app);
    }

    for (int i=0; i<nCC; i++){
        stringstream ss;
        ss << srcDTN.Get(i)->GetObject<Ipv4>() -> GetAddress (1, 0).GetLocal();
        Ptr<GridftpClientApp> app = CreateObject<GridftpClientApp>(2021, 50000, ss.str().c_str(), i, nPara);
        app -> SetStartTime(Seconds(ts) + NanoSeconds(1));
        app -> SetStopTime(Seconds(te));
        app -> srcName = srcName;
        app -> dstName = dstName;
        dstDTN.Get(i) -> AddApplication(app);
    } 
}