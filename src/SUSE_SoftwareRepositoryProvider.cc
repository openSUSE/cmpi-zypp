
#include <iostream>

#include <cmpi/cmpimacs.h>
#include <cmpi/cmpidt.h>
#include <cmpi/CmpiResult.h>
#include <cmpi/CmpiBroker.h>
#include <cmpi/CmpiArray.h>
#include <cmpi/CmpiBooleanData.h>

#include <zypp/base/String.h>
#include <zypp/base/LogTools.h>
#include <zypp/repo/RepoType.h>

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

  RepoManager repoManager( zyppac->repoManager() );

  RepoInfoList repos = repoManager.knownRepositories();
  for ( RepoInfoList::iterator it = repos.begin(); it != repos.end(); ++it )
  {
    rslt.returnData( makeObjectPath( *it, cop, csop ) );
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

  RepoManager repoManager( zyppac->repoManager() );

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

  RepoManager repoManager( zyppac->repoManager() );
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

CmpiStatus SUSE_SoftwareRepositoryProviderClass::setInstance (const CmpiContext &ctx, CmpiResult &rslt,
                                                              const CmpiObjectPath &cop, const CmpiInstance &inst,
                                                              const char **properties)
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() called.", _ClassName));

  CmpiInstance orig( broker->getInstance(ctx, cop, NULL ) );

  const char *oldsccn = orig.getProperty("SystemCreationClassName");
  const char *oldsn   = orig.getProperty("SystemName");
  const char *oldccn  = orig.getProperty("CreationClassName");
  const char *oldn    = orig.getProperty("Name");

  const char *newsccn = inst.getProperty("SystemCreationClassName");
  const char *newsn   = inst.getProperty("SystemName");
  const char *newccn  = inst.getProperty("CreationClassName");
  const char *newn    = inst.getProperty("Name");

  if( newsccn== NULL || newsn == NULL || newccn == NULL || newn == NULL ||
      oldsccn== NULL || oldsn == NULL || oldccn == NULL || oldn == NULL ||
      ::strcmp(newccn, oldccn) != 0 ||
      ::strcmp(newsccn, oldsccn) != 0 || ::strcmp(newsn, oldsn) != 0)
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Invalid Instance." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }
  if( ::strcmp(newn, oldn) != 0 )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Name cannot be changed." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() failed : %s", _ClassName, rc.msg()));
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
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize libzypp: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoManager repoManager( zyppac->repoManager() );
  // Check, if repo exists
  if( ! repoManager.hasRepo( newn ) )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Instance do not exist." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoInfo repoinfo( repoManager.getRepo( newn ) );
  setRepoInfo( repoinfo, inst );

  try
  {
    repoManager.modifyRepository( repoinfo );
  }
  catch( const zypp::Exception & err )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Building repository info data failed: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  CmpiInstance newci( broker->getInstance( ctx, cop, NULL ) );

  _CMPIZYPP_TRACE(1,("--- %s CMPI setInstance() exited.", _ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareRepositoryProviderClass::deleteInstance (const CmpiContext &ctx, CmpiResult &rslt,
                                                                 const CmpiObjectPath &cop)
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI deleteInstance() called.", _ClassName));

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  const char *newsccn = cop.getKey("SystemCreationClassName");
  const char *newsn   = cop.getKey("SystemName");
  const char *newccn  = cop.getKey("CreationClassName");
  const char *newn    = cop.getKey("Name");

  if( newsccn== NULL || newsn == NULL || newccn == NULL || newn == NULL ||
      ::strcmp(newccn, _ClassName) != 0 ||
      ::strcmp(newsccn, sccn) != 0 || ::strcmp(newsn, sn) != 0)
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Invalid Instance." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI deleteInstance() failed : %s", _ClassName, rc.msg()));
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
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI deleteInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoManager repoManager( zyppac->repoManager() );
  // Check, if repo exists
  if( ! repoManager.hasRepo( newn ) )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Instance does not exist." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI deleteInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoInfo repoinfo( repoManager.getRepo( newn ) );

  try
  {
    repoManager.removeRepository( repoinfo );
  }
  catch( const zypp::Exception & err )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Removing repository failed: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI deleteInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  _CMPIZYPP_TRACE(1,("--- %s CMPI deleteInstance() exited.", _ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareRepositoryProviderClass::createInstance (const CmpiContext &ctx, CmpiResult &rslt,
                                                                 const CmpiObjectPath &cop, const CmpiInstance &inst)
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() called.", _ClassName));

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, cop);
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  //const char *ns = cop.getNameSpace().charPtr();
  CmpiObjectPath newcop = inst.getObjectPath();

  const char *newsccn = newcop.getKey("SystemCreationClassName");
  const char *newsn   = newcop.getKey("SystemName");
  const char *newccn = newcop.getKey("CreationClassName");
  const char *newn   = newcop.getKey("Name");

  if( newsccn== NULL || newsn == NULL || newccn == NULL || newn == NULL ||
      ::strcmp(newccn, _ClassName) != 0 ||
      ::strcmp(newsccn, sccn) != 0 || ::strcmp(newsn, sn) != 0)
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Invalid Instance." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
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
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoManager repoManager( zyppac->repoManager() );
  // Check, if repo exists
  if( repoManager.hasRepo( newn ) )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Instance already exists." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  RepoInfo repoinfo;
  repoinfo.setAlias( newn );

  setRepoInfo( repoinfo, inst );

  try
  {
    repoManager.addRepository( repoinfo );
  }
  catch( const zypp::Exception & err )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Building repository info data failed: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    return rc;
  }

  CmpiInstance newci( broker->getInstance( ctx, newcop, NULL ) );

  rslt.returnData( newci.getObjectPath() );
  rslt.returnDone();

  _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() exited.", _ClassName));
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
    st = requestStateChange(ctx, rslt, ref, in, out);
  }
  else if( ::strcasecmp( methodName, "Refresh" ) == 0)
  {
    st = refresh(ctx, rslt, ref, in, out);
  }

  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() exited",_ClassName));
  return st;
}

