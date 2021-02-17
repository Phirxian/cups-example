#include <iostream>
#include <thread>

#include "sync/process.h"
#include "network/socket.h"
#include "network/util.h"
#include "network/poller.h"

using namespace std;
using namespace netsys::PollerShortNames;

namespace syncsys
{
    void* process_run(void *i)
    {
        auto *tmp = static_cast<syncsys::process*>(i);
        tmp->run();
        return 0;
    }
}

int main(int argc, char *argv[])
{
    if(argc < 1)
        abort();

    if(argc != 4)
    {
        cerr << "Usage: " << argv[ 0 ] << " HOST PORT filepath" << endl;
        return EXIT_FAILURE;
    }

    string host{argv[1]}, port{argv[2]};

    cerr << "Looking up " << host << ":" << port << endl;
    netsys::Address server( host, port );
    cerr << "Done. Found " << server.to_string() << endl;

    netsys::TCPSocket socket;
    cerr << "Connecting...";
    socket.connect(server);
    cerr << "done." << endl;

    netsys::Poller poller;

    poller.add_action(
        Action(
            socket, Direction::In, [&]() {
                //! print received file
                cout << socket.read();
                if(socket.eof())
                     return ResultType::Exit;
                else return ResultType::Continue;
            }
        )
    );
    
    //! send filename
    socket.write(argv[3]);

    cerr << "..................."<< endl;
    while(true)
    {
        const auto ret = poller.poll(-1);
        if(ret.result == PollResult::Exit)
        {
            cerr << "..................."<< endl;
            return ret.exit_status;
        }
    }
    
    cout << endl;
    return EXIT_SUCCESS;
}
