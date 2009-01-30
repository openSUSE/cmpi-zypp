#ifndef SUSE_SOFTWAREINSTALLATIONSERVICECAPABILITIESPROVIDER_H
#define SUSE_SOFTWAREINSTALLATIONSERVICECAPABILITIESPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SoftwareInstallationServiceCapabilitiesProviderClass : public CmpiInstanceMI
  {
    public:
      SUSE_SoftwareInstallationServiceCapabilitiesProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );

    private:
      CmpiInstance makeInstance( const CmpiObjectPath & cop, const char ** properties );
  };

} // namespace cmpizypp

#endif // SUSE_SOFTWAREINSTALLATIONSERVICECAPABILITIESPROVIDER_H
