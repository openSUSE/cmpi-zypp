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

#define _CMPIZYPP_TRACE( LV, FORMAT ) USR << "[" << LV << "] " << str::form FORMAT << endl;

namespace cmpizypp
{
  /** Access to libzypp components. */
  class ZyppAC
  {
    public:
      ZyppAC();
      ~ZyppAC();

      zypp::Target &     target()    const { return *zypp::getZYpp()->getTarget(); }
      zypp::ResPool      pool()      const { return zypp::ResPool::instance(); }
      zypp::ResPoolProxy poolProxy() const { return pool().proxy(); }
      zypp::sat::Pool    satpool()   const { return zypp::sat::Pool::instance(); }

      static std::string exceptionString( const zypp::Exception & err_r, const std::string & prefix = std::string() );

      /** SoftwareIdentity::InstanceId = SUSE:<name>-<version>-<release>.<arch>@<repoalias> */
      std::string SoftwareIdentityInstanceId( const zypp::sat::Solvable & slv ) const;
      std::string SoftwareIdentityInstanceId( const zypp::PoolItem & pi ) const
      { return SoftwareIdentityInstanceId( pi.satSolvable() ); }
      /** \see \ref SoftwareIdentityInstanceId */
      zypp::PoolItem findSoftwareIdentityInstanceId( const std::string & id_r ) const;

  };
}

#endif // touch SUSE_ZYPP_H
