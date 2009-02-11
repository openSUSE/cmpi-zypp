
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
#include "SUSE_SoftwareRepositoryProvider.h"
#include "SUSE_RepositoryAvailableForSoftwareIdentityProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName     = "SUSE_RepositoryAvailableForSoftwareIdentity";
    const char * _RefLeft       = "AvailableSAP";
    const char * _RefRight      = "ManagedElement";
    const char * _RefLeftClass  = "SUSE_SoftwareRepository";
    const char * _RefRightClass = "SUSE_SoftwareIdentity";

  } // namespace

  SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::SUSE_RepositoryAvailableForSoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiAssociationMI( mbp, ctx )
  {
  }

  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

    CmpiObjectPath op(cop.getNameSpace(), _RefLeftClass); // Enum over Repositories
    CmpiEnumeration en = broker->enumInstanceNames( ctx, op );
    if( en.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstanceNames failed.");
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed: %s", _ClassName, st.msg() ) );
      return st;
    }

    // zypp init
    zypp::scoped_ptr<ZyppAC> zyppac;
    try
    {
      zyppac.reset( new ZyppAC() );
    }
    catch ( const zypp::Exception & err )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    while( en.hasNext() )
    {
      CmpiObjectPath leftop = en.getNext();
      const char* repoalias = leftop.getKey( "Name" );

      try
      {
        ResPool pool( zyppac->pool() );
        Repository r ( pool.reposFind( repoalias ) );
        if( !r )
        {
          continue;
        }

        _CMPIZYPP_TRACE(1,("--- list SoftwareIdentities in %s ", repoalias ));

        for_( it, r.solvablesBegin(), r.solvablesEnd() )
        {
          CmpiObjectPath rightop ( cop.getNameSpace(), _RefRightClass );
          rightop.setKey( "InstanceID", zyppac->SoftwareIdentityInstanceId( *it ).c_str() );

          CmpiObjectPath resop( cop.getNameSpace(), _ClassName );
          resop.setKey( _RefLeft, leftop );
          resop.setKey( _RefRight, rightop );
          rslt.returnData( resop );
        }
      }
      catch( const zypp::Exception & err )
      {
        CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "" ).c_str() );
        _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed : %s", _ClassName, rc.msg()));
        return rc;
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

    CmpiObjectPath op(cop.getNameSpace(), _RefLeftClass); // Enum over Rpositories
    CmpiEnumeration en = broker->enumInstanceNames( ctx, op );
    if( en.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstanceNames failed.");
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() failed: %s", _ClassName, st.msg() ) );
      return st;
    }

    // zypp init
    zypp::scoped_ptr<ZyppAC> zyppac;
    try
    {
      zyppac.reset( new ZyppAC() );
    }
    catch ( const zypp::Exception & err )
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    while( en.hasNext() )
    {
      CmpiObjectPath leftop = en.getNext();
      const char* repoalias = leftop.getKey( "Name" );

      try
      {
        ResPool pool( zyppac->pool() );
        Repository r ( pool.reposFind( repoalias ) );
        if( !r )
        {
          continue;
        }

        _CMPIZYPP_TRACE(1,("--- list SoftwareIdentities in %s", repoalias ));

        for_( it, r.solvablesBegin(), r.solvablesEnd() )
        {
          CmpiObjectPath rightop ( cop.getNameSpace(), _RefRightClass );
          rightop.setKey( "InstanceID", zyppac->SoftwareIdentityInstanceId( *it ).c_str() );

          CmpiInstance resinst( CmpiObjectPath(cop.getNameSpace(), _ClassName ) );
          resinst.setProperty( _RefLeft, leftop );
          resinst.setProperty( _RefRight, rightop );
          rslt.returnData( resinst );
        }
      }
      catch( const zypp::Exception & err )
      {
        CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "" ).c_str() );
        _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() failed : %s", _ClassName, rc.msg()));
        return rc;
      }
    }


    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }


  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

    CmpiInstance ci = assoc_get_inst( *broker, ctx, cop, _ClassName, _RefLeft, _RefRight);

    rslt.returnData( ci );
    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }



  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::associators( const CmpiContext& ctx, CmpiResult& rslt,
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
        // zypp init
        zypp::scoped_ptr<ZyppAC> zyppac;
        try
        {
          zyppac.reset( new ZyppAC() );
        }
        catch ( const zypp::Exception & err )
        {
          CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
          _CMPIZYPP_TRACE(1,("--- %s CMPI associators() failed : %s", _ClassName, rc.msg()));
          return rc;
        }

        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareRepository
        {
          const char* repoalias = cop.getKey( "Name" );

          try
          {
            ResPool pool( zyppac->pool() );
            for_( it, pool.begin(), pool.end() )
            {
              if( (*it)->repository().alias() != repoalias )
              {
                continue;
              }
              _CMPIZYPP_TRACE(1,("--- list SoftwareIdentities in %s", repoalias ));

              rslt.returnData(SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityInstance( *it, *zyppac, cop, properties));
            }
          }
          catch( const zypp::Exception & err )
          {
            CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "" ).c_str() );
            _CMPIZYPP_TRACE(1,("--- %s CMPI associators() failed : %s", _ClassName, rc.msg()));
            return rc;
          }
        }
        else  // SUSE_SoftwareIdentity
        {
          const char* instanceID = cop.getKey("InstanceID");

          PoolItem pi = zyppac->findSoftwareIdentityInstanceId( instanceID );

          CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);

          rslt.returnData( SUSE_SoftwareRepositoryProviderClass::makeInstance( pi->repository().info(), cop, csop, properties ) );
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associators() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
   }

  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::associatorNames( const CmpiContext& ctx, CmpiResult& rslt,
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
        // zypp init
        zypp::scoped_ptr<ZyppAC> zyppac;
        try
        {
          zyppac.reset( new ZyppAC() );
        }
        catch ( const zypp::Exception & err )
        {
          CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
          _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() failed : %s", _ClassName, rc.msg()));
          return rc;
        }

        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareRepository
        {
          const char* repoalias = cop.getKey( "Name" );

          try
          {
            ResPool pool( zyppac->pool() );
            for_( it, pool.begin(), pool.end() )
            {
              if( (*it)->repository().alias() != repoalias )
              {
                continue;
              }
              _CMPIZYPP_TRACE(1,("--- list SoftwareIdentities in %s", repoalias ));

              rslt.returnData(SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP( *it, *zyppac, cop ));
            }
          }
          catch( const zypp::Exception & err )
          {
            CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "" ).c_str() );
            _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() failed : %s", _ClassName, rc.msg()));
            return rc;
          }
        }
        else  // SUSE_SoftwareIdentity
        {
          const char* instanceID = cop.getKey("InstanceID");

          PoolItem pi = zyppac->findSoftwareIdentityInstanceId( instanceID );

          CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);

          rslt.returnData( SUSE_SoftwareRepositoryProviderClass::makeObjectPath( pi->repository().info(), cop, csop ) );
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI associatorNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::references( const CmpiContext& ctx, CmpiResult& rslt,
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
        // zypp init
        zypp::scoped_ptr<ZyppAC> zyppac;
        try
        {
          zyppac.reset( new ZyppAC() );
        }
        catch ( const zypp::Exception & err )
        {
          CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
          _CMPIZYPP_TRACE(1,("--- %s CMPI references() failed : %s", _ClassName, rc.msg()));
          return rc;
        }

        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareRepository
        {
          const char* repoalias = cop.getKey( "Name" );

          try
          {
            ResPool pool( zyppac->pool() );
            for_( it, pool.begin(), pool.end() )
            {
              if( (*it)->repository().alias() != repoalias )
              {
                continue;
              }
              _CMPIZYPP_TRACE(1,("--- list SoftwareIdentities in %s", repoalias ));

              CmpiInstance ci( CmpiObjectPath(cop.getNameSpace(), _ClassName) );
              ci.setProperty( _RefLeft, cop );
              ci.setProperty( _RefRight, SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP( *it, *zyppac, cop ) );

              rslt.returnData( ci );
            }
          }
          catch( const zypp::Exception & err )
          {
            CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "" ).c_str() );
            _CMPIZYPP_TRACE(1,("--- %s CMPI references() failed : %s", _ClassName, rc.msg()));
            return rc;
          }
        }
        else  // SUSE_SoftwareIdentity
        {
          const char* instanceID = cop.getKey("InstanceID");

          PoolItem pi = zyppac->findSoftwareIdentityInstanceId( instanceID );

          CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);

          CmpiInstance ci( CmpiObjectPath(cop.getNameSpace(), _ClassName) );
          ci.setProperty( _RefLeft, SUSE_SoftwareRepositoryProviderClass::makeObjectPath( pi->repository().info(), cop, csop ) );
          ci.setProperty( _RefRight, cop );

          rslt.returnData( ci );
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI references() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
  }

  CmpiStatus SUSE_RepositoryAvailableForSoftwareIdentityProviderClass::referenceNames( const CmpiContext& ctx, CmpiResult& rslt,
      const CmpiObjectPath& cop, const char* resultClass, const char* role )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() called",_ClassName));

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );

    if ( !resultClass || op.classPathIsA( resultClass ) )
    {
      if ( assoc_check_parameter_const( cop, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                        NULL/*resultClass*/, role, NULL /*resultRole*/ ) )
      {
        // zypp init
        zypp::scoped_ptr<ZyppAC> zyppac;
        try
        {
          zyppac.reset( new ZyppAC() );
        }
        catch ( const zypp::Exception & err )
        {
          CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
          _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() failed : %s", _ClassName, rc.msg()));
          return rc;
        }

        if ( cop.classPathIsA(_RefLeftClass) )  //SUSE_SoftwareRepository
        {
          const char* repoalias = cop.getKey( "Name" );

          try
          {
            ResPool pool( zyppac->pool() );
            for_( it, pool.begin(), pool.end() )
            {
              if( (*it)->repository().alias() != repoalias )
              {
                continue;
              }
              _CMPIZYPP_TRACE(1,("--- list SoftwareIdentities in %s", repoalias ));

              CmpiInstance ci( CmpiObjectPath(cop.getNameSpace(), _ClassName) );
              ci.setProperty( _RefLeft, cop );
              ci.setProperty( _RefRight, SUSE_SoftwareIdentityProviderClass::makeSoftwareIdentityOP( *it, *zyppac, cop ) );

              CmpiObjectPath tmp = ci.getObjectPath();
              tmp.setNameSpace(cop.getNameSpace());

              rslt.returnData( tmp );
            }
          }
          catch( const zypp::Exception & err )
          {
            CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "" ).c_str() );
            _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() failed : %s", _ClassName, rc.msg()));
            return rc;
          }
        }
        else  // SUSE_SoftwareIdentity
        {
          const char* instanceID = cop.getKey("InstanceID");

          PoolItem pi = zyppac->findSoftwareIdentityInstanceId( instanceID );

          CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);

          CmpiInstance ci( CmpiObjectPath(cop.getNameSpace(), _ClassName) );
          ci.setProperty( _RefLeft, SUSE_SoftwareRepositoryProviderClass::makeObjectPath( pi->repository().info(), cop, csop ) );
          ci.setProperty( _RefRight, cop );

          CmpiObjectPath tmp = ci.getObjectPath();
          tmp.setNameSpace(cop.getNameSpace());

          rslt.returnData( tmp );
        }
      }
    }

    rslt.returnDone();
    _CMPIZYPP_TRACE(1,("--- %s CMPI referenceNames() exited",_ClassName));
    return CmpiStatus(CMPI_RC_OK);
 }

} // namespace cmpizypp

CMProviderBase( SUSE_RepositoryAvailableForSoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_RepositoryAvailableForSoftwareIdentityProviderClass, SUSE_RepositoryAvailableForSoftwareIdentityProvider );
CMAssociationMIFactory( cmpizypp::SUSE_RepositoryAvailableForSoftwareIdentityProviderClass, SUSE_RepositoryAvailableForSoftwareIdentityProvider );
