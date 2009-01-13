#ifndef SUSE_ZYPP_H
#define SUSE_ZYPP_H

#include "zypp/ZYppFactory.h"
#include "zypp/ZYpp.h"
#include "zypp/TmpPath.h"
#include "zypp/Glob.h"
#include "zypp/PathInfo.h"
#include "zypp/RepoManager.h"
#include "zypp/Target.h"
#include "zypp/ResPool.h"

namespace cmpizypp
{
  /** Access to libzypp components. */
  class ZyppAC
  {
    public:
      ZyppAC();
      ~ZyppAC();

      zypp::Target &     target()    { return *zypp::getZYpp()->getTarget(); }
      zypp::ResPool      pool()      { return zypp::ResPool::instance(); }
      zypp::ResPoolProxy poolProxy() { return pool().proxy(); }
      zypp::sat::Pool    satpool()   { return zypp::sat::Pool::instance(); }

      static std::string exceptionString( const zypp::Exception & err_r, const std::string & prefix = std::string() );

      /** SoftwareIdentity::InstanceId = SUSE:<name>-<version>-<release>.<arch>@<repoalias> */
      std::string SoftwareIdentityInstanceId( const zypp::sat::Solvable & slv ) const;
      std::string SoftwareIdentityInstanceId( const zypp::PoolItem & pi ) const
      { return SoftwareIdentityInstanceId( pi.satSolvable() ); }

  };
}

#endif // touch SUSE_ZYPP_H
