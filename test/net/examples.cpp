#include <iostream>
#include <gtest/gtest.h>
#include "net/connector.hpp"
#include "net/acceptor.hpp"

using namespace std;
using namespace net;

TEST(NetExampleHttpRequest,CommandLine)
try
{
    auto s = connect("www.google.com","http");

    s << "GET / HTTP/1.1\r\n"
      << "Host: www.google.com\r\n"
      << "Connection: close\r\n"
      << "Accept: text/plain, text/html\r\n"
      << "Accept-Charset: utf-8\r\n"
      << "\r\n"
      << flush;

    while(s)
    {
        auto c = ' ';
        s >> noskipws >> c;
        clog << c;
    }
    clog << flush;
}
catch(const exception& e)
{
    cerr << "Exception: " << e.what() << endl;
}

TEST(ExampleEchoServer,CommandLine)
try
{
    auto ator = acceptor{"::1", "2112"}; // IPv6 localhost
    auto s = ator.accept();

    while(s)
    {
        auto echo = string{};
        getline(s, echo);
        s << echo << endl;
        clog << echo << endl;
    }
}
catch(const exception& e)
{
    cerr << "Exception: " << e.what() << endl;
}
