/* simple TCP listener/server to demonstrate sourdough starter classes */
/* Keith Winstein <keithw@cs.stanford.edu>, January 2015 */

#include <thread>
#include <iostream>
#include <fstream>

#include "sync/process.h"
#include "network/socket.h"
#include "network/util.h"

using namespace std;

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

    if(argc != 2)
    {
        cerr << "Usage: " << argv[ 0 ] << " PORT" << endl;
        return EXIT_FAILURE;
    }

    netsys::TCPSocket listening_socket;
    
    listening_socket.set_reuseaddr();
    listening_socket.bind(netsys::Address("::0", argv[1]));
    listening_socket.listen();
    
    cerr << "Listening on local address: "
         << listening_socket.local_address().to_string()
         << endl;

    while(true)
    {
        thread client_handler(
            [](netsys::TCPSocket client)
            {
                cerr << "New connection from "
                     << client.peer_address().to_string()
                     << endl;
                     
                // get filename
                     
                std::string buff;
                const string chunk = client.read();
                ifstream stream(chunk, std::ios::binary);
                
                if(stream.is_open())
                {
                    cerr << "sending " << chunk << endl; 
                    while(stream.good())
                    {
                        getline(stream,buff);
                        buff += "\n";
                        client.write(buff, true);
                    }
                    stream.close();
                }
                else
                {
                    cerr << chunk + " file not found" << endl; 
                    client.write(chunk + " file not found");
                }

                cerr << client.peer_address().to_string()
                     << " closed the connection."
                     << endl; 
            },
            listening_socket.accept()
        );
        
        client_handler.detach();
    }

    return EXIT_SUCCESS;
}