CmpiObjectPath SUSE_SoftwareRepositoryProviderClass::makeObjectPath(const zypp::RepoInfo &repo, const CmpiObjectPath &cop,
                                                                    const CmpiObjectPath &csop)
{
  const char *sccn = csop.getKey("CreationClassName");
  const char *sn   = csop.getKey("Name");

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  op.setKey( "SystemCreationClassName", sccn);
  op.setKey( "SystemName", sn);
  op.setKey( "CreationClassName", _ClassName);
  op.setKey( "Name", repo.alias().c_str() );

  return op;
}


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

  ci.setProperty( "Autorefresh", (repo.autorefresh())?CmpiTrue:CmpiFalse );
  ci.setProperty( "SignatureCheck", (repo.gpgCheck())?CmpiTrue:CmpiFalse );
  ci.setProperty( "Priority", CMPIUint16(repo.priority()) );

  switch( repo.type().toEnum() )
  {
  case repo::RepoType::RPMMD_e:
    ci.setProperty( "RepositoryType", CMPIUint16(2) );
    break;
  case repo::RepoType::YAST2_e:
    ci.setProperty( "RepositoryType", CMPIUint16(3) );
    break;
  case repo::RepoType::RPMPLAINDIR_e:
    ci.setProperty( "RepositoryType", CMPIUint16(4) );
    break;
  case repo::RepoType::NONE_e:
    ci.setProperty( "RepositoryType", CMPIUint16(0) );
    break;
  default:
    ci.setProperty( "RepositoryType", CMPIUint16(1) );
    break;
  }
  return ci;
}

/* ========================= private ================================== */

void SUSE_SoftwareRepositoryProviderClass::setRepoInfo( zypp::RepoInfo &repoinfo,
                                                        const CmpiInstance &inst )
{
  const char* name = inst.getProperty("ElementName");
  CMPIUint16 enabledState = inst.getProperty("EnabledState");
  bool enabled = true ;
  if( enabledState == 2)
     enabled = true;
  else if( enabledState == 3 )
    enabled = false;
  else
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Invalid EnabledState value." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    throw rc;
  }

  bool autorefresh = true;
  unsigned char ar = inst.getProperty("Autorefresh");
  if(  ar == 0 )
    autorefresh = false;

  CMPIUint16 priority = inst.getProperty("Priority");
  bool gpgcheck = true;
  unsigned char gc = inst.getProperty("SignatureCheck");
  if( gc == 0 )
    gpgcheck = false;

  if( CMPIUint16(inst.getProperty("InfoFormat")) != CMPIUint16(200) )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Invalid InfoFormat value." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    throw rc;
  }

  const char* url = inst.getProperty("AccessInfo");
  if( url == NULL )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Invalid AccessValue value." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    throw rc;
  }

  try
  {
    repoinfo.setName( name );
    repoinfo.setEnabled( enabled );
    repoinfo.setAutorefresh( autorefresh );
    repoinfo.setPriority( priority );
    repoinfo.setGpgCheck( gpgcheck );
    repoinfo.setBaseUrl( Url( url ) );
  }
  catch( const zypp::Exception & err )
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Building repository info data failed: " ).c_str() );
    _CMPIZYPP_TRACE(1,("--- %s CMPI createInstance() failed : %s", _ClassName, rc.msg()));
    throw rc;
  }
}

