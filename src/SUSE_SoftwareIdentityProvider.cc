
#include <iostream>

#include <cmpi/cmpimacs.h>
#include <cmpi/CmpiResult.h>
#include <cmpi/CmpiBroker.h>

#include <zypp/base/String.h>
#include <zypp/base/LogTools.h>

#include "SUSE_zypp.h"
#include "SUSE_SoftwareIdentityProvider.h"

using namespace zypp;
using std::endl;

#define _CMPIZYPP_TRACE2( LV, FORMAT ) CMTraceMessage( broker->getEnc(), LV, "SUSE_SoftwareIdentityProvider", str::form( FORMAT ).c_str(), 0 );
#define _CMPIZYPP_LOG( LV, FORMAT ) CMLogMessage( broker->getEnc(), LV, "SUSE_SoftwareIdentityProvider", str::form( FORMAT ).c_str(), 0 );

#define _CMPIZYPP_TRACE( LV, FORMAT ) USR << "[" << LV << "] " << str::form( FORMAT ) << endl


namespace cmpizypp
{
  namespace
  {
    char * _ClassName = "SUSE_SoftwareIdentity";
  } // namespace

  SUSE_SoftwareIdentityProviderClass::SUSE_SoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_SoftwareIdentityProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
  {
    zypp::scoped_ptr<ZyppAC> zyppac;
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

    // zypp init
    try
    {
      zyppac.reset( new ZyppAC() );
    }
    catch ( const zypp::Exception & err )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ("Could not list software identities: "+err.asUserHistory()).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    ResPool pool( zyppac->pool() );
    for_( it, pool.begin(), pool.end() )
    {
      CmpiObjectPath op( cop.getNameSpace(), _ClassName );
      op.setKey( "InstanceID", zyppac->SoftwareIdentityInstanceId( *it ).c_str() );
      rslt.returnData( op );
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_SoftwareIdentityProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    zypp::scoped_ptr<ZyppAC> zyppac;
    // zypp init
    try
    {
      zyppac.reset( new ZyppAC() );
    }
    catch ( const zypp::Exception & err )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ("Could not list software identities: "+err.asUserHistory()).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    ResPool pool( zyppac->pool() );
    for_( it, pool.begin(), pool.end() )
    {
      CmpiObjectPath op( cop.getNameSpace(), _ClassName );
      op.setKey( "InstanceID", zyppac->SoftwareIdentityInstanceId( *it ).c_str() );
      rslt.returnData( op );
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareIdentityProviderClass, SUSE_SoftwareIdentityProvider );
