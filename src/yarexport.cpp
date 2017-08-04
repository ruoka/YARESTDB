
#include <iostream>
#include <fstream>
#include "gsl/span.hpp"
#include "db/metadata.hpp"
#include "xson/json.hpp"

using namespace std;
using namespace string_literals;
using namespace gsl;
using namespace xson;

const auto usage = R"(yarexport [--help] [--file=<name>])";

int main(int argc, char** argv)
try
{
    const auto arguments = make_span(argv,argc).subspan(1);
    auto file = "yar.db"s;

    for(const string_view option : arguments)
    {
        if(option.find("--file") == 0)
        {
            file = option.substr(option.find('=')+1);
        }
        else if(option.find("--help") == 0)
        {
            clog << usage << endl;
            return 0;
        }
        else if(option.find("-") == 0)
        {
            clog << usage << endl;
            return 1;
        }
    }

    auto storage = ifstream{file};

    if(!storage.is_open())
        throw runtime_error{"file "s + file + " not found"s};

    while(storage)
    {
        auto metadata = db::metadata{};
        auto document = db::object{};
        storage >> metadata >> document;
        if(storage)
            clog << json::stringify(
                            {{"collection"s, metadata.collection         },
                             {"status"s,     to_string(metadata.status)  },
                             {"position"s,   metadata.position           },
                             {"previous"s,   metadata.previous           },
                             {"document"s,   document                    }})
                      << ",\n";
    }
}
catch(const system_error& e)
{
    cerr << "System error with code " << e.code() << " " << e.what() << endl;
    return 1;
}
catch(const exception& e)
{
    cerr << "Exception " << e.what() << endl;
    return 1;
}
catch(...)
{
    cerr << "Shit hit the fan!" << endl;
    return 1;
}
