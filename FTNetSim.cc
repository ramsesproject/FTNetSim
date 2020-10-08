#include "FTNetSim.h"

int main (int argc, char *argv[]){

    uint32_t sysID, instIDSeq = 0;
    Time::SetResolution (Time::NS);
    Config::SetDefault("ns3::TcpSocket::SndBufSize", UintegerValue(67108864));
    Config::SetDefault("ns3::TcpSocket::RcvBufSize", UintegerValue(67108864));
    Config::SetDefault("ns3::CsmaNetDevice::Mtu", UintegerValue(9000));

    Config::SetDefault("ns3::PointToPointNetDevice::Mtu", UintegerValue(9000));
    Config::SetDefault("ns3::BridgeNetDevice::Mtu", UintegerValue(9000));

    Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHtcp::GetTypeId ()));
    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(8765));
    Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(10));
    // Config::SetDefault("ns3::QueueBase::MaxSize", StringValue ("4p"));

    GlobalValue::Bind("ChecksumEnabled", BooleanValue (true));

    sysID = createRouters(0);

    Ptr<globusTransfer> anl2lbl = CreateObject<globusTransfer>("ANL", "LBL", 1, 1, instIDSeq++);
    sysID = anl2lbl->nodeInit(sysID);
    anl2lbl->buildLANTop();
    anl2lbl->installAPP(.05, simSeconds);

    Ptr<globusTransfer> anl2bnl = CreateObject<globusTransfer>("ANL", "BNL", 1, 1, instIDSeq++);
    sysID = anl2bnl->nodeInit(sysID);
    anl2bnl->buildLANTop();
    anl2bnl->installAPP(.05, simSeconds);

    Ptr<globusTransfer> bnl2lbl = CreateObject<globusTransfer>("LIGO", "CERN-513", 1, 1, instIDSeq++);
    sysID = bnl2lbl->nodeInit(sysID);
    bnl2lbl->buildLANTop();
    bnl2lbl->installAPP(.05, simSeconds);

    cout << "There are " << sysID << " nodes in this scenario" << endl;
    
    Simulator::Stop(Seconds (simSeconds+1));

    // Simulator::EnableParallelSimulation();

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
