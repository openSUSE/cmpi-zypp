#ifndef SUSE_SOFTWAREINSTALLATIONJOBPROVIDER_H
#define SUSE_SOFTWAREINSTALLATIONJOBPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>
#include <cmpi/CmpiMethodMI.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SoftwareInstallationJobProviderClass : public CmpiInstanceMI, public CmpiMethodMI
  {
    public:
      SUSE_SoftwareInstallationJobProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );

      virtual CmpiStatus invokeMethod (const CmpiContext &ctx, CmpiResult &rslt,
                                       const CmpiObjectPath &ref, const char *methodName,
                                       const CmpiArgs &in, CmpiArgs &out);

    private:
      CmpiStatus provideAnswer(const CmpiContext &ctx, CmpiResult &rslt,
                               const CmpiObjectPath &ref, const CmpiArgs &in, CmpiArgs &out);
  };

} // namespace cmpizypp

#endif // SUSE_SOFTWAREINSTALLATIONJOBPROVIDER_H
