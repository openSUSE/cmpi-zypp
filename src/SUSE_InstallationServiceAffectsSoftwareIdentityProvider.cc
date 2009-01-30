
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
#include "SUSE_InstallationServiceAffectsSoftwareIdentityProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName     = "SUSE_InstallationServiceAffectsSoftwareIdentity";
    const char * _RefLeft       = "AffectedElement";
    const char * _RefRight      = "AffectingElement";
    const char * _RefLeftClass  = "SUSE_SoftwareIdentity";
    const char * _RefRightClass = "SUSE_SoftwareInstallationService";

  } // namespace

  SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiAssociationMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
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

    CmpiObjectPath op(cop.getNameSpace(), _RefRightClass); // SUSE_SoftwareInstallationService
    if( op.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed.");
      _CMPIZYPP_TRACE(1,("--- enumInstanceNames() failed: %s", st.msg() ) );
      return st;
    }
    CmpiEnumeration en = broker->enumInstanceNames( ctx, op );
    if( en.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstanceNames failed.");
      _CMPIZYPP_TRACE(1,("--- enumInstanceNames() failed: %s", st.msg() ) );
      return st;
    }

    CmpiObjectPath rop( cop.getNameSpace(), _ClassName );

    while( en.hasNext() )
    {
      CmpiData data = en.getNext();

      ResPool pool( zyppac->pool() );
      for_( it, pool.begin(), pool.end() )
      {
        if( (*it).status().isInstalled() )
          continue;

        CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
        CmpiInstance ci( rop );
        if( ci.isNull() )
        {
          return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
        }
        ci.setProperty( _RefRight, iop );
        ci.setProperty( _RefLeft, data );

        CmpiObjectPath tmp = ci.getObjectPath();
        tmp.setNameSpace(cop.getNameSpace());
        rslt.returnData(tmp);
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
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

    CmpiObjectPath op(cop.getNameSpace(), _RefRightClass); // SUSE_SoftwareInstallationService
    if( op.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed.");
      _CMPIZYPP_TRACE(1,("--- enumInstances() failed: %s", st.msg() ) );
      return st;
    }
    CmpiEnumeration en = broker->enumInstanceNames( ctx, op );
    if( en.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstanceNames failed.");
      _CMPIZYPP_TRACE(1,("--- enumInstances() failed: %s", st.msg() ) );
      return st;
    }

    CmpiObjectPath rop( cop.getNameSpace(), _ClassName );

    while( en.hasNext() )
    {
      CmpiData data = en.getNext();

      ResPool pool( zyppac->pool() );
      for_( it, pool.begin(), pool.end() )
      {
        if( (*it).status().isInstalled() )
          continue;

        CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
        CmpiInstance ci( rop );
        if( ci.isNull() )
        {
          return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
        }
        ci.setProperty( _RefRight, iop );
        ci.setProperty( _RefLeft, data );

        rslt.returnData(ci);
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

    CmpiInstance ci = assoc_get_inst( *broker, ctx, cop, _ClassName, _RefLeft, _RefRight);

    rslt.returnData( ci );
    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }



  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::associators( const CmpiContext& ctx, CmpiResult& rslt,
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
        if ( cop.classPathIsA(_RefRightClass) )  // SUSE_SoftwareInstallationService
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
            _CMPIZYPP_TRACE(1,("--- %s CMPI associators() failed : %s", _ClassName, rc.msg()));
            return rc;
          }

          ResPool pool( zyppac->pool() );
          for_( it, pool.begin(), pool.end() )
          {
            if( (*it).status().isInstalled() )
              continue;

            CmpiInstance ci = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityInstance(*it, *zyppac, cop, properties);
            rslt.returnData(ci);
          }
        }
        else // ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          const char * copInstanceId = cop.getKey("InstanceId");
           if ( ! ZyppAC::isSystemSoftwareIdentityInstanceId( copInstanceId ) )
           {
             if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                  _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 1) )
             {
               CmpiStatus st(CMPI_RC_ERR_FAILED, "Create associations failed.");
               _CMPIZYPP_TRACE(1,("--- CMPI associators() failed."));
               return st;
             }
           }
        }

      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
   }

  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::associatorNames( const CmpiContext& ctx, CmpiResult& rslt,
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
        if ( cop.classPathIsA(_RefRightClass) )  // SUSE_SoftwareInstallationService
        {
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefRightClass) )
          {
            return CmpiStatus(CMPI_RC_ERR_FAILED, "Create instance failed.");
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
            _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() failed : %s", _ClassName, rc.msg()));
            return rc;
          }

          ResPool pool( zyppac->pool() );
          for_( it, pool.begin(), pool.end() )
          {
            if( (*it).status().isInstalled() )
              continue;

            CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
            
	    rslt.returnData(iop);
          }
        }
        else // ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          const char * copInstanceId = cop.getKey("InstanceId");
           if ( ! ZyppAC::isSystemSoftwareIdentityInstanceId( copInstanceId ) )
           {
             if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                  _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 1) )
             {
               CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
               _CMPIZYPP_TRACE(1,("--- CMPI associatorNames() failed."));
               return st;
             }
           }
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::references( const CmpiContext& ctx, CmpiResult& rslt,
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
        if ( cop.classPathIsA(_RefRightClass) )  // SUSE_SoftwareInstallationService
	{
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefLeftClass) )
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
            _CMPIZYPP_TRACE(1,("--- %s CMPI references() failed : %s", _ClassName, rc.msg()));
            return rc;
          }

          ResPool pool( zyppac->pool() );
          for_( it, pool.begin(), pool.end() )
          {
            if( (*it).status().isInstalled() )
              continue;

            CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
            CmpiInstance ci( rop );
            if( ci.isNull() )
            {
              return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
            }
            ci.setProperty( _RefRight, iop );
            ci.setProperty( _RefLeft, cop );
            rslt.returnData(ci);
          }
        }
        else //( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          const char * copInstanceId = cop.getKey("InstanceId");
          if ( ! ZyppAC::isSystemSoftwareIdentityInstanceId( copInstanceId ) )
          {
            if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                 _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 1, 0) )
            {
              CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
              _CMPIZYPP_TRACE(1,("--- CMPI references() failed."));
              return st;
            }
          }
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI references() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass::referenceNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* resultClass, const char* role )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !resultClass || op.classPathIsA( resultClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        NULL/*resultClass*/, role, NULL /*resultRole*/ ) )
      {
        if ( cop.classPathIsA(_RefRightClass) ) // SUSE_SoftwareInstallationService
        {
          // we need to implement our own handling,
          // because we need to search for SUSE_SoftwareIdentity
          CmpiInstance cis( broker->getInstance( ctx, cop, NULL ) ); // source instance

          if( !cis.instanceIsA(_RefRightClass) )
          {
            return CmpiStatus(CMPI_RC_ERR_FAILED, "Create referenceNames failed.");
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
            _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() failed : %s", _ClassName, rc.msg()));
            return rc;
          }

          ResPool pool( zyppac->pool() );
          for_( it, pool.begin(), pool.end() )
          {
            if( (*it).status().isInstalled() )
              continue;

            CmpiObjectPath iop = SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP(*it, *zyppac, cop);
            CmpiInstance ci( rop );
            if( ci.isNull() )
            {
              return CmpiStatus(CMPI_RC_ERR_FAILED, "Create CmpiInstance failed.");
            }
            ci.setProperty( _RefRight, iop );
            ci.setProperty( _RefLeft, cop );

            CmpiObjectPath tmp = ci.getObjectPath();
            tmp.setNameSpace(cop.getNameSpace());
            rslt.returnData(tmp);
          }
        }
        else // ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareIdentity
        {
          const char * copInstanceId = cop.getKey("InstanceId");
          if ( ! ZyppAC::isSystemSoftwareIdentityInstanceId( copInstanceId ) )
          {
            if(! assoc_create_refs_1toN_ST( *broker, ctx, rslt, cop, _ClassName,
                 _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, 0, 0) )
            {
              CmpiStatus st(CMPI_RC_ERR_FAILED, "Create references failed.");
              _CMPIZYPP_TRACE(1,("--- CMPI referenceNames() failed."));
              return st;
            }
          }
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
 }

} // namespace cmpizypp

CMProviderBase( SUSE_InstallationServiceAffectsSoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass, SUSE_InstallationServiceAffectsSoftwareIdentityProvider );
CMAssociationMIFactory( cmpizypp::SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass, SUSE_InstallationServiceAffectsSoftwareIdentityProvider );
