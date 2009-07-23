#ifndef INSTALLHELPER_H
#define INSTALLHELPER_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>

#define SHM_NAME "cmpi-zypp-helper"

#define STR_SIZE 256
#define MAX_ANSWERS 10

/*
JobState
Values {"New", "Starting", "Running", "Suspended", "Shutting Down", "Completed", "Terminated", "Killed", "Exception", "Service", "Query Pending", "DMTF Reserved", "Vendor Reserved"}
ValueMap {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13..32767", "32768..65535"}
*/

namespace cmpizypp
{
  /**
   * Base class for lockable shared memory objetcs.
   */
  struct ShmData
  {
    ShmData()
      : _lock(1)
    {}
    boost::interprocess::interprocess_semaphore _lock;
  };

  /**
   * Locked access to shared memory objetcs.
   * \code
   *   struct Comm {...};
   *   managed_shared_memory managed_shm( create_only, SHM_NAME, 65536 );
   *   Comm * comm = managed_shm.construct<Comm>("Comm")();
   *   try
   *   {
   *     ShmAccess<Comm> comm( managed_shm, "Comm" );
   *     // got exclusive access...
   *   }
   *   catch...
   * \endcode
   */
  template <class Derived>
  class ShmAccess : private boost::noncopyable
  {
    public:
      /**
       * Ctor for locked access to a named object.
       * \throws Exception if named object was not found.
       * \throws Exception if timeout expired.
       * \todo Define Exceptions.
       */
      ShmAccess( boost::interprocess::managed_shared_memory & managed_shm_r,
                 const char * name_r, time_t timeout = 5 )
        : _d( managed_shm_r.find<Derived>( name_r ).first )
      {
        if ( !_d )
          throw std::string( "Can't find " ) + name_r;
        if ( _d && ! _d->_lock.timed_wait( boost::posix_time::from_time_t( ::time(0)+timeout ) ) )
          throw std::string( "Got no lock" );
      }

      ~ShmAccess()
      { release(); }

    public:
      /** Give up lock and access. */
      void release()
      { if ( _d ) { _d->_lock.post(); _d = 0; } }

    public:
      const Derived *	get() const		{ return _d; }
      Derived * 	get()			{ return _d; }
      const Derived *	operator->() const	{ return _d; }
      Derived * 	operator->()		{ return _d; }
      const Derived & 	operator*() const	{ return _d; }
      Derived & 	operator*()		{ return _d; }
    private:
      Derived * _d;
  };


  enum Answers
  {
    A_unknown = 0,
    A_yes, A_no, A_cancel, A_abort
  };

  /**
   */
  struct Comm : public ShmData
  {
    pid_t     pid;
    uint16_t  status;
    uint16_t  percent;
    uint16_t  error;
    char      errorStr[STR_SIZE];
    uint16_t  answers[MAX_ANSWERS];
    char      questionStr[STR_SIZE];
    char      dataStr[STR_SIZE];
  };
}
#endif