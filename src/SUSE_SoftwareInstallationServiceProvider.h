#ifndef SUSE_SOFTWAREINSTALLATIONSERVICEPROVIDER_H
#define SUSE_SOFTWAREINSTALLATIONSERVICEPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>
#include <cmpi/CmpiMethodMI.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SoftwareInstallationServiceProviderClass : public CmpiInstanceMI, public CmpiMethodMI
  {
    public:
      SUSE_SoftwareInstallationServiceProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );

      virtual CmpiStatus invokeMethod (const CmpiContext &ctx, CmpiResult &rslt,
                                       const CmpiObjectPath &ref, const char *methodName,
                                       const CmpiArgs &in, CmpiArgs &out);

    private:
      CmpiStatus installFromSoftwareIdentity(const CmpiContext &ctx, CmpiResult &rslt,
                                             const CmpiObjectPath &ref, const CmpiArgs &in, CmpiArgs &out);
      CmpiStatus installFromSoftwareIdentities(const CmpiContext &ctx, CmpiResult &rslt,
                                               const CmpiObjectPath &ref, const CmpiArgs &in, CmpiArgs &out);
      CmpiStatus refreshAllRepositories(const CmpiContext &ctx, CmpiResult &rslt,
                                        const CmpiObjectPath &ref, const CmpiArgs &in, CmpiArgs &out);

      int createJob(const CmpiContext &ctx, const CmpiArray &swIdentities,
                    const CmpiArray &installOptions, CmpiInstance &job);
  };

} // namespace cmpizypp

#endif // SUSE_SOFTWAREINSTALLATIONSERVICEPROVIDER_H
