#ifndef __ESNET_H__
#define __ESNET_H__
#include <ns3/simulator.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/core-module.h>
#include <ns3/applications-module.h>
#include <ns3/config-store.h>
#include <ns3/trace-helper.h>
#include <ns3/global-route-manager.h>
#include <ns3/ipv4-nix-vector-helper.h>

#include <map>
#include <string>
#include <vector>
#include <set>
using namespace ns3;
using namespace std;

typedef struct
{
    string end1, end2, rate, delay;
}linkInfo;

map< string, Ptr<Node> > esnetRouter;

// std::vector<linkInfo> esnetLink = {
//     {"ANL", "STAR", "100Gbps", "1ms"}, {"STAR", "CHIC", "100Gbps", "1.5ms"},
//     {"CHIC", "KANS", "100Gbps", "2ms"},{"KANS", "DENV", "100Gbps", "2ms"},
//     {"DENV", "SACR", "100Gbps", "3ms"},{"SACR", "SUNN", "100Gbps", "1.5ms"},
//     {"SUNN", "LBL", "100Gbps", "1ms"}
// };
std::vector<linkInfo> esnetLink = {
    {"LIGO", "PNWG", "10Gbps", "1ms"}, {"PNWG", "PNNL", "10Gbps", "1ms"},
    {"PNWG", "SACR", "100Gbps", "1ms"}, {"JGI", "SACR", "10Gbps", "1ms"},
    {"SNLL", "SACR", "10Gbps", "2ms"}, {"SUNN", "SACR", "100Gbps", "2ms"},
    {"SUNN", "LBL", "100Gbps", "800us"}, {"LBL", "NPS", "1Gbps", "800us"},
    {"LBL", "MSRI", "1Gbps", "1ms"}, {"SUNN", "NERSC", "100Gbps", "1ms"},
    {"SUNN", "SLAC", "100Gbps", "1ms"}, {"SUNN", "LLNL", "100Gbps", "1ms"},
    {"SUNN", "SAND", "10Gbps", "1ms"}, {"SAND", "GA", "10Gbps", "1ms"},
    {"SUNN", "ELPA", "100Gbps", "1.5ms"}, {"ELPA", "ALBQ", "100Gbps", "1.5ms"},
    {"ALBQ", "LANL", "100Gbps", "1ms"}, {"ALBQ", "KCP-ALBQ", "10Gbps", "1ms"},
    {"ALBQ", "SNLA", "10Gbps", "1.5ms"}, {"ALBQ", "DENV", "100Gbps", "1.5ms"},
    {"DENV", "LSVN", "10Gbps", "1ms"}, {"LSVN", "NSO", "10Gbps", "1ms"},
    {"LSVN", "IARC", "1Gbps", "3ms"}, {"DENV", "SACR", "100Gbps", "3ms"},
    {"DENV", "BOIS", "10Gbps", "1ms"}, {"BOIS", "INL", "10Gbps", "1ms"},
    {"BOIS", "PNWG", "10Gbps", "0.5ms"}, {"DENV", "PNWG", "100Gbps", "0.5ms"},
    {"DENV", "NREL", "10Gbps", "0.5ms"}, {"DENV", "NGA-SW", "10Gbps", "0.5ms"},
    {"DENV", "PANTEX", "1Gbps", "2ms"}, {"DENV", "KANS", "100Gbps", "2ms"},
    {"KANS", "KCP", "10Gbps", "2ms"}, {"KANS", "CHIC", "100Gbps", "2ms"},
    {"KANS", "HOUS", "100Gbps", "2ms"}, {"HOUS", "ELPA", "100Gbps", "2ms"},
    {"HOUS", "NASH", "100Gbps", "2ms"}, {"CHIC", "NASH", "100Gbps", "2ms"},
    {"CHIC", "STAR", "100Gbps", "1.5ms"}, {"STAR", "AMES", "100Gbps", "1.5ms"},
    {"STAR", "ANL", "100Gbps", "1ms"}, {"STAR", "FNAL", "100Gbps", "1ms"},
    {"STAR", "BOST", "100Gbps", "3ms"}, {"STAR", "AOFA", "100Gbps", "3ms"},
    {"NASH", "ATLA", "100Gbps", "1ms"}, {"ATLA", "SRS", "1Gbps", "1ms"},
    {"ATLA", "SRS-EM", "1Gbps", "1.5ms"}, {"ATLA", "ORNL", "100Gbps", "1.5ms"},
    {"ORNL", "Y12", "10Gbps", "1ms"}, {"ORNL", "OSTI", "10Gbps", "1ms"},
    {"ORNL", "ORAU", "10Gbps", "1ms"}, {"ORNL", "ETTP", "10Gbps", "1ms"},
    {"ATLA", "WASH", "100Gbps", "4ms"}, {"WASH", "CHIC", "100Gbps", "4ms"},
    {"WASH", "JLAB", "10Gbps", "1ms"}, {"WASH", "PPPL", "10Gbps", "1ms"},
    {"WASH", "AOFA", "100Gbps", "7ms"}, {"AOFA", "LOND", "100Gbps", "7ms"},
    {"WASH", "DOE-GTN", "1Gbps", "1ms"}, {"WASH", "DOE-FORRESTAL", "1Gbps", "1ms"},
    {"WASH", "DOE-NNSA", "1Gbps", "1.5ms"}, {"AOFA", "NEWY", "100Gbps", "1.5ms"},
    {"NEWY", "BOST", "100Gbps", "1ms"}, {"BOST", "PSFC", "10Gbps", "1ms"},
    {"BOST", "AMST", "10Gbps", "1ms"}, {"BOST", "LNS", "10Gbps", "1ms"},
    {"NEWY", "BNL", "100Gbps", "7ms"}, {"NEWY", "LOND", "100Gbps", "7ms"},
    {"LOND", "AMST", "100Gbps", "1.5ms"}, {"AMST", "CERN-272", "100Gbps", "1.5ms"},
    {"CERN-272", "CERN", "100Gbps", "1.5ms"}, {"CERN", "CERN-513", "100Gbps", "1.5ms"},
    {"CERN-272", "CERN-513", "100Gbps", "10ms"}, {"CERN-513", "WASH", "100Gbps", "10ms"},
    {"CERN-513", "LOND", "100Gbps", "1.5ms"}
};
#endif