/* simple TCP listener/server to demonstrate sourdough starter classes */
/* Keith Winstein <keithw@cs.stanford.edu>, January 2015 */

#include <thread>
#include <iostream>
#include <fstream>

#include "cups/Spooler.h"
#include "cups/User.h"
#include "cups/Printer.h"

#include "sync/process.h"
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
    
    cups::Spooler server(netsys::Address("::0", argv[1]));
    server.join(new cups::Printer());
    server.run();

    return EXIT_SUCCESS;
}
