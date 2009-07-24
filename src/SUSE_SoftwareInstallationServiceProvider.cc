#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
#include <fstream>

#include <cmpi/cmpimacs.h>
#include <cmpi/cmpidt.h>
#include <cmpi/CmpiResult.h>
#include <cmpi/CmpiBroker.h>
#include <cmpi/CmpiArray.h>
#include <cmpi/CmpiBooleanData.h>

#include <zypp/base/String.h>
#include <zypp/base/LogTools.h>
#include <zypp/Pathname.h>
#include <zypp/TmpPath.h>
#include <zypp/ExternalProgram.h>

#include "installHelper.h"
#include "SUSE_zypp.h"
#include "SUSE_Common.h"
#include "SUSE_SoftwareInstallationServiceProvider.h"

using namespace zypp;
using namespace boost::interprocess;
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
  , CmpiMethodMI( mbp, ctx )
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

CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::invokeMethod (const CmpiContext &ctx, CmpiResult &rslt,
                                                                        const CmpiObjectPath &ref, const char *methodName,
                                                                        const CmpiArgs &in, CmpiArgs &out)
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() called with method: %s",_ClassName, methodName));

  if( ::strcasecmp( ref.getClassName().charPtr(), _ClassName) != 0 )
  {
    return CmpiStatus(CMPI_RC_ERR_NOT_SUPPORTED, "invalid classname");
  }
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);

  if( ::strcasecmp( methodName, "InstallFromSoftwareIdentity" ) == 0)
  {
    st = this->installFromSoftwareIdentity(ctx, rslt, ref, in, out);
  }
  else if( ::strcasecmp( methodName, "InstallFromSoftwareIdentities" ) == 0)
  {
    st = this->installFromSoftwareIdentities(ctx, rslt, ref, in, out);
  }
  else if( ::strcasecmp( methodName, "RefreshAllRepositories" ) == 0)
  {
    st = this->refreshAllRepositories(ctx, rslt, ref, in, out);
  }
  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() exited",_ClassName));
  return st;
}

/* ========================= private ================================== */
CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::installFromSoftwareIdentity(const CmpiContext &ctx,
                                                                                      CmpiResult &rslt,
                                                                                      const CmpiObjectPath &ref,
                                                                                      const CmpiArgs &in, CmpiArgs &out)
{
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);
  _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() called.",_ClassName));

  CmpiArray installOptions;
  CmpiArray installOptionValues;
  CmpiObjectPath source(ref.getNameSpace(), "");
  CmpiObjectPath target(ref.getNameSpace(), "");
  CmpiObjectPath collection(ref.getNameSpace(), "");
  const char *siID = "";
  try
  {
    installOptions = static_cast<CmpiArray>(in.getArg("InstallOptions"));
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no InstallOptions", _ClassName, rc.msg()));
    /*
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter." );
    return rc;
    */
  }
  try
  {
    installOptionValues = static_cast<CmpiArray>(in.getArg("InstallOptionValues"));
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no InstallOptionValues", _ClassName, rc.msg()));
    /*
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter." );
    return rc;
    */
  }
  try
  {
    source = in.getArg("Source");

    siID = source.getKey("InstanceID");
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no Source", _ClassName, rc.msg()));
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    return st;
  }
  try
  {
    target = in.getArg("Target");
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no Target", _ClassName, rc.msg()));
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    return st;
  }
  try
  {
    collection = in.getArg("Collection");
  }
  catch(CmpiStatus rc)
  {
    // we do not support collection
  }

  if(target.getKeyCount() > 0 && collection.getKeyCount() > 0)
  {
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() exited. Unclear to which target should be installed to.",_ClassName));
    return st;
  }

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, target);

  const char *selfccn  = csop.getKey("CreationClassName");
  const char *selfn    = csop.getKey("Name");

  const char *tccn  = target.getKey("CreationClassName");
  const char *tn    = target.getKey("Name");

  if( tccn == NULL || tn == NULL || selfccn == NULL || selfn == NULL ||
    ::strcmp(tccn, selfccn) != 0 || ::strcmp(tn, selfn) != 0)
  {
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() exited. Unable to install on this target.",_ClassName));
    return st;
  }

  CmpiArray swIdentities( 1, CMPI_string );
  swIdentities[0] = CmpiString(siID);

  CmpiInstance job(CmpiObjectPath(target.getNameSpace(), "SUSE_SoftwareInstallationJob"));
  createJob(ctx, swIdentities, installOptions, job);

  out.setArg("Job", CmpiData(job.getObjectPath()));

  rslt.returnData( CmpiData(CMPIUint32(4096)) ); //Job Started
  rslt.returnDone();
  st =  CmpiStatus ( CMPI_RC_OK );
  _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() exited.",_ClassName));
  return st;
}

CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::installFromSoftwareIdentities(const CmpiContext &ctx,
                                                                                        CmpiResult &rslt,
                                                                                        const CmpiObjectPath &ref,
                                                                                        const CmpiArgs &in, CmpiArgs &out)
{
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);
  _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() called.",_ClassName));

  CmpiArray installOptions;
  CmpiArray installOptionValues;
  CmpiArray source;
  CmpiObjectPath target(ref.getNameSpace(), "");
  CmpiObjectPath collection(ref.getNameSpace(), "");
  const char *siID = "";
  try
  {
    installOptions = static_cast<CmpiArray>(in.getArg("InstallOptions"));
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no InstallOptions", _ClassName, rc.msg()));
    /*
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter." );
    return rc;
    */
  }
  try
  {
    installOptionValues = static_cast<CmpiArray>(in.getArg("InstallOptionValues"));
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no InstallOptionValues", _ClassName, rc.msg()));
    /*
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter." );
    return rc;
    */
  }
  try
  {
    source = static_cast<CmpiArray>(in.getArg("Source"));
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no Source", _ClassName, rc.msg()));
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    return st;
  }
  try
  {
    target = in.getArg("Target");
  }
  catch(CmpiStatus rc)
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() failed : %s no Target", _ClassName, rc.msg()));
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    return st;
  }
  try
  {
    collection = in.getArg("Collection");
  }
  catch(CmpiStatus rc)
  {
    // we do not support collection
  }

  if(target.getKeyCount() > 0 && collection.getKeyCount() > 0)
  {
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() exited. Unclear to which target should be installed to.",_ClassName));
    return st;
  }

  CmpiObjectPath csop = get_this_computersystem(*broker, ctx, target);

  const char *selfccn  = csop.getKey("CreationClassName");
  const char *selfn    = csop.getKey("Name");

  const char *tccn  = target.getKey("CreationClassName");
  const char *tn    = target.getKey("Name");

  if( tccn == NULL || tn == NULL || selfccn == NULL || selfn == NULL ||
    ::strcmp(tccn, selfccn) != 0 || ::strcmp(tn, selfn) != 0)
  {
    rslt.returnData( CmpiData(CMPIUint32(2)) ); // Error
    rslt.returnDone();
    st =  CmpiStatus ( CMPI_RC_OK );
    _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() exited. Unable to install on this target.",_ClassName));
    return st;
  }

  CmpiArray swIdentities( source.size(), CMPI_string );
  for(uint j = 0; j < source.size(); ++j)
  {
    CmpiObjectPath tmp = source[j];
    siID = tmp.getKey("InstanceID");
    swIdentities[j] = CmpiString(siID);
  }

  CmpiInstance job(CmpiObjectPath(target.getNameSpace(), "SUSE_SoftwareInstallationJob"));
  createJob(ctx, swIdentities, installOptions, job);

  out.setArg("Job", CmpiData(job.getObjectPath()));

  rslt.returnData( CmpiData(CMPIUint32(4096)) ); //Job Started
  rslt.returnDone();
  st =  CmpiStatus ( CMPI_RC_OK );
  _CMPIZYPP_TRACE(1,("--- %s CMPI installFromSoftwareIdentity() exited.",_ClassName));
  return st;
}

