#ifndef SUSE_SOFTWAREIDENTITYPROVIDER_H
#define SUSE_SOFTWAREIDENTITYPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>
#include <zypp/PoolItem.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SoftwareIdentityProviderClass : public CmpiInstanceMI
  {
    public:
      SUSE_SoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );

      static CmpiObjectPath makeSoftwareIdentityOP( const zypp::PoolItem & pi, ZyppAC & zyppac, const CmpiObjectPath & cop);
      static CmpiInstance makeSoftwareIdentityInstance( const zypp::PoolItem & pi, ZyppAC & zyppac, const CmpiObjectPath & cop, const char** properties );

  };

} // namespace cmpizypp

#endif // SUSE_SOFTWAREIDENTITYPROVIDER_H