CmpiStatus SUSE_SoftwareRepositoryProviderClass::requestStateChange(const CmpiContext &ctx, CmpiResult &rslt,
                                                                    const CmpiObjectPath &ref,
                                                                    const CmpiArgs &in, CmpiArgs &out)
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

    RepoManager repoManager( zyppac->repoManager() );
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


CmpiStatus SUSE_SoftwareRepositoryProviderClass::refresh(const CmpiContext &ctx, CmpiResult &rslt,
                                                         const CmpiObjectPath &ref,
                                                         const CmpiArgs &in, CmpiArgs &out)
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
      _CMPIZYPP_TRACE(1,("--- %s CMPI InvokeMethod() failed : %s", _ClassName, rc.msg()));
      return rc;
    }

    CMPIUint16 policy = 0;
    try
    {
      policy = in.getArg("RefreshPolicy");
    }
    catch(CmpiStatus rc)
    {
      CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter RefreshPolicy." );
      _CMPIZYPP_TRACE(1,("--- %s CMPI InvokeMethod() failed : %s", _ClassName, rc.msg()));
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
      CmpiStatus rc( CMPI_RC_ERR_FAILED, ZyppAC::exceptionString( err, "Could not initialize zypp: " ).c_str() );
      _CMPIZYPP_TRACE(1,("--- %s CMPI InvokeMethod() failed : %s", _ClassName, rc.msg()));
      return rc;
    }
    RepoManager::RawMetadataRefreshPolicy pol = RepoManager::RefreshIfNeeded;

    switch( policy )
    {
      case 0:
        pol = RepoManager::RefreshIfNeeded;
        break;
      case 1:
        pol = RepoManager::RefreshForced;
        break;
      case 2:
        pol = RepoManager::RefreshIfNeededIgnoreDelay;
        break;
    }

    RepoManager repoManager( zyppac->repoManager() );
    if( ! repoManager.hasRepo( ln ) )
    {
      _CMPIZYPP_TRACE(1,("--- %s CMPI InvokeMethod() failed : Could not find this instance.", _ClassName ));
      rslt.returnData( CMPIUint32(1) ); // Error
      rslt.returnDone();
      return CmpiStatus ( CMPI_RC_OK );
    }
    try
    {
      RepoInfo repo( repoManager.getRepo( ln ) );

      if( pol != RepoManager::RefreshForced && !repo.enabled() )
      {
        rslt.returnData( CmpiData(CMPIUint32(0)) ); // Completed with No Error
        rslt.returnDone();
        return CmpiStatus ( CMPI_RC_OK );
      }
      if ( repoManager.isCached( repo ) )
      {
        repoManager.cleanCache( repo );
      }
      repoManager.refreshMetadata( repo, pol );
      repoManager.buildCache( repo );
      repoManager.loadFromCache( repo );
    }
    catch( const zypp::Exception & err )
    {
      _CMPIZYPP_TRACE(1,("--- %s CMPI InvokeMethod() failed : %s ", _ClassName,
                                                                    ZyppAC::exceptionString( err, "" ).c_str() ));
      rslt.returnData( CMPIUint32(1) ); // Error
      rslt.returnDone();
      return CmpiStatus ( CMPI_RC_OK );
    }
    rslt.returnData( CmpiData(CMPIUint32(0)) ); // Completed with No Error
    rslt.returnDone();
    return CmpiStatus ( CMPI_RC_OK );
}

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareRepositoryProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareRepositoryProviderClass, SUSE_SoftwareRepositoryProvider );
CMMethodMIFactory( cmpizypp::SUSE_SoftwareRepositoryProviderClass, SUSE_SoftwareRepositoryProvider );
