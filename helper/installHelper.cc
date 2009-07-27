#include <iostream>
#include <fstream>
#include <list>
#include <set>

#include <zypp/base/LogControl.h>
#include <zypp/base/Exception.h>
#include <zypp/base/String.h>
#include <zypp/base/IOStream.h>

#include <zypp/ZYppFactory.h>
#include <zypp/Misc.h>
#include <zypp/ResPool.h>
#include <zypp/InstanceId.h>

#define HELPERDEBUG
#include "installHelper.h"

using namespace zypp;
using namespace zypp::misc;
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

  ///////////////////////////////////////////////////////////////////
  // Get the job

  std::list<std::string>   instanceIds;
  std::set<InstallOptions> installOptions;

  MIL << "Prepare to receive initial data..." << endl;
  ShmAccessUnlocked<TextExch> textExch( shm(), "TextExch" );

  MIL << "Receive instanceIds..." << endl;
  for ( std::string rec = textExch->get(); ! rec.empty(); rec = textExch->get() )
  {
    USR << rec << endl;
    instanceIds.push_back( rec );
  }
  MIL << "Received." << endl;

  MIL << "Receive InstallOptions..." << endl;
  for ( std::string rec = textExch->get(); ! rec.empty(); rec = textExch->get() )
  {
    InstallOptions opt( InstallOptions(zypp::str::strtonum<uint16_t>(rec)) );
    USR << opt << endl;
    installOptions.insert( opt );
  }
  MIL << "Received." << endl;

  ///////////////////////////////////////////////////////////////////
  // Check the job
  try {

    if ( instanceIds.empty() )
      throw "Nothing To Do";

    // load repos
    defaultLoadSystem( LS_READONLY | LS_NOREFRESH );
    ResPool pool( ResPool::instance() );
    InstanceId instanceId( "SUSE:" ); // using a namespace

    // select and process items to by instance id
    for_( it, instanceIds.begin(), instanceIds.end() )
    {
      PoolItem pi( instanceId(*it) );
      if ( ! pi )
        throw *it + " not in pool";
      MIL << pi << endl;

      INT << "TODO: SET STATUS MISSING" << endl;
    }

    // solve...
    if ( ! getZYpp()->resolver()->resolvePool() )
    {
      ERR << "Solve error ..." << endl;
      getZYpp()->resolver()->problems();
      throw "Solver error";
    }

    // install...
    INT << "TODO: COMMIT MISSING" << endl;

  }
  catch( ... )
  {
    ShmAccess<Comm> comm( shm(), "Comm" );
    comm->setStatus( JS_EXCEPTION );
    throw;
  }

//   for(int i = 0; i < 10; ++i)
//   {
//     ShmAccess<Comm> comm( shm(), "Comm" );
//     comm->percent = i*10;
//     sleep(1);
//   }

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
catch ( const char * exp )
{ INT << "exception: " << (exp?exp:"") << endl; throw; }
catch (...)
{ INT << "exception" << endl; throw; }
