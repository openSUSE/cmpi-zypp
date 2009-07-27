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

#define HELPERDEBUG
#include "installHelper.h"

using namespace zypp;
using namespace zypp::filesystem;
using namespace boost::interprocess;
using namespace cmpizypp;

using std::endl;

/*
void shmDebug()
{
  ExternalProgram( "ls -l /dev/shm" ) >> MIL;
}
*/

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
  INT << "=1" << endl;

  if ( ! shm().construct<Comm>("Comm")() )
  {
    throw std::string( "Out of shmem constructing Comm" );
  }
  INT << "=2" << endl;
  if ( ! shm().find_or_construct<TextExch>("TextExch")() )
  {
    throw std::string( "Out of shmem constructing TextExch" );
  }
  INT << "=3" << endl;

  ShmAccess<Comm> comm( shm(), "Comm" ); // blocks the helper
  USR << "STATUS: " << comm << endl;
  ExternalProgram helper( "./installHelper" );
  comm->pid = helper.getpid();
  comm.release(); // go...
  USR << "STATUS: " << comm << endl;



  MIL << "Prepare to send data..." << endl;
  const char * args[] = {
    "Das Reh huepft hoch",
    "Das Reh huepft weit",
    "Warum auch nicht",
    "Es hat ja Zeit",
  };
  InstallOptions iargs[] = {
    IO_INSTALL,
    IO_UPDATE,
    IO_LOG,
    IO_SOLVE_DEPENDENCIES
  };

  ShmAccessUnlocked<TextExch> textExch( shm(), "TextExch" );
  MIL << "Send data..." << endl;
  for_( it, arrayBegin( args ), arrayEnd( args ) )
  {
    textExch->send( *it );
  }
  textExch->sendEOD();
  MIL << "Send data..." << endl;
  for_( it, arrayBegin( iargs ), arrayEnd( iargs ) )
  {
    textExch->send( *it );
  }
  textExch->sendEOD();
  MIL << "Sent." << endl;


  while ( helper.running() )
  {
    USR << "STATUS: " << comm << endl;
    sleep( 1 );
  }
  USR << "FIN:    " << comm << endl;

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
