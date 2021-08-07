#include "Adapters.h"
Adapters::Adapters()
{
    this->dhcp = "";
    this->gateway = "";
    this->ip = "";
    this->mac = "";
    this->name = "";
    this->type = 0;
}

Adapters::Adapters(PIP_ADAPTER_INFO info)
{
    this->ip = info->IpAddressList.IpAddress.String;
    this->gateway = info->GatewayList.IpAddress.String;
    this->name = info->Description;
    this->dhcp = info->DhcpEnabled ? info->DhcpServer.IpAddress.String : "";
    if(this->gateway == "0.0.0.0")
        this->gateway = this->dhcp;
    if(this->gateway == "")
        this->gateway = this->ip;
    std::stringstream stream;
    for(UINT i = 0; i < info->AddressLength; i++)
    {
        stream << std::hex << (int)info->Address[i] << "-";
    }
    this->mac = stream.str();
    if(!this->mac.empty())
        this->mac.pop_back();
    boost::to_upper(this->mac);
    this->type = info->Type;
}

void Adapters::printInfo()
{
    std::cout << "  名稱: " << this->name << "\n";
    std::cout << "  類型: ";
    switch(this->type)
    {
        case 53:
            std::cout << "虛擬網卡\n";
            break;
        case 6:
            std::cout << "乙太網卡\n";
            break;
        case 71:
            std::cout << "Wifi\n";
            break;
        default:
            std::cout << "未知類型" << this->type << "\n";
            break;
    }
    std::cout << "  IP: " << this->ip << "\n";
    std::cout << "  Gate: " << this->gateway << "\n";
}

bool Adapters::setRoute(std::string IP, int mode, std::string mask)
{
    namespace bp = boost::process;
    std::string routeMode[] = {
        "add",
        "change",
        "delete"
    };

    bp::ipstream ips;
    boost::format base("route %s %s mask %s %s");
    std::cout << base % routeMode[mode] % IP % mask % (mode == 2 ? "" : this->gateway) << '\n';
    bp::child com((base % routeMode[mode] % IP % mask % (mode == 2 ? "" : this->gateway)).str(), bp::shell, bp::std_out > ips);
    com.wait();

    std::string res, tmp;
    while(std::getline(ips, tmp))
        res += tmp + '\n';
    std::cout << res;
    if(com.exit_code())
    {
        std::cerr << "Error: " << res << '\n';
        return false;
    }
    return true;
}