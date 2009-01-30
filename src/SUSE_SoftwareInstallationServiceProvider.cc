
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
#include "SUSE_SoftwareInstallationServiceProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName  = "SUSE_SoftwareInstallationService";
    const char * _Name = "ZYPP";
  } // namespace


SUSE_SoftwareInstallationServiceProviderClass::SUSE_SoftwareInstallationServiceProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
{
}

CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  op.setKey( "SystemCreationClassName", sccn);
  op.setKey( "SystemName", sn);
  op.setKey( "CreationClassName", _ClassName);
  op.setKey( "Name", _Name);
  rslt.returnData( op );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  const char * keys[] = { "SystemCreationClassName", "SystemName", "CreationClassName", "Name" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "SystemCreationClassName", sccn );
  ci.setProperty( "SystemName", sn );
  ci.setProperty( "CreationClassName", _ClassName );
  ci.setProperty( "Name", _Name );

  rslt.returnData( ci );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}


CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

  const char *lsccn = cop.getKey("SystemCreationClassName");
  const char *lsn   = cop.getKey("SystemName");
  const char *lccn = cop.getKey("CreationClassName");
  const char *ln   = cop.getKey("Name");

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  if( lsccn== NULL || lsn == NULL || lccn == NULL || ln == NULL ||
      ::strcmp(lccn, _ClassName) != 0 || ::strcmp(ln, _Name) != 0 ||
      ::strcmp(lsccn, sccn) != 0 || ::strcmp(lsn, sn) != 0)
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not find this instance." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );

  const char * keys[] = { "SystemCreationClassName", "SystemName", "CreationClassName", "Name" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "SystemCreationClassName", sccn );
  ci.setProperty( "SystemName", sn );
  ci.setProperty( "CreationClassName", _ClassName );
  ci.setProperty( "Name", _Name );

  rslt.returnData( ci );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareInstallationServiceProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareInstallationServiceProviderClass, SUSE_SoftwareInstallationServiceProvider );
