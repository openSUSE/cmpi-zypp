#ifndef SUSE_INSTALLATIONSERVICEAFFECTSSOFTWAREIDENTITYPROVIDERCLASS_H
#define SUSE_INSTALLATIONSERVICEAFFECTSSOFTWAREIDENTITYPROVIDERCLASS_H

#include <cmpi/CmpiInstanceMI.h>
#include <cmpi/CmpiAssociationMI.h>

namespace cmpizypp
{
  /**
  */
  class SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass : public CmpiInstanceMI, public CmpiAssociationMI
  {
    public:
      SUSE_InstallationServiceAffectsSoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx );

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

} // namespace cmpizypp

#endif // SUSE_INSTALLATIONSERVICEAFFECTSSOFTWAREIDENTITYPROVIDERCLASS_H
