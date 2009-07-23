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


namespace cmpizypp
{
  managed_shared_memory & shm()
  {
    static managed_shared_memory managed_shm( open_only, SHM_NAME );
    return managed_shm;
  }
}

int main( int argc, char * argv[] )
try {
  //zypp::base::LogControl::instance().logToStdErr();
  INT << "===[START]==========================================" << endl;
  ///////////////////////////////////////////////////////////////////

  MIL << "InstallHelper " << getpid() << endl;

  std::string iFile;
  {
    ShmAccess<Comm> comm( shm(), "Comm" );
    MIL << comm->pid << endl;
    if ( comm->pid != getpid() )
    {
      ERR << "Not my pid: " << comm->pid << "(" << getpid() << ")" << endl;
      return 1;
    }

    if ( * comm->dataStr )
      iFile = comm->dataStr;
  }

  MIL << "read " << iFile << endl;
  std::ifstream infile( iFile.c_str() );
  for( iostr::EachLine in( infile ); in; in.next() )
  {
    USR << *in << endl;
  }

  MIL << "Done" << endl;

  ///////////////////////////////////////////////////////////////////
  INT << "===[END]============================================" << endl << endl;
  zypp::base::LogControl::instance().logNothing();
  return 0;
}
catch ( const Exception & exp )
{ INT << exp << endl << exp.historyAsString(); throw; }
catch ( const std::string & exp )
{ INT << "exception: " << exp << endl; throw; }
catch (...)
{ INT << "exception" << endl; throw; }
