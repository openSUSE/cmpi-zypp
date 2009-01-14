
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
#include "SUSE_InstalledSoftwareIdentityProvider.h"

using namespace zypp;
using std::endl;

std::ostream & operator<<( std::ostream & str, const CmpiObjectPath & obj )
{
  str << obj.getNameSpace().charPtr();
  return str;
}

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName = "SUSE_InstalledSoftwareIdentity";


  } // namespace

  SUSE_InstalledSoftwareIdentityProviderClass::SUSE_InstalledSoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiAssociationMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
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


    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
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


    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));


    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }



  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::associators( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& op, const char* asscClass, const char* resultClass,
      const char* role, const char* resultRole, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() called",_ClassName));

    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
   }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::associatorNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& op, const char* assocClass, const char* resultClass,
      const char* role, const char* resultRole )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() called",_ClassName));

    USR << op << endl;

    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::references( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& op, const char* resultClass, const char* role,
      const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI references() called",_ClassName));

    USR << op << endl;

    _CMPIZYPP_TRACE(1,("--- %s CMPI references() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::referenceNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& op, const char* resultClass, const char* role )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() called",_ClassName));

    USR << op << endl;

    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
 }

} // namespace cmpizypp

CMProviderBase( SUSE_InstalledSoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_InstalledSoftwareIdentityProviderClass, SUSE_InstalledSoftwareIdentityProvider );
CMAssociationMIFactory( cmpizypp::SUSE_InstalledSoftwareIdentityProviderClass, SUSE_InstalledSoftwareIdentityProvider );
