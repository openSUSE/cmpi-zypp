
#include <iostream>
#include <zypp/base/LogTools.h>

#include <cmpi/cmpimacs.h>
#include <cmpi/cmpidt.h>
#include <cmpi/CmpiResult.h>
#include <cmpi/CmpiBroker.h>
#include <cmpi/CmpiArray.h>
#include <cmpi/CmpiBooleanData.h>

#include "SUSE_zypp.h"
#include "SUSE_SoftwareIdentityProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName = "SUSE_SoftwareIdentity";

    CmpiInstance makeSoftwareIdentityInstance( const PoolItem & pi, ZyppAC & zyppac, const CmpiObjectPath & cop, const char** properties )
    {
      CmpiObjectPath op( cop.getNameSpace(), _ClassName );
      CmpiInstance   ci( op );

      const char * keys[] = { "InstanceID" };
      ci.setPropertyFilter( properties, keys );

      sat::Solvable solv( pi->satSolvable() );

      ci.setProperty( "InstanceID", zyppac.SoftwareIdentityInstanceId( pi ).c_str() );
      ci.setProperty( "IsEntity", CmpiTrue );
      ci.setProperty( "VersionString", solv.edition().c_str() );

      ci.setProperty( "Name", solv.ident().c_str() );
      ci.setProperty( "ElementName", solv.ident().c_str() );
      ci.setProperty( "Manufacturer", solv.vendor().c_str() );

      ci.setProperty( "Caption", pi->summary().c_str() );
      ci.setProperty( "Description", pi->description().c_str() );

      CmpiArray targetOsTypes( 1, CMPI_uint16 );
      targetOsTypes[0] = CMPIUint16(36); // LINUX
      ci.setProperty( "TargetOSTypes", targetOsTypes );

      unsigned size = 1;
      CmpiArray types( size, CMPI_string );
      CmpiArray values( size, CMPI_string );
      std::string softwareFamily( "SUSE:1:36:" );
      softwareFamily += solv.arch().c_str();

      types[0] = CmpiString("CIM:SoftwareFamily");
      values[0] = CmpiString(softwareFamily.c_str());
      ci.setProperty( "IdentityInfoType", types );
      ci.setProperty( "IdentityInfoValue", values );


      CmpiArray classifications( 1, CMPI_uint16 );
      classifications[0] = CMPIUint16(1); // other
      ci.setProperty( "Classifications", classifications );

      if ( solv.isKind<Package>() || solv.isKind<SrcPackage>() )
        ci.setProperty( "ExtendedResourceType", CMPIUint16(3) ); // Linux RPM
      else
        ci.setProperty( "ExtendedResourceType", CMPIUint16(1) ); // other


      if ( solv.isSystem() )
      {
        CMPIUint64 idate = pi->installtime();
        ci.setProperty( "InstallDate", CmpiDateTime( idate*1000000, CmpiFalse ) );
      }

      //
      //ci.setProperty( "ReleaseDate", "NULL" );

      return ci;
    }


  } // namespace

  SUSE_SoftwareIdentityProviderClass::SUSE_SoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_SoftwareIdentityProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

    // zypp init
    zypp::scoped_ptr<ZyppAC> zyppac;
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
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    // zypp init
    zypp::scoped_ptr<ZyppAC> zyppac;
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
      rslt.returnData( makeSoftwareIdentityInstance( *it, *zyppac, cop, properties ) );
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


  CmpiStatus SUSE_SoftwareIdentityProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

    const char * id = cop.getKey( "InstanceId" );
    if( id == NULL )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not get software identities InstanceId: " );
      _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    // zypp init
    zypp::scoped_ptr<ZyppAC> zyppac;
    try
    {
      zyppac.reset( new ZyppAC() );
    }
    catch ( const zypp::Exception & err )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not get software identity: " ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    // id in pool?
    PoolItem pi( zyppac->findSoftwareIdentityInstanceId( id ) );
    if ( ! pi )
    {
      CmpiStatus rc( CMPI_RC_ERR_NOT_FOUND, str::form( "Could not find software identity with key '%s'", id ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    // found one...
    rslt.returnData( makeSoftwareIdentityInstance( pi, *zyppac, cop, properties ) );

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareIdentityProviderClass, SUSE_SoftwareIdentityProvider );
