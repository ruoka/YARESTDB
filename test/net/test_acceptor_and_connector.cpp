#include <array>
#include <future>
#include <gtest/gtest.h>
#include "net/acceptor.hpp"
#include "net/connector.hpp"

using namespace std;
using namespace net;

class AcceptorAndConnectorTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        int s{0};
        for(auto& i : test)
            i = ++s;
    }

    array<int,100> test;

    mutex m;
};

TEST_F(AcceptorAndConnectorTest,runTwoThreads)
{
    auto f1 = async(
        launch::async,
        [&]{
            acceptor ator{"localhost", "54321"};
            {
                unique_lock<mutex> l{m};
                clog << "Accepting...  " << ator.host() << '.' << ator.service() << endl;
            }
            auto os = ator.accept();
            for(auto i : test)
            {
                os << i << endl;
                unique_lock<mutex> l{m};
                //clog << "Acceptor:  " << i << endl;
            }
        });

    auto f2 = async(
        launch::async,
        [&]{
            connector ctor{"localhost", "54321"};
            {
                unique_lock<mutex> l{m};
                clog << "Connecting... " << ctor.host() << '.' << ctor.service() << endl;
            }
            auto is = ctor.connect();
            for(auto i : test)
            {
                int ii;
                is >> ii;
                ASSERT_EQ(i,ii);
                unique_lock<mutex> l{m};
                //clog << "Connector: " << ii << endl;
            }
        });

    f1.get();
    f2.get();
}
