
#include <iostream>
#include <boost/thread.hpp>

#include "zypp/base/LogControl.h"
#include "zypp/base/LogTools.h"

#include "SUSE_zypp.h"

#undef  ZYPP_BASE_LOGGER_LOGGROUP
#define ZYPP_BASE_LOGGER_LOGGROUP "zyppAC"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    void zyppACInit()
    {
      static bool zyppACInitialized = false;
      if ( zyppACInitialized )
        return;

      Pathname sysRoot( ZyppAC::getSysRoot() );

      KeyRing::setDefaultAccept( KeyRing::ACCEPT_UNKNOWNKEY|KeyRing::TRUST_KEY_TEMPORARILY );

      // Load Target
      getZYpp()->initializeTarget( sysRoot );
      getZYpp()->target()->load();

      // Load repos
      RepoManager repoManager( sysRoot );

      // check services ?
      if ( 0 )
      {
        ServiceInfoList services = repoManager.knownServices();

        for ( ServiceInfoList::iterator it = services.begin(); it != services.end(); ++it )
        {
          ServiceInfo & nservice( *it );
          SEC << nservice << endl;

          if ( ! nservice.enabled() )
            continue;

          repoManager.refreshService( nservice );
        }
      }

      // Load all enabled repos in repos.d to pool.
      RepoInfoList repos = repoManager.knownRepositories();
      for ( RepoInfoList::iterator it = repos.begin(); it != repos.end(); ++it )
      {
        RepoInfo & nrepo( *it );
        USR << nrepo << endl;

        if ( ! nrepo.enabled() )
          continue;

        if ( ! repoManager.isCached( nrepo ) || nrepo.type() == repo::RepoType::RPMPLAINDIR )
        {
          if ( repoManager.isCached( nrepo ) )
          {
            USR << "cleanCache" << endl;
            repoManager.cleanCache( nrepo );
          }
          //USR << "refreshMetadata" << endl;
          //repoManager.refreshMetadata( nrepo );
          USR << "buildCache" << endl;
          repoManager.buildCache( nrepo );
        }
        USR << "Create from cache" << endl;
        repoManager.loadFromCache( nrepo );
      }

      ResPool   pool( ResPool::instance() );
      sat::Pool satpool( sat::Pool::instance() );

      dumpRange( USR << "Repos: ", satpool.reposBegin(), satpool.reposEnd() ) << endl;
      USR << "pool: " << pool << endl;

      zyppACInitialized = true;
    }
  } // namespace

  zypp::Pathname ZyppAC::getSysRoot()
  {
    static zypp::Pathname _sysRoot( getenv("CMPIZYPP_ROOT") ? getenv("CMPIZYPP_ROOT") : "/" );
    return _sysRoot;
  }

  ZyppAC::ZyppAC()
  {
    //INT << "+++Lock " << this << endl;
    zyppACInit();
  }

  ZyppAC::~ZyppAC()
  {
    //MIL << "---Lock " << this << endl;
  }

  std::string ZyppAC::exceptionString( const Exception & err_r, const std::string & prefix_r )
  {
    if ( err_r.historyEmpty() )
      return prefix_r+err_r.asUserString();

    std::string ret( err_r.asUserString() );
    if ( ret.empty() )
      return prefix_r+err_r.historyAsString();

    ret += '\n';
    ret += err_r.historyAsString();
    return prefix_r+ret;
  }


  std::string ZyppAC::SoftwareIdentityInstanceId( const zypp::sat::Solvable & slv ) const
  {
    return( str::form( "SUSE:%s%s-%s.%s@%s",
                        slv.isKind<SrcPackage>() ? ResKind::srcpackage.c_str() : "",
                        slv.ident().c_str(),
                        slv.edition().c_str(),
                        slv.arch().c_str(),
                        slv.repository().alias().c_str() ) );
  }

  PoolItem ZyppAC::findSoftwareIdentityInstanceId( const C_Str & id_r ) const
  {
    // SoftwareIdentity::InstanceId = SUSE:<name>-<version>-<release>.<arch>@<repoalias>
    const std::string id( id_r );

    if ( ! str::hasPrefix( id, "SUSE:" ) )
      return PoolItem();

    // check repo
    std::string::size_type rdelim( id.find( "@" ) );
    if ( rdelim == std::string::npos )
      return PoolItem();

    Repository repo( sat::Pool::instance().reposFind( id.substr( rdelim+1) ) );
    if ( ! repo )
      return PoolItem();

    // check n-v-r.a from behind
    std::string::size_type delim = id.rfind( ".", rdelim );
    if ( delim == std::string::npos )
      return PoolItem();

    Arch arch( id.substr( delim+1, rdelim-delim-1 ) );

    // v-r starts at one but last '-'
    rdelim = delim;
    delim = id.rfind( "-", rdelim );
    if ( delim == std::string::npos )
      return PoolItem();
    delim = id.rfind( "-", delim-1 );
    if ( delim == std::string::npos )
      return PoolItem();

    Edition ed( id.substr( delim+1, rdelim-delim-1 ) );

    // eveythig before is name (except the leading "SUSE:")
    std::string identstring( id.substr( 5, delim-5 ) );

    DBG << "Lookup:" << endl;
    DBG << identstring << endl;
    DBG << ed << endl;
    DBG << arch << endl;
    DBG << repo << endl;

    // now lookup in pool..
    sat::Solvable::SplitIdent ident( (IdString(identstring)) );
    for_( it, pool().byIdentBegin( ident.kind(), ident.name() ), pool().byIdentEnd( ident.kind(), ident.name() ) )
    {
      sat::Solvable solv( (*it).satSolvable() );
      if ( solv.repository() == repo && solv.arch() == arch && solv.edition() == ed )
      {
        return *it;
      }
    }

    return PoolItem();
  }

  bool ZyppAC::isSystemSoftwareIdentityInstanceId( const  C_Str & id_r )
  {
    return str::hasSuffix( id_r, Repository::systemRepoAlias() );
  }

}
