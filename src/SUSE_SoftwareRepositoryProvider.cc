
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
#include "SUSE_SoftwareRepositoryProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName  = "SUSE_SoftwareRepository";
  } // namespace


SUSE_SoftwareRepositoryProviderClass::SUSE_SoftwareRepositoryProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiMethodMI( mbp, ctx )
{
}

CmpiStatus SUSE_SoftwareRepositoryProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

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

  RepoManager repoManager( zyppac->getSysRoot() );

  RepoInfoList repos = repoManager.knownRepositories();
  for ( RepoInfoList::iterator it = repos.begin(); it != repos.end(); ++it )
  {
    CmpiObjectPath op( cop.getNameSpace(), _ClassName );
    op.setKey( "SystemCreationClassName", sccn);
    op.setKey( "SystemName", sn);
    op.setKey( "CreationClassName", _ClassName);
    op.setKey( "Name", it->alias().c_str() );
    rslt.returnData( op );
  }

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareRepositoryProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
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
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoManager repoManager( zyppac->getSysRoot() );

  RepoInfoList repos = repoManager.knownRepositories();
  for ( RepoInfoList::iterator it = repos.begin(); it != repos.end(); ++it )
  {
    rslt.returnData( makeInstance( *it, cop, csop, properties ) );
  }

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}


CmpiStatus SUSE_SoftwareRepositoryProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
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
      ::strcmp(lccn, _ClassName) != 0 ||
      ::strcmp(lsccn, sccn) != 0 || ::strcmp(lsn, sn) != 0)
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not find this instance." );
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
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not list software identities: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoManager repoManager( zyppac->getSysRoot() );
  if( ! repoManager.hasRepo( ln ) )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not find this instance." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }
  RepoInfo repo( repoManager.getRepo( ln ) );

  rslt.returnData( makeInstance( repo, cop, csop, properties ) );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareRepositoryProviderClass::invokeMethod (const CmpiContext &ctx, CmpiResult &rslt,
                                                               const CmpiObjectPath &ref, const char *methodName,
                                                               const CmpiArgs &in, CmpiArgs &out)
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() called with method: %s",_ClassName, methodName));

  if( ::strcasecmp( ref.getClassName().charPtr(), _ClassName) != 0 )
  {
    return CmpiStatus(CMPI_RC_ERR_NOT_SUPPORTED, "invalid classname");
  }
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);

  if( ::strcasecmp( methodName, "RequestStateChange" ) == 0)
  {
    const char *lsccn = ref.getKey("SystemCreationClassName");
    const char *lsn   = ref.getKey("SystemName");
    const char *lccn  = ref.getKey("CreationClassName");
    const char *ln    = ref.getKey("Name");

    CmpiObjectPath csop = get_this_computersystem(*broker, ctx, ref);
    const char *sccn = csop.getKey("CreationClassName");
    const char *sn   = csop.getKey("Name");

    if( lsccn== NULL || lsn == NULL || lccn == NULL || ln == NULL ||
        ::strcmp(lccn, _ClassName) != 0 ||
        ::strcmp(lsccn, sccn) != 0 || ::strcmp(lsn, sn) != 0)
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, "Could not find this instance." );
      _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    CMPIUint16 reqState = 0;
    try
    {
      reqState = in.getArg("RequestedState");
    }
    catch(CmpiStatus rc)
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter RequestedState." );
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
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not list software identities: " ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() failed : %s", _ClassName, rc.msg()));
      return rc;
    }
    bool newStateEnabled = true;

    if( reqState == 2 )
      newStateEnabled = true;
    else if( reqState == 3 )
      newStateEnabled = false;
    else
    {
      _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : Invalid Parameter.", _ClassName ));
      rslt.returnData( CMPIUint32(5) ); // invalid Parameter
      rslt.returnDone();
      return CmpiStatus ( CMPI_RC_OK );
    }

    RepoManager repoManager( zyppac->getSysRoot() );
    if( ! repoManager.hasRepo( ln ) )
    {
      _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() failed : Could not find this instance.", _ClassName ));
      rslt.returnData( CMPIUint32(2) ); // unknown Error
      rslt.returnDone();
      return CmpiStatus ( CMPI_RC_OK );
    }
    try
    {
      RepoInfo repo( repoManager.getRepo( ln ) );
      repo.setEnabled( newStateEnabled );
      repoManager.modifyRepository( repo );
    }
    catch( const zypp::Exception & err )
    {
      _CMPIZYPP_TRACE(1,("--- %s CMPI InvokeMethod() failed : %s ", _ClassName,
                                                                    ZyppAC::exceptionString( err, "" ).c_str() ));
      rslt.returnData( CMPIUint32(2) ); // unknown Error
      rslt.returnDone();
      return CmpiStatus ( CMPI_RC_OK );
    }
    rslt.returnData( CmpiData(CMPIUint32(0)) ); // Completed with No Error
    rslt.returnDone();
    return CmpiStatus ( CMPI_RC_OK );
  }

  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() exited",_ClassName));
  return st;
}

/* ========================= private ================================== */

CmpiInstance SUSE_SoftwareRepositoryProviderClass::makeInstance( const RepoInfo &repo,
                                                                 const CmpiObjectPath & cop,
                                                                 const CmpiObjectPath & csop,
                                                                 const char ** properties )
{
  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );

  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  const char * keys[] = { "SystemCreationClassName", "SystemName", "CreationClassName", "Name" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "SystemCreationClassName", sccn);
  ci.setProperty( "SystemName", sn);
  ci.setProperty( "CreationClassName", _ClassName);
  ci.setProperty( "Name", repo.alias().c_str() );

  ci.setProperty( "AccessInfo", repo.url().asString().c_str() );

  CmpiArray avReqState( 2, CMPI_uint16 );
  avReqState[0] = CMPIUint16(2); // Enabled
  avReqState[1] = CMPIUint16(3); // Disabled
  ci.setProperty( "AvailableRequestedStates", avReqState );

  ci.setProperty( "Caption", "Software Repository" );
  ci.setProperty( "Description", (repo.name() + " Software Repository").c_str() );
  ci.setProperty( "ElementName", repo.name().c_str() );
  ci.setProperty( "EnabledDefault", CMPIUint16(7) );
  ci.setProperty( "EnabledState", (repo.enabled())?CMPIUint16(2):CMPIUint16(3) );
  ci.setProperty( "ExtendedResourceType", CMPIUint16(2) );

  return ci;
}

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareRepositoryProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareRepositoryProviderClass, SUSE_SoftwareRepositoryProvider );
CMMethodMIFactory( cmpizypp::SUSE_SoftwareRepositoryProviderClass, SUSE_SoftwareRepositoryProvider );