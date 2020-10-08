#ifndef __FTNETSIM_H__
#define __FTNETSIM_H__
#include <ns3/nstime.h>
#include <ns3/simulator.h>
#include <ns3/network-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/core-module.h>
#include <ns3/applications-module.h>
#include <ns3/bridge-module.h>
#include <ns3/config-store.h>
#include <ns3/traffic-control-helper.h>
#include <ns3/trace-helper.h>
#include <ns3/global-route-manager.h>
#include <ns3/ipv4-nix-vector-helper.h>
#include <ns3/v4ping-helper.h>

#include "dts/gridftp-server.h"
#include "dts/gridftp-client.h"
#include "dts/globus.h"

using namespace ns3;
using namespace std;

uint32_t simSeconds = 5;

extern map< string, Ptr<Node> > esnetRouter;

extern uint32_t createRouters(uint32_t sys_idb);

#endif