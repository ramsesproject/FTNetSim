#ifndef __ESNET_H__
#define __ESNET_H__
#include <string>
#include <iostream>
using namespace std;

typedef union {
    unsigned char u8[4];
    unsigned int u32;
    }ipv4;

ipv4 initAddr = {.u8={0, 1, 0, 20}};

#endif