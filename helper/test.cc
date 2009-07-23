#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <fstream>

#include <zypp/base/LogControl.h>
#include <zypp/base/Exception.h>
#include <zypp/base/String.h>
#include <zypp/ExternalProgram.h>
#include <zypp/Pathname.h>
#include <zypp/TmpPath.h>

#include "installHelper.h"

using namespace zypp;
using namespace zypp::filesystem;
using namespace boost::interprocess;
using namespace cmpizypp;

using std::endl;

void shmDebug()
{
  ExternalProgram( "ls -l /dev/shm" ) >> MIL;
}

namespace cmpizypp
{
  managed_shared_memory & shm()
  {
    static ::mode_t mask = ::umask( 0077 );
    static managed_shared_memory managed_shm( create_only, SHM_NAME, 65536 );
    static ::mode_t omask = ::umask( mask );
    return managed_shm;
  }
}

int main( int argc, char * argv[] )
try {
  //zypp::base::LogControl::instance().logToStdErr();
  INT << "===[START]==========================================" << endl;
  ///////////////////////////////////////////////////////////////////

  struct shm_remove
  {
    shm_remove()  { shared_memory_object::remove(SHM_NAME); }
    ~shm_remove() { shared_memory_object::remove(SHM_NAME); }
  } remover;

  if ( ! shm().construct<Comm>("Comm")() )
  {
    throw std::string( "Out of shmem constructing Comm" );
  }

  // write task list
  std::string path( TmpFile().path().asString() );
  std::ofstream o( path.c_str() );
  o << "hi" << endl;
  o.close();

  ShmAccess<Comm> comm( shm(), "Comm" ); // blocks the helper

  strncpy( comm->dataStr, path.c_str(), STR_SIZE );

  ExternalProgram helper( "./installHelper" );
  comm->pid = helper.getpid();
  comm.release(); // go...

  while ( helper.running() )
  {
    sleep( 5 );
    MIL << "ping..." << endl;
  }

  if ( helper.close() != 0 )
    ERR << helper.execError() << endl;

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
