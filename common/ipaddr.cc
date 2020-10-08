#include "ipaddr.h"
#include <stdio.h>
#include <string.h>

void ipIncre()
{
    initAddr.u32 += 256;
}

void getIPBase(char *out)
{
    string ret = to_string(initAddr.u8[3]) + "." + to_string(initAddr.u8[2]) + "." + to_string(initAddr.u8[1]) + "." + to_string(initAddr.u8[0]);
    strcpy(out, ret.c_str());
    ipIncre();
}
