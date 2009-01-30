#ifndef SUSE_SOFTWAREINSTALLATIONSERVICEPROVIDER_H
#define SUSE_SOFTWAREINSTALLATIONSERVICEPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SoftwareInstallationServiceProviderClass : public CmpiInstanceMI
  {
    public:
      SUSE_SoftwareInstallationServiceProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );
  };

} // namespace cmpizypp

#endif // SUSE_SOFTWAREINSTALLATIONSERVICEPROVIDER_H
