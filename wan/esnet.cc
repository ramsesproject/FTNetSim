#include "esnet.h"

/*
 ***************************************************************************************************
 * func   name: createRouters
 * description: create esnet topology
 * parameters :
 *             sys_idb: starting system id for distributed simulation
 * return: the next avaliable system id
 ***************************************************************************************************
 */
uint32_t createRouters(uint32_t sys_idb){
    uint32_t sysid = sys_idb;
    set<string> router_name;

    // get unique router name from link information 
    for(int i=0; i<esnetLink.size(); i++){
        router_name.insert(esnetLink[i].end1);
        router_name.insert(esnetLink[i].end2);
    }
    cout << "There are " << router_name.size() << " routers" << endl;

    // create router node
    InternetStackHelper ns3IpStack;
    Ipv4NixVectorHelper nixRouting;
    ns3IpStack.SetRoutingHelper(nixRouting); 
    for(auto it=router_name.begin(); it!=router_name.end(); it++){
        esnetRouter[*it] = CreateObject<Node> (sysid++);
        ns3IpStack.Install (esnetRouter[*it]);
    }
    
    // build link according to link information
    uint8_t ip_icr = 1;
    for(auto it=esnetLink.begin(); it!=esnetLink.end(); it++){
        PointToPointHelper p2p;
        p2p.SetDeviceAttribute("DataRate", StringValue ((*it).rate));
        p2p.SetChannelAttribute("Delay", StringValue((*it).delay));
        NetDeviceContainer link = p2p.Install(esnetRouter[(*it).end1], esnetRouter[(*it).end2]);

        Ipv4AddressHelper ipv4;
        string ipbase = "192.1." + std::to_string(ip_icr++) + ".0";
        ipv4.SetBase (ipbase.c_str(), "255.255.255.0");
        // string ipbase = "0.0.0." + std::to_string(ip_icr+=2);
        // ipv4.SetBase ("10.1.2.0", "255.255.255.0", ipbase.c_str());
        // cout << (*it).end1 << " and " << (*it).end2 << " have ip from " << ipbase << endl;
        ipv4.Assign(link);
    }
    return sysid;
}
