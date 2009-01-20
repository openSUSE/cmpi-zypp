#ifndef SUSE_SYSTEMSPECIFICCOLLECTIONPROVIDER_H
#define SUSE_SYSTEMSPECIFICCOLLECTIONPROVIDER_H

#include <cmpi/CmpiInstanceMI.h>

namespace cmpizypp
{
  /**
   */
  class SUSE_SystemSpecificCollectionProviderClass : public CmpiInstanceMI
  {
    public:
      SUSE_SystemSpecificCollectionProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );
  };

} // namespace cmpizypp

#endif // SUSE_SYSTEMSPECIFICCOLLECTIONPROVIDER_H
