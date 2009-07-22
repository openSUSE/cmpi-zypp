#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <fstream>

#include <zypp/base/LogControl.h>
#include <zypp/base/Exception.h>
#include <zypp/base/String.h>
#include <zypp/Pathname.h>
#include <zypp/TmpPath.h>

#include "installHelper.h"

using namespace zypp;
using namespace zypp::filesystem;
using namespace cmpizypp;
using namespace boost::interprocess;

using std::endl;

namespace cmpizypp
{
  Comm & getshmem()
  {
    static bool initialized = false;
    if ( ! initialized )
    {
    // Create a shared memory object.
      MIL << "Create shared_memory_object " << SHM_NAME << endl;
    }
    static ::mode_t mask = ::umask( 0077 );
    static shared_memory_object shm ( create_only, SHM_NAME, read_write );
    if ( ! initialized )
    {
        ::umask( mask );
    // Set size
        shm.truncate( 1000 );
    }
  // Map the whole shared memory in this process
    static mapped_region region( shm, read_write );
    if ( ! initialized )
    {
    // Write all the memory to '0'
      std::memset( region.get_address(), 0, region.get_size() );
    // Initialize
      new ( region.get_address() ) Comm;
      initialized = true;
    }
    return *static_cast<Comm *>(region.get_address());
  }
}

int main( int argc, char * argv[] )
try {
  //zypp::base::LogControl::instance().logToStdErr();
  INT << "===[START]==========================================" << endl;
  ///////////////////////////////////////////////////////////////////

  struct shm_remove
  {
    shm_remove() {
      MIL << "Remove shared_memory_object " << SHM_NAME << endl;
      shared_memory_object::remove(SHM_NAME);
    }
    ~shm_remove()
    {
      MIL << "Remove shared_memory_object " << SHM_NAME << endl;
      shared_memory_object::remove(SHM_NAME);
    }
  } remover;

  // write task list
  std::string path( TmpFile().path().asString() );
  std::ofstream o( path.c_str() );
  o << "hi" << endl;
  o.close();

  pid_t p = 0;
  {
    CommAccess c( getshmem() );

    // Launch child process
    p = fork();
    if ( p == 0 )
    {
      execvp( "./installHelper", NULL );
      _exit( 128 );
    }

    // parent
    MIL << "installHelper started (" << p << ")" << endl;
    c->pid = p;
    strncpy( c->dataStr, path.c_str(), STR_SIZE );
  }

  int ret = 0;
  int status = 0;
  do
  {
    MIL << "ping..." << endl;
    sleep( 5 );
    ret = waitpid( p, &status, WNOHANG );
  }
  while ( ret == 0 || ( ret == -1 && errno == EINTR ) );
  MIL << "installHelper exited!" << endl;

  ///////////////////////////////////////////////////////////////////
  INT << "===[END]============================================" << endl << endl;
  zypp::base::LogControl::instance().logNothing();
  return 0;
}
catch ( const Exception & exp )
{
  INT << exp << endl << exp.historyAsString();
}
catch (const char * i)
{ INT << "Exception :( " << i << endl; }
catch (...)
{ INT << "Exception :(" << endl; }
