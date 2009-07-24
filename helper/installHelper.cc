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

  {
    ShmAccess<Comm> comm( shm(), "Comm" );
    MIL << comm->pid << endl;
    if ( comm->pid != getpid() )
    {
      ERR << "Not my pid: " << comm->pid << "(" << getpid() << ")" << endl;
      return 1;
    }
    comm->setStatus( JS_RUNNING );
  }


  MIL << "Prepare to receive data..." << endl;
  ShmAccessUnlocked<TextExch> textExch( shm(), "TextExch" );
  MIL << "Receive data..." << endl;
  std::string rec;
  do {
    rec = textExch->get();
    USR << rec << endl;
  } while( ! rec.empty() );
  MIL << "Received." << endl;


  for(int i = 0; i < 10; ++i)
  {
    ShmAccess<Comm> comm( shm(), "Comm" );
    comm->percent = i*10;
    sleep(1);
  }

  {
    ShmAccess<Comm> comm( shm(), "Comm" );
    comm->percent = 100;
    comm->setStatus( JS_COMPLETED );
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
