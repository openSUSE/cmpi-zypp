
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
#include "SUSE_SystemSpecificCollectionProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName  = "SUSE_SystemSpecificCollection";
    const char * _InstanceID = "SUSE:ZYPP_POOL";
  } // namespace


SUSE_SystemSpecificCollectionProviderClass::SUSE_SystemSpecificCollectionProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
{
}

CmpiStatus SUSE_SystemSpecificCollectionProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  op.setKey( "InstanceID", _InstanceID);
  rslt.returnData( op );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SystemSpecificCollectionProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );

  const char * keys[] = { "InstanceID" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "InstanceID", _InstanceID );
  ci.setProperty( "ElementName", "Available Software" );
  ci.setProperty( "Caption", "Available Software" );
  ci.setProperty( "Description", "Collection of all Available Software for installation on this System" );

  rslt.returnData( ci );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}


CmpiStatus SUSE_SystemSpecificCollectionProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

  const char * id = cop.getKey( "InstanceId" );
  if( id == NULL || ::strcmp(id, _InstanceID) != 0 )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not find this collection." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );
    CmpiInstance   ci( op );

    const char * keys[] = { "InstanceID" };
    ci.setPropertyFilter( properties, keys );

    ci.setProperty( "InstanceID", _InstanceID );
    ci.setProperty( "ElementName", "Available Software" );
    ci.setProperty( "Caption", "Available Software" );
    ci.setProperty( "Description", "All Available Software on this System" );

    rslt.returnData( ci );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

} // namespace cmpizypp

CMProviderBase( SUSE_SystemSpecificCollectionProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SystemSpecificCollectionProviderClass, SUSE_SystemSpecificCollectionProvider );
