
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
#include "SUSE_SoftwareIdentityProvider.h"
#include "SUSE_InstalledSoftwareIdentityProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName     = "SUSE_InstalledSoftwareIdentity";
    const char * _RefLeft       = "InstalledSoftware";
    const char * _RefRight      = "System";
    const char * _RefLeftClass  = "SUSE_SoftwareIdentity";
    const char * _RefRightClass = "CIM_ComputerSystem";

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

    CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);

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

    CmpiObjectPath rop( cop.getNameSpace(), _ClassName );

    ResPool pool( zyppac->pool() );
    for_( it, pool.begin(), pool.end() )
    {
      if( !(*it).status().isInstalled() )
        continue;

      CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
      CmpiInstance ci( rop );
      if( ci.isNull() )
      {
        return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
      }
      ci.setProperty( _RefLeft, iop );
      ci.setProperty( _RefRight, csop );

      CmpiObjectPath tmp = ci.getObjectPath();
      tmp.setNameSpace(cop.getNameSpace());
      rslt.returnData(tmp);
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);

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

    CmpiObjectPath rop( cop.getNameSpace(), _ClassName );
    ResPool pool( zyppac->pool() );
    for_( it, pool.begin(), pool.end() )
    {
      if( !(*it).status().isInstalled() )
        continue;

      CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
      CmpiInstance ci( rop );
      if( ci.isNull() )
      {
        return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
      }
      ci.setProperty( _RefLeft, iop );
      ci.setProperty( _RefRight, csop );

      rslt.returnData(ci);
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

    CmpiInstance ci = assoc_get_inst( *broker, ctx, cop, _ClassName, _RefLeft, _RefRight);

    rslt.returnData( ci );
    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }



  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::associators( const CmpiContext& ctx, CmpiResult& rslt,
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
        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          SUSE_InstalledSoftwareIdentityFilter filter(get_this_computersystem(*broker, ctx, cop));
          if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                                          _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 1, filter ) )
          {
            CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
            _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
            return st;
          }
        }
        else  // CIM_System
        {
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefRightClass) )
          {
            return CmpiStatus(CMPI_RC_ERR_FAILED, "Create references failed.");
          }

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
            if( !(*it).status().isInstalled() )
              continue;

            CmpiInstance ci = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityInstance(*it, *zyppac, cop, properties);
            rslt.returnData(ci);
          }
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
   }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::associatorNames( const CmpiContext& ctx, CmpiResult& rslt,
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
        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          SUSE_InstalledSoftwareIdentityFilter filter(get_this_computersystem(*broker, ctx, cop));
          if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                                          _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 1, filter) )
          {
            CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
            _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
            return st;
          }
        }
        else  // CIM_System
        {
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefRightClass) )
          {
            return CmpiStatus(CMPI_RC_ERR_FAILED, "Create references failed.");
          }

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
            if( !(*it).status().isInstalled() )
              continue;

            CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
