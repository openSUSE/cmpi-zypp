#ifndef INSTALLHELPER_H
#define INSTALLHELPER_H

#include <zypp/base/String.h>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>

#define SHM_NAME "cmpi-zypp-helper"

#define STR_SIZE 256
#define MAX_ANSWERS 10

namespace cmpizypp
{
  /**
   * Base class for lockable shared memory objetcs.
   */
  struct ShmData
  {
    typedef boost::interprocess::interprocess_mutex mutex_t;
    mutex_t _mutex;
  };

  enum
  {
    SHMACCESS_UNLOCKED		= -2,
    SHMACCESS_WAIT		= -1,
    SHMACCESS_IMMEDIATE		=  0,
    SHMACCESS_DEFAULTTIMEOUT	=  5,
  };

  /**
   * Locked access (timeout) to shared memory objetcs.
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
                 const char * name_r, time_t timeout = SHMACCESS_DEFAULTTIMEOUT )
        : _d( managed_shm_r.find<Derived>( name_r ).first )
        , _locked( false )
      {
        if ( !_d )
          throw std::string( "Can't find " ) + name_r;
        if ( timeout >= 0 )
        {
          _locked = timeout ? _d->_mutex.timed_lock( boost::posix_time::from_time_t( ::time(0)+timeout ) )
                            : _d->_mutex.try_lock();
          if ( !_locked )
            throw std::string( "Got no lock" );
        }
        else if ( timeout == SHMACCESS_WAIT )
        {
          _d->_mutex.lock();
        }
        // else unlocked
      }

      ~ShmAccess()	{ release(); }

    public:
      /** Give up lock. */
      void release()	{ if ( _locked ) { _d->_mutex.unlock(); _locked = false; } }

    public:
      const Derived *	get() const		{ return _d; }
      Derived * 	get()			{ return _d; }
      const Derived *	operator->() const	{ return _d; }
      Derived * 	operator->()		{ return _d; }
      const Derived & 	operator*() const	{ return *_d; }
      Derived & 	operator*()		{ return *_d; }
    private:
      Derived * _d;
      bool _locked;
  };

  /**
   * Unlocked access to shared memory objetcs.
   */
  template <class Derived>
  struct ShmAccessUnlocked : public ShmAccess<Derived>
  {
    ShmAccessUnlocked( boost::interprocess::managed_shared_memory & managed_shm_r, const char * name_r )
      : ShmAccess<Derived>( managed_shm_r, name_r, SHMACCESS_UNLOCKED )
    {}
  };

  /**
   * Locked access (no timeout) to shared memory objetcs.
   */
  template <class Derived>
  struct ShmAccessWait : public ShmAccess<Derived>
  {
    ShmAccessWait( boost::interprocess::managed_shared_memory & managed_shm_r, const char * name_r )
      : ShmAccess<Derived>( managed_shm_r, name_r, SHMACCESS_WAIT )
    {}
  };

  ////////////////////////////////////////////////////////////////////////////////

  struct TextExch : public ShmData
  {
    enum { buffsize = 1024 };

    TextExch()
      : _filled( false )
    {}

    void sendEOD()
    { send(""); }

    void send( const char * text_r )
    { send( std::string( text_r ? text_r : "" ) ); }

    void send( uint16_t int_r )
    { send( zypp::str::numstring( int_r ) ); }

    void send( const std::string & text_r )
    {
      bool noEOD = true;
      const char * data = text_r.c_str();
      std::string::size_type remaining = text_r.size();
      while ( remaining || noEOD )
      {
        boost::interprocess::scoped_lock<mutex_t> lock( _mutex );
        if ( _filled )
        {
          buff_full.wait( lock );
        }
        if ( remaining )
        {
          std::string::size_type toSend( std::min<unsigned>( remaining, buffsize ) );
          strncpy( _buff, data, toSend );
          data += toSend;
          remaining -= toSend;
        }
        else
        {
          *_buff = 0;
          noEOD = false;
        }
        //Notify to the other process that there is a message
        _filled = true;
        buff_empty.notify_one();
      }
    }

    std::string get()
    {
      bool noEOD = true;
      std::string ret;
      do {
        boost::interprocess::scoped_lock<mutex_t> lock( _mutex );
        if ( ! _filled )
        {
          buff_empty.wait( lock );
        }
        if ( *_buff )
          ret += _buff;
        else
          noEOD = false;
        //Notify the other process that the buffer is empty
        _filled = false;
        buff_full.notify_one();
      } while ( noEOD );
      return ret;
    }

    private:
      /** Wait when buffer is empty. */
      boost::interprocess::interprocess_condition  buff_empty;
      /** Wait when buffer is full. */
      boost::interprocess::interprocess_condition  buff_full;
      /** The buffer. */
      char _buff[buffsize+1];
      /** Whether the buffer has data. */
      bool _filled;
  };

  ////////////////////////////////////////////////////////////////////////////////

  /** Job States
   * \li DMTF Reserved 13..32767
   * \li Vendor Reserved 32768..65535
   */
  enum JobState
  {
    JS_NEW		= 2,
    JS_STARTING		= 3,
    JS_RUNNING		= 4,
    JS_SUSPENDED	= 5,
    JS_SHUTTING_DOWN	= 6,
    JS_COMPLETED	= 7,
    JS_TERMINATE	= 8,
    JS_KILLED		= 9,
    JS_EXCEPTION	= 10,
    JS_SERVICE		= 11,
    JS_QUERY_PENDING	= 12
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
    Comm()
      : pid	( 0 )
      , _status	( JS_NEW )
      , percent	( 0 )
    {}

    JobState status() const		{ return JobState(_status); }
    void setStatus( JobState val_r )	{ _status = val_r; }

    pid_t     pid;
    uint16_t  _status;
    uint16_t  percent;

    uint16_t  error;
    char      errorStr[STR_SIZE];
    uint16_t  answers[MAX_ANSWERS];
    char      questionStr[STR_SIZE];
    char      dataStr[STR_SIZE];
  };
}
#ifdef HELPERDEBUG
#include <iostream>
namespace cmpizypp
{
  std::ostream & operator<<( std::ostream & str, JobState obj )
  {
    switch ( obj )
    {
#define OUTS(E) case E: return str << #E; break
      OUTS( JS_NEW );
      OUTS( JS_STARTING );
      OUTS( JS_RUNNING );
      OUTS( JS_SUSPENDED );
      OUTS( JS_SHUTTING_DOWN );
      OUTS( JS_COMPLETED );
      OUTS( JS_TERMINATE );
      OUTS( JS_KILLED );
      OUTS( JS_EXCEPTION );
      OUTS( JS_SERVICE );
      OUTS( JS_QUERY_PENDING );
#undef OUTS
    }
    return str << "JS_(" << unsigned(obj) << ")";
  }

  std::ostream & operator<<( std::ostream & str, const Comm & obj )
  {
    return str << "Comm(" << obj.pid << "|" << obj.status() << "|" << obj.percent << "%)";
  }

  template <class Derived>
  inline std::ostream & operator<<( std::ostream & str, const ShmAccess<Derived> & obj )
  { return str << *obj; }
}
#endif
#endif