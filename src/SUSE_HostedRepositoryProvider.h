#ifndef SUSE_HOSTEDREPOSITORYPROVIDERCLASS_H
#define SUSE_HOSTEDREPOSITORYPROVIDERCLASS_H

#include <cmpi/CmpiInstanceMI.h>
#include <cmpi/CmpiAssociationMI.h>

#include "SUSE_AssocFilter.h"

namespace cmpizypp
{
  /**
  */
  class SUSE_HostedRepositoryProviderClass : public CmpiInstanceMI, public CmpiAssociationMI
  {
    public:
      SUSE_HostedRepositoryProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

      virtual CmpiStatus enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop );
      virtual CmpiStatus enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties );
      virtual CmpiStatus getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties );

      virtual CmpiStatus associators
          (const CmpiContext& ctx, CmpiResult& rslt,
           const CmpiObjectPath& cop, const char* assocClass, const char* resultClass,
           const char* role, const char* resultRole, const char** properties);

      virtual CmpiStatus associatorNames
          (const CmpiContext& ctx, CmpiResult& rslt,
           const CmpiObjectPath& cop, const char* assocClass, const char* resultClass,
           const char* role, const char* resultRole);

      virtual CmpiStatus references
          (const CmpiContext& ctx, CmpiResult& rslt,
           const CmpiObjectPath& cop, const char* resultClass, const char* role,
           const char** properties);

      virtual CmpiStatus referenceNames
          (const CmpiContext& ctx, CmpiResult& rslt,
           const CmpiObjectPath& cop, const char* resultClass, const char* role);
  };


  class SUSE_HostedRepositoryFilter: public SUSE_AssocFilter
  {
    private:
      CmpiObjectPath csop;

    public:
      SUSE_HostedRepositoryFilter(const CmpiObjectPath &op);
      virtual ~SUSE_HostedRepositoryFilter() {};

      virtual bool filterInstance(const CmpiObjectPath &scop, const char * _RefSource, const CmpiInstance &ci, bool associators) const;
      virtual bool filterObjectPath(const CmpiObjectPath &scop, const char * _RefSource, const CmpiObjectPath &op, bool associators) const;
  };

} // namespace cmpizypp

#endif // SUSE_HOSTEDREPOSITORYPROVIDERCLASS_H
