
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
#include "SUSE_HostedCollectionProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName     = "SUSE_HostedCollection";
    const char * _RefLeft       = "Antecedent";
    const char * _RefRight      = "Dependent";
    const char * _RefLeftClass  = "CIM_ComputerSystem";
    const char * _RefRightClass = "SUSE_SystemSpecificCollection";

  } // namespace

  SUSE_HostedCollectionProviderClass::SUSE_HostedCollectionProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiAssociationMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_HostedCollectionProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

    CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
    bool ret = assoc_create_refs_1toN( *broker, ctx, rslt, csop,
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

  CmpiStatus SUSE_HostedCollectionProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
    bool ret = assoc_create_refs_1toN( *broker, ctx, rslt, csop,
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


  CmpiStatus SUSE_HostedCollectionProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

    CmpiInstance ci = assoc_get_inst( *broker, ctx, cop, _ClassName, _RefLeft, _RefRight);

    rslt.returnData( ci );
    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }



  CmpiStatus SUSE_HostedCollectionProviderClass::associators( const CmpiContext& ctx, CmpiResult& rslt,
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
        SUSE_HostedCollectionFilter filter(get_this_computersystem(*broker, ctx, cop));
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 1, filter ) )
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

  CmpiStatus SUSE_HostedCollectionProviderClass::associatorNames( const CmpiContext& ctx, CmpiResult& rslt,
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
        SUSE_HostedCollectionFilter filter(get_this_computersystem(*broker, ctx, cop));
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 1, filter ) )
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

  CmpiStatus SUSE_HostedCollectionProviderClass::references( const CmpiContext& ctx, CmpiResult& rslt,
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
        SUSE_HostedCollectionFilter filter(get_this_computersystem(*broker, ctx, cop));
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 0, filter ) )
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

  CmpiStatus SUSE_HostedCollectionProviderClass::referenceNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* resultClass, const char* role )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !resultClass || op.classPathIsA( resultClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        NULL/*resultClass*/, role, NULL /*resultRole*/ ) )
      {
        SUSE_HostedCollectionFilter filter(get_this_computersystem(*broker, ctx, cop));
        if(! assoc_create_refs_1toN( *broker, ctx, rslt, cop, _ClassName,
                                     _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 0, filter ) )
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

/* -------------------------------------------------------------------------- */

 SUSE_HostedCollectionFilter::SUSE_HostedCollectionFilter(const CmpiObjectPath &op)
 : SUSE_AssocFilter()
 , csop(op)
 { }

 bool SUSE_HostedCollectionFilter::filterInstance(const CmpiInstance &ci, bool associators) const
 {
    _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterInstance called"));
    USR << ci << endl;
    CmpiString CSName = csop.getKey("CreationClassName");
    CmpiString Name = csop.getKey("Name");

    const char *filterCSName = "";
    const char *filterName   = "";
    if( ! associators )
    {
      CmpiObjectPath filterop = ci.getProperty( _RefLeft );
      filterCSName = filterop.getKey("CreationClassName");
      filterName   = filterop.getKey("Name");
    }
    else
    {
      CmpiInstance lci( ci );
      if( lci.instanceIsA( _RefLeftClass ) )
      {
        _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterInstance exited: true"));
        return true;
      }

      filterCSName = ci.getProperty("CreationClassName");
      filterName   = ci.getProperty("Name");
    }

    if(CSName.equals(filterCSName) && Name.equals(filterName) )
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterInstance exited: true"));
      return true;
    }
    else
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterInstance exited: false"));
      return false;
    }
 }

 bool SUSE_HostedCollectionFilter::filterObjectPath(const CmpiObjectPath &op, bool associators) const
 {
    _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterObjectPath called"));
    USR << op << endl;
    CmpiString CSName = csop.getKey("CreationClassName");
    CmpiString Name = csop.getKey("Name");

    CmpiObjectPath filterop("", "");
    if( ! associators )
    {
      filterop = op.getKey( _RefLeft );
    }
    else
    {
      if( op.classPathIsA( _RefLeftClass ) )
      {
        _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterObjectPath exited: true"));
        return true;
      }

      filterop = op;
    }
    CmpiString filterCSName = filterop.getKey("CreationClassName");
    CmpiString filterName = filterop.getKey("Name");

    if(filterCSName.equals(CSName) && filterName.equals(Name) )
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterObjectPath exited: true"));
      return true;
    }
    else
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_HostedCollectionFilter::filterObjectPath exited: false"));
      return false;
    }
 }

} // namespace cmpizypp

CMProviderBase( SUSE_HostedCollectionProvider );

CMInstanceMIFactory( cmpizypp::SUSE_HostedCollectionProviderClass, SUSE_HostedCollectionProvider );
CMAssociationMIFactory( cmpizypp::SUSE_HostedCollectionProviderClass, SUSE_HostedCollectionProvider );
