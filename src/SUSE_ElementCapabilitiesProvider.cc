
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
#include "SUSE_ElementCapabilitiesProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName     = "SUSE_ElementCapabilities";
    const char * _RefLeft       = "ManagedElement";
    const char * _RefRight      = "Capabilities";
    const char * _RefLeftClass  = "SUSE_SoftwareInstallationService";
    const char * _RefRightClass = "SUSE_SoftwareInstallationServiceCapabilities";

  } // namespace

  SUSE_ElementCapabilitiesProviderClass::SUSE_ElementCapabilitiesProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiAssociationMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_ElementCapabilitiesProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

    bool ret = assoc_create_refs_1toN( *broker, ctx, rslt, cop,
                                       _ClassName,_RefLeft,_RefRight,
                                       _RefLeftClass,_RefRightClass,0,0);
    if(!ret)
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstanceNames failed." );
      return st;
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_ElementCapabilitiesProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    bool ret = assoc_create_refs_1toN( *broker, ctx, rslt, cop,
                                       _ClassName,_RefLeft,_RefRight,
                                       _RefLeftClass,_RefRightClass,1,0);
    if(!ret)
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstances failed." );
      return st;
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


  CmpiStatus SUSE_ElementCapabilitiesProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

    CmpiInstance ci = assoc_get_inst( *broker, ctx, cop, _ClassName, _RefLeft, _RefRight);

    rslt.returnData( ci );
    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }



  CmpiStatus SUSE_ElementCapabilitiesProviderClass::associators( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* assocClass, const char* resultClass,
      const char* role, const char* resultRole, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !assocClass || op.classPathIsA( assocClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        resultClass, role, resultRole ) )
      {
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 1 ) )
        {
          CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
          _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
          return st;
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
   }

  CmpiStatus SUSE_ElementCapabilitiesProviderClass::associatorNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* assocClass, const char* resultClass,
      const char* role, const char* resultRole )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !assocClass || op.classPathIsA( assocClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        resultClass, role, resultRole ) )
      {
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 1 ) )
        {
          CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
          _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
          return st;
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_ElementCapabilitiesProviderClass::references( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* resultClass, const char* role,
      const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI references() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !resultClass || op.classPathIsA( resultClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        NULL/*resultClass*/, role, NULL /*resultRole*/ ) )
      {
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 0 ) )
        {
          CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
          _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
          return st;
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI references() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_ElementCapabilitiesProviderClass::referenceNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* resultClass, const char* role )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !resultClass || op.classPathIsA( resultClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        NULL/*resultClass*/, role, NULL /*resultRole*/ ) )
      {
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 0 ) )
        {
          CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
          _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
          return st;
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
 }

} // namespace cmpizypp

CMProviderBase( SUSE_ElementCapabilitiesProvider );

CMInstanceMIFactory( cmpizypp::SUSE_ElementCapabilitiesProviderClass, SUSE_ElementCapabilitiesProvider );
CMAssociationMIFactory( cmpizypp::SUSE_ElementCapabilitiesProviderClass, SUSE_ElementCapabilitiesProvider );
