#include "db/rest/server.hpp"

using namespace std;
using namespace net;

int main(int, char**)
try
{
    slog.tie(&clog);
    slog.tag("YARESTDB");
    slog.level(syslog::severity::debug);
    slog << notice << "Initializing server" << flush;
    auto engine = db::engine{};
    auto server = db::rest::server{engine};
    slog << notice << "Initialized server" << flush;
    server.start("2112");
    return 0;
}
catch(...)
{
    slog << error << "Shit hitted the fan!" << flush;
    return 1;
}