/*
            CmpiObjectPath tmp = (CmpiObjectPath)data;
            tmp.setNameSpace(cop.getNameSpace());
*/
            rslt.returnData(iop);
          }
        }

        USR << op <<endl;
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::references( const CmpiContext& ctx, CmpiResult& rslt,
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
        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          SUSE_InstalledSoftwareIdentityFilter filter(get_this_computersystem(*broker, ctx, cop));
          if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                                          _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 0, filter ) )
          {
            CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
            _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
            return st;
          }
        }
        else  // CIM_System
        {
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefRightClass) )
          {
            return CmpiStatus(CMPI_RC_ERR_FAILED, "Create references failed.");
          }

          CmpiObjectPath rop( cop.getNameSpace(), _ClassName ); // associations op

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
            if( !(*it).status().isInstalled() )
              continue;

            CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
            CmpiInstance ci( rop );
            if( ci.isNull() )
            {
              return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
            }
            ci.setProperty( _RefLeft, iop );
            ci.setProperty( _RefRight, cop );
            rslt.returnData(ci);
          }
        }

        USR << op <<endl;
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI references() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstalledSoftwareIdentityProviderClass::referenceNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* resultClass, const char* role )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !resultClass || op.classPathIsA( resultClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        NULL/*resultClass*/, role, NULL /*resultRole*/ ) )
      {
        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          SUSE_InstalledSoftwareIdentityFilter filter(get_this_computersystem(*broker, ctx, cop));
          if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                                           _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 0, filter ) )
          {
            CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
            _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
            return st;
          }
        }
        else  // CIM_System
        {
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefRightClass) )
          {
            return CmpiStatus(CMPI_RC_ERR_FAILED, "Create references failed.");
          }

          CmpiObjectPath rop( cop.getNameSpace(), _ClassName ); // associations op

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
            if( !(*it).status().isInstalled() )
              continue;

            CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
            CmpiInstance ci( rop );
            if( ci.isNull() )
            {
              return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
            }
            ci.setProperty( _RefLeft, iop );
            ci.setProperty( _RefRight, cop );

            CmpiObjectPath tmp = ci.getObjectPath();
            tmp.setNameSpace(cop.getNameSpace());
            rslt.returnData(tmp);
          }
        }

        USR << op <<endl;
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
 }


 SUSE_InstalledSoftwareIdentityFilter::SUSE_InstalledSoftwareIdentityFilter(const CmpiObjectPath &op)
 : SUSE_AssocFilter()
 , csop(op)
 { }


 bool SUSE_InstalledSoftwareIdentityFilter::filterObjectPath(const CmpiObjectPath &scop, const char * _RefSource, const CmpiObjectPath &op, bool associators) const
 {
    _CMPIZYPP_TRACE(1,("--- SUSE_InstalledSoftwareIdentityFilter::filterObjectPath called"));
    USR << scop << " " <<_RefSource << ": " <<  op << endl;

    if ( strcmp(_RefSource,_RefLeft) != 0 )
    {
      INT << "Unexpected filter!" << endl;
      return false;
    }

    const char * instanceId = scop.getKey("InstanceId");
    if ( ! instanceId )
    {
      INT << "No instance Id!" << endl;
      return false;
    }

    if ( ! ZyppAC::isSystemSoftwareIdentityInstanceId( instanceId ) )
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_InstalledSoftwareIdentityFilter::filterObjectPath exited: false"));
      return false;
    }

    CmpiObjectPath filterop("", "");
    if( ! associators )
    {
      filterop = op.getKey( _RefRight );
    }
    else
    {
      filterop = op;
    }

    CmpiString CSName = csop.getKey("CreationClassName");
    CmpiString Name = csop.getKey("Name");

    CmpiString filterCSName = filterop.getKey("CreationClassName");
    CmpiString filterName = filterop.getKey("Name");

    if(filterCSName.equals(CSName) && filterName.equals(Name) )
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_InstalledSoftwareIdentityFilter::filterObjectPath exited: true"));
      return true;
    }
    else
    {
      _CMPIZYPP_TRACE(1,("--- SUSE_InstalledSoftwareIdentityFilter::filterObjectPath exited: false"));
      return false;
    }
 }


 bool SUSE_InstalledSoftwareIdentityFilter::filterInstance(const CmpiObjectPath &scop, const char * _RefSource, const CmpiInstance &ci, bool associators) const
 {
    _CMPIZYPP_TRACE(1,("--- SUSE_InstalledSoftwareIdentityFilter::filterInstance called"));
    return filterObjectPath(scop, _RefSource, ci.getObjectPath(), associators);
 }

} // namespace cmpizypp

CMProviderBase( SUSE_InstalledSoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_InstalledSoftwareIdentityProviderClass, SUSE_InstalledSoftwareIdentityProvider );
CMAssociationMIFactory( cmpizypp::SUSE_InstalledSoftwareIdentityProviderClass, SUSE_InstalledSoftwareIdentityProvider );
