
#include <iostream>

#include "zypp/base/LogControl.h"
#include "zypp/base/LogTools.h"

#include "SUSE_zypp.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    int _loginit()
    {
		zypp::base::LogControl::instance().logToStdErr();
		return 1;
    }
    int loginit = _loginit();


    const Pathname sysRoot( "/Local/cimROOT" );
    bool zyppACInitialized = false;

    void zyppACInit()
    {
      if ( zyppACInitialized )
        return;

      KeyRing::setDefaultAccept( KeyRing::ACCEPT_UNKNOWNKEY|KeyRing::TRUST_KEY_TEMPORARILY );

      // Load Target
      getZYpp()->initializeTarget( sysRoot );
      //getZYpp()->target()->load();

      // check services ?
      if ( 0 )
      {
        RepoManager repoManager( sysRoot );
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
      RepoManager repoManager( sysRoot );
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

      zyppACInitialized = true;
    }

  } // namespace


  ZyppAC::ZyppAC()
  {
    zyppACInit();
  }

  ZyppAC::~ZyppAC()
  {
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
                        slv.isKind<SrcPackage>() ? "srcpackage:" : "",
                        slv.ident().c_str(),
                        slv.edition().c_str(),
                        slv.arch().c_str(),
                        slv.repository().alias().c_str() ) );
  }

}
