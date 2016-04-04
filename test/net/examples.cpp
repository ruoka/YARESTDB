#include <iostream>
#include <gtest/gtest.h>
#include "net/connector.hpp"
#include "net/acceptor.hpp"
#include "net/syslogstream.hpp"

using namespace std;
using namespace net;

TEST(ExampleHttpRequest,CommandLine)
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
        auto echo = ""s;
        getline(s, echo);
        s << echo << endl;
        clog << echo << endl;
    }
}
catch(const exception& e)
{
    cerr << "Exception: " << e.what() << endl;
}

TEST(ExampleSyslog,CommandLine)
{
    slog.level(syslog::severity::info);
    slog.facility(syslog::facility::local0);
    slog.tag("example");

    auto clothes = "shirts"s; auto spouse = "wife"; auto wrong = false;

    slog << debug   << "... " << 3 << ' ' << 2 << ' ' << 1 << " Liftoff" << flush;

    slog << info    << "The papers want to know whose " << clothes << " you wear..." << flush;

    slog << notice  << "Tell my " << spouse << " I love her very much!" << flush;

    slog << warning << "Ground Control to Major Tom Your circuit's dead, there's something " << boolalpha << wrong << '?' << flush;

    slog << error   << "Planet Earth is blue and there's nothing I can do." << flush;
}
