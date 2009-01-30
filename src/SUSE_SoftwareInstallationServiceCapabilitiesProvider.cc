
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
#include "SUSE_Common.h"
#include "SUSE_SoftwareInstallationServiceCapabilitiesProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName  = "SUSE_SoftwareInstallationServiceCapabilities";
    const char * _InstanceID = "SUSE:ZYPP_CAPS";
  } // namespace


SUSE_SoftwareInstallationServiceCapabilitiesProviderClass::SUSE_SoftwareInstallationServiceCapabilitiesProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
{
}

CmpiStatus SUSE_SoftwareInstallationServiceCapabilitiesProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  op.setKey( "InstanceID", _InstanceID );
  rslt.returnData( op );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareInstallationServiceCapabilitiesProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

  rslt.returnData( makeInstance( cop, properties ) );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}


CmpiStatus SUSE_SoftwareInstallationServiceCapabilitiesProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

  const char *id = cop.getKey("InstanceID");

  if( id == NULL || ::strcmp( id, _InstanceID ) != 0 )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not find this instance." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  rslt.returnData( makeInstance( cop, properties ) );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiInstance SUSE_SoftwareInstallationServiceCapabilitiesProviderClass::makeInstance( const CmpiObjectPath & cop, const char ** properties )
{
  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );

  const char * keys[] = { "InstanceID" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "InstanceID", _InstanceID );

  unsigned size = 3;
  CmpiArray opts( size, CMPI_uint16 );
  opts[0] = CMPIUint16(3); // Force installation
  opts[1] = CMPIUint16(4); // Install
  opts[2] = CMPIUint16(5); // Update
  //opts[3] = CMPIUint16();
  ci.setProperty( "SupportedInstallOptions", opts );
  ci.setProperty( "CanAddToCollection", CmpiFalse );

  size = 1;
  CmpiArray types( size, CMPI_uint16 );
  types[0] = CMPIUint16(3); // Linux RPM
  ci.setProperty( "SupportedExtendedResourceTypes", types );

  CmpiArray typesmajv( size, CMPI_uint16 );
  typesmajv[0] = CMPIUint16(4); // RPM Major version
  ci.setProperty( "SupportedExtendedResourceTypesMajorVersions", typesmajv );

  CmpiArray typesminv( size, CMPI_uint16 );
  typesminv[0] = CMPIUint16(4); // RPM Major version
  ci.setProperty( "SupportedExtendedResourceTypesMinorVersions", typesminv );

  CmpiArray typesrevn( size, CMPI_uint16 );
  typesrevn[0] = CMPIUint16(2); // RPM Revision Number
  ci.setProperty( "SupportedExtendedResourceTypesRevisionNumbers", typesrevn );

  CmpiArray typesbn( size, CMPI_uint16 );
  typesbn[0] = CMPIUint16(0); // RPM Major version
  ci.setProperty( "SupportedExtendedResourceTypesBuildNumbers", typesbn );

  CmpiArray instsync( size, CMPI_uint16 );
  instsync[0] = CMPIUint16(3); // Install From Software Identity
  ci.setProperty( "SupportedSynchronousActions", instsync );

  CmpiArray instasync( size, CMPI_uint16 );
  instasync[0] = CMPIUint16(2); // None supported
  ci.setProperty( "SupportedAsynchronousActions", instasync );


  return ci;
}

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareInstallationServiceCapabilitiesProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareInstallationServiceCapabilitiesProviderClass, SUSE_SoftwareInstallationServiceCapabilitiesProvider );
