#ifndef SUSE_HOSTEDINSTALLATIONSERVICEPROVIDERCLASS_H
#define SUSE_HOSTEDINSTALLATIONSERVICEPROVIDERCLASS_H

#include <cmpi/CmpiInstanceMI.h>
#include <cmpi/CmpiAssociationMI.h>

#include "SUSE_AssocFilter.h"

namespace cmpizypp
{
  /**
  */
  class SUSE_HostedInstallationServiceProviderClass : public CmpiInstanceMI, public CmpiAssociationMI
  {
    public:
      SUSE_HostedInstallationServiceProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

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


  class SUSE_HostedInstallationServiceFilter: public SUSE_AssocFilter
  {
    private:
      CmpiObjectPath csop;

    public:
      SUSE_HostedInstallationServiceFilter(const CmpiObjectPath &op);
      virtual ~SUSE_HostedInstallationServiceFilter() {};

      virtual bool filterInstance(const CmpiObjectPath &scop, const char * _RefSource, const CmpiInstance &ci, bool associators) const;
      virtual bool filterObjectPath(const CmpiObjectPath &scop, const char * _RefSource, const CmpiObjectPath &op, bool associators) const;
  };

} // namespace cmpizypp

#endif // SUSE_HOSTEDINSTALLATIONSERVICEROVIDERCLASS_H
