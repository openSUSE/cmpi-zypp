#ifndef SUSE_SOFTWAREREPOSITORYPROVIDER_H
#define SUSE_SOFTWAREREPOSITORYPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>
#include <cmpi/CmpiMethodMI.h>
#include <zypp/RepoInfo.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SoftwareRepositoryProviderClass : public CmpiInstanceMI, public CmpiMethodMI
  {
    public:
      SUSE_SoftwareRepositoryProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );

      virtual CmpiStatus invokeMethod (const CmpiContext &ctx, CmpiResult &rslt,
                                       const CmpiObjectPath &ref, const char *methodName,
                                       const CmpiArgs &in, CmpiArgs &out);

    private:
      CmpiInstance makeInstance( const zypp::RepoInfo &repo, const CmpiObjectPath &cop,
                                 const CmpiObjectPath &csop, const char ** properties );
  };

} // namespace cmpizypp

#endif // SUSE_SOFTWAREREPOSITORYPROVIDER_H