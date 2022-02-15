#include <iostream>
#include <cstdlib>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <vector>
#include <boost/process.hpp>
#include "Adapters.h"
#include "NameSolver.h"
#pragma comment(lib, "iphlpapi.lib")
using namespace std;

bool getAdp(vector<Adapters>&);
bool init(vector<Adapters>&);
void menu(vector<Adapters>);
const string routeLlist[] = {
    "api-umamusume.cygames.jp", //umamusume
    "prd-storage-umamusume.akamaized.net",
    "prd-storage-app-umamusume.akamaized.net",
    "prd-storage-game-umamusume.akamaized.net",
    "apidgp-gameplayer.games.dmm.com", //dmm
    "api-priconne-redive.cygames.jp" //priconne
};

int main()
{
    vector<Adapters> adaptersList;

    try
    {
        if(init(adaptersList))
        {
            cerr << "程式初始化失敗\n";
            ::system("pause");
            return 1;
        }

        menu(adaptersList);
        cout << "路由設定完畢\n";
        ::system("pause");
        return 0;
    }
    catch(std::exception e)
    {
        cerr << "程式發生錯誤: " << e.what() << '\n';
        ::system("pause");
        return 1;
    }
}

bool init(vector<Adapters>& adaptersList)
{
    ::system("title Set Route ver.0.9.62957.3");
    boost::process::system("chcp 950", boost::process::shell/*, boost::process::std_out > boost::process::null*/);
    ::system("cls");
    cout << "這是為了讓VPN只經過DMM遊戲的路由設定程式\n";
    cout << "目前本程式支援以下程式及遊戲:\n";
    cout << "    1.DMM Game Player/DMM Game Player Beta\n";
    cout << "    2.公主連結\n";
    cout << "    3.馬娘\n";
    cout << "若有任何問題，或希望新增支援項目，請以email聯絡我。\n";
    cout << "Email: dev@danielwu.tw\n";
    ::system("pause");
    ::system("cls");
    return getAdp(adaptersList);
}

void menu(vector<Adapters> adaptersList)
{
    UINT internet = 1, vpn = 1;
    Adapters internetAdp, vpnAdp;
    while(true)
    {
        int count = 1;
        for(auto& i : adaptersList)
        {
            cout << "#" << count++ << '\n';
            i.printInfo();
            cout << '\n';
        }
        cout << "選擇網際網路網卡: ";
        cin >> internet;
        cout << "選擇VPN網卡: ";
        cin >> vpn;
        if(internet != vpn && internet <= adaptersList.size() && vpn <= adaptersList.size() && internet > 0 && vpn > 0)
            break;
        ::system("cls");
        cout << "輸入錯誤\n";
    }
    internetAdp = adaptersList.at(internet - 1);
    vpnAdp = adaptersList.at(vpn - 1);
    internetAdp.setRoute("0.0.0.0", ROUTE_MODE_CHANGE, "0.0.0.0");
    vpnAdp.setRoute("128.0.0.0", ROUTE_MODE_DELETE, "128.0.0.0");
    vpnAdp.setRoute("0.0.0.0", ROUTE_MODE_DELETE, "128.0.0.0");
    for(auto& i : routeLlist)
        vpnAdp.setRoute(get_ipv4(i), ROUTE_MODE_ADD);
}

bool getAdp(vector<Adapters>& adaptersList)
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = new IP_ADAPTER_INFO;

    if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        delete pAdapterInfo;
        pAdapterInfo = new IP_ADAPTER_INFO[ulOutBufLen];
        if(pAdapterInfo == NULL)
        {
            cerr << "Error allocating memory needed to call GetAdaptersinfo\n";
            return 1;
        }
    }

    if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while(pAdapter)
        {
            adaptersList.push_back(Adapters(pAdapter));
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        cerr << "取得介面卡資訊發生錯誤: " << dwRetVal << '\n';
        return 1;
    }
    return 0;
}
