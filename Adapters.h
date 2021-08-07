#pragma once
#include <String>
#include <iostream>
#include <minwindef.h>
#include <IPTypes.h>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <boost/format.hpp>

#define ROUTE_MODE_ADD 0
#define ROUTE_MODE_CHANGE 1
#define ROUTE_MODE_DELETE 2

class Adapters
{
public:
    Adapters();
    Adapters(PIP_ADAPTER_INFO);
    void printInfo();
    bool setRoute(std::string, int, std::string = "255.255.255.255");

private:
    std::string name;
    std::string ip;
    std::string gateway;
    std::string mac;
    std::string dhcp;
    UINT type;
};
