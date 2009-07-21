#include <iostream>
#include <fstream>

#include <zypp/base/LogControl.h>
#include <zypp/base/Exception.h>
#include <zypp/base/String.h>
#include <zypp/base/IOStream.h>
#include "installHelper.h"

using namespace zypp;
using namespace cmpizypp;
using namespace boost::interprocess;

using std::endl;


Comm & getshmem()
{
  static shared_memory_object shm( open_only, SHM_NAME, read_write );
  //Map the whole shared memory in this process
  static mapped_region region( shm, read_write );
  return * static_cast<Comm*>(region.get_address());
}


int main( int argc, char * argv[] )
try {
  //zypp::base::LogControl::instance().logToStdErr();
  INT << "===[START]==========================================" << endl;
  ///////////////////////////////////////////////////////////////////

  MIL << "InstallHelper " << getpid() << endl;

  MIL << "Waiting for lock..." << endl;
  std::string iFile;
  {
    CommAccess c;
    MIL << c->pid << endl;
    if ( c->pid != getpid() )
    {
      ERR << "Not my pid: " << c->pid << "(" << getpid() << ")" << endl;
      return 1;
    }

    if ( * c->dataStr )
      iFile = c->dataStr;
  }

  MIL << "read " << iFile << endl;
  std::ifstream infile( iFile.c_str() );
  for( iostr::EachLine in( infile ); in; in.next() )
  {
    DBG << *in << endl;
  }







  MIL << "Done" << endl;

  ///////////////////////////////////////////////////////////////////
  INT << "===[END]============================================" << endl << endl;
  zypp::base::LogControl::instance().logNothing();
  return 0;
}
catch ( const Exception & exp )
{
  INT << exp << endl << exp.historyAsString();
}
catch (...)
{ INT << "exception" << endl; }
