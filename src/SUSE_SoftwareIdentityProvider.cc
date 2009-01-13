
#include <iostream>

#include <cmpi/cmpimacs.h>
#include <cmpi/cmpidt.h>
#include <cmpi/CmpiResult.h>
#include <cmpi/CmpiBroker.h>
#include <cmpi/CmpiArray.h>
#include <cmpi/CmpiBooleanData.h>

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
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not list software identities: " ).c_str() );
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
     zypp::scoped_ptr<ZyppAC> zyppac;
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    // zypp init
    try
    {
      zyppac.reset( new ZyppAC() );
    }
    catch ( const zypp::Exception & err )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not list software identities: " ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    ResPool pool( zyppac->pool() );
    for_( it, pool.begin(), pool.end() )
    {
      CmpiObjectPath op( cop.getNameSpace(), _ClassName );
      CmpiInstance   ci( op );

      const char * keys[] = { "InstanceID" };
      ci.setPropertyFilter( properties, keys );

      sat::Solvable solv( it->satSolvable() );

      ci.setProperty( "InstanceID", zyppac->SoftwareIdentityInstanceId( *it ).c_str() );
      ci.setProperty( "IsEntity", CmpiTrue );
      ci.setProperty( "VersionString", solv.edition().c_str() );

      CmpiArray targetOsTypes( 1, CMPI_uint16 );
      targetOsTypes[0] = CMPIUint16(36); // LINUX
      ci.setProperty( "TargetOSTypes", targetOsTypes );

      if ( 0 )
      {
      unsigned size = 1;
      CmpiArray types( size, CMPI_string );
      CmpiArray values( size, CMPI_string );
      std::string softwareFamily( "SUSE:1:36:" );
      softwareFamily += solv.arch().c_str();
      types [0] = "CIM:SoftwareFamily";
      values[0] = softwareFamily.c_str();
      ci.setProperty( "IndenitifyingInfoTypes", types );
      ci.setProperty( "IndenitifyingInfoValues", values );
      }

      CmpiArray classifications( 1, CMPI_uint16 );
      classifications[0] = CMPIUint16(1); // other
      ci.setProperty( "Classifications", classifications );

      if ( solv.isKind<Package>() || solv.isKind<SrcPackage>() )
        ci.setProperty( "ExtendedResourceType", CMPIUint16(3) ); // Linux RPM
      else
        ci.setProperty( "ExtendedResourceType", CMPIUint16(1) ); // other

/*      types[0]  = "";
      values[0] = "";*/

//       ci.setProperty( "", it-> );


      rslt.returnData( ci );
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
 }

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareIdentityProviderClass, SUSE_SoftwareIdentityProvider );
