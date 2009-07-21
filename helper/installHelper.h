#ifndef INSTALLHELPER_H
#define INSTALLHELPER_H

#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>

#define SHM_NAME "MySharedMemory"
#define STR_SIZE 256

#define MAX_ANSWERS 10

namespace cmpizypp
{
  enum Answers
  {
    A_unknown = 0,
    A_yes, A_no, A_cancel, A_abort
  };

  struct Comm
  {
    Comm() : lock(1) {}

    boost::interprocess::interprocess_semaphore lock;

    pid_t     pid;
    uint16_t  status;
    uint16_t  percent;
    uint16_t  error;
    char      errorStr[STR_SIZE];
    uint16_t  answers[MAX_ANSWERS];
    char      questionStr[STR_SIZE];
    char      dataStr[STR_SIZE];
  };


  extern Comm & getshmem();

  struct CommAccess : private boost::noncopyable
  {
    CommAccess()
      : _c( getshmem() )
    {
      if ( ! _c.lock.timed_wait( boost::posix_time::from_time_t( ::time(0)+5 ) ) )
      {
        throw "got no lock";
      }
    }

    ~CommAccess()
    {
      _c.lock.post();
    }

    const Comm * operator->() const
    { return &_c; }

    Comm * operator->()
    { return &_c; }

    private:
      Comm & _c;
  };

}
#endif