CmpiStatus SUSE_SoftwareInstallationServiceProviderClass::refreshAllRepositories(const CmpiContext &ctx, CmpiResult &rslt,
                                                                                 const CmpiObjectPath &ref,
                                                                                 const CmpiArgs &in, CmpiArgs &out)
{
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);
  _CMPIZYPP_TRACE(1,("--- %s CMPI refreshAllRepositories() called.",_ClassName));

  CMPIUint16 policy = 0;
  try
  {
    policy = in.getArg("RefreshPolicy");
  }
  catch(CmpiStatus rc)
  {
    CmpiStatus rc( CMPI_RC_ERR_FAILED, "Missing parameter RefreshPolicy." );
    _CMPIZYPP_TRACE(1,("--- %s CMPI refreshAllRepositories() failed : %s", _ClassName, rc.msg()));
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
    _CMPIZYPP_TRACE(1,("--- %s CMPI refreshAllRepositories() failed : %s", _ClassName, rc.msg()));
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

  try
  {
    // Load all enabled repos in repos.d to pool.
    RepoInfoList repos = repoManager.knownRepositories();
    for ( RepoInfoList::iterator it = repos.begin(); it != repos.end(); ++it )
    {

      RepoInfo & repo( *it );

      if ( ! repo.enabled() )
        continue;

      if ( repoManager.isCached( repo ) )
      {
        repoManager.cleanCache( repo );
      }
      repoManager.refreshMetadata( repo, pol );
      repoManager.buildCache( repo );
      repoManager.loadFromCache( repo );
    }
  }
  catch( const zypp::Exception & err )
  {
    _CMPIZYPP_TRACE(1,("--- %s CMPI refreshAllRepositories() failed : %s ", _ClassName,
                        ZyppAC::exceptionString( err, "" ).c_str() ));
                        rslt.returnData( CMPIUint32(1) ); // Error
                        rslt.returnDone();
                        return CmpiStatus ( CMPI_RC_OK );
  }
  rslt.returnData( CmpiData(CMPIUint32(0)) ); // Completed with No Error
  rslt.returnDone();
  st =  CmpiStatus ( CMPI_RC_OK );
  _CMPIZYPP_TRACE(1,("--- %s CMPI refreshAllRepositories() exited.",_ClassName));
  return st;
}


int SUSE_SoftwareInstallationServiceProviderClass::createJob(const CmpiContext &ctx, const CmpiArray &swIdentities,
                                                             const CmpiArray &installOptions, CmpiInstance &job)
{
  shared_memory_object::remove(SHM_NAME);

  _CMPIZYPP_TRACE(1,("Creating shared memory"));
  ::mode_t mask = ::umask( 0077 );
  managed_shared_memory managed_shm( create_only, SHM_NAME, 65536 );
  ::umask( mask );

  if ( ! managed_shm.construct<Comm>("Comm")() )
  {
    _CMPIZYPP_TRACE(1,("Out of shmem constructing Comm"));
    throw std::string( "Out of shmem constructing Comm" );
  }
  if ( ! managed_shm.find_or_construct<TextExch>("TextExch")() )
  {
    _CMPIZYPP_TRACE(1,("Out of shmem constructing TextExch"));
    throw std::string( "Out of shmem constructing TextExch" );
  }

  ShmAccess<Comm> comm( managed_shm, "Comm" ); // blocks the helper

  _CMPIZYPP_TRACE(1,("fork and execute installHelper"));
  ExternalProgram helper( "/usr/lib/cmpi-zypp/installHelper" );

  _CMPIZYPP_TRACE(1,("installHelper started (%d)", helper.getpid() ));

  comm->pid = helper.getpid();
  comm.release(); // go...

  ShmAccessUnlocked<TextExch> textExch( managed_shm, "TextExch" );
  // write task list
  for(uint i = 0; i < swIdentities.size(); ++i)
  {
    CmpiString s = swIdentities[i];
    textExch->send( s.charPtr() );
  }
  textExch->sendEOD();

  for(uint i = 0; i < installOptions.size(); ++i)
  {
    uint16_t opt = installOptions[i];
    textExch->send( opt );
  }
  textExch->sendEOD();

  CmpiObjectPath jobOP(job.getObjectPath().getNameSpace(), "SUSE_SoftwareInstallationJob");
  jobOP.setKey("InstanceID", str::form("SUSE:%d", helper.getpid()).c_str());
  job = broker->getInstance( ctx, jobOP, NULL ); // source instance

  return 0;
}

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareInstallationServiceProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareInstallationServiceProviderClass, SUSE_SoftwareInstallationServiceProvider );
CMMethodMIFactory( cmpizypp::SUSE_SoftwareInstallationServiceProviderClass, SUSE_SoftwareInstallationServiceProvider );
