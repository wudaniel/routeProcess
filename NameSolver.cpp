#include "NameSolver.h"

string get_ipv4(string host)
{
    string port_num = "80";
    asio::io_service ios(16843009);
    asio::ip::tcp::resolver::query resolver_query(host, port_num, asio::ip::tcp::resolver::query::numeric_service);
    asio::ip::tcp::resolver resolver(ios);
    asio::ip::tcp::resolver::iterator end;
    asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
    while(!it->endpoint().address().is_v4())
        it++;
    return it->endpoint().address().to_string();
}
