
#include "SUSE_SoftwareIdentityProvider.h"
#include <cmpi/cmpimacs.h>

namespace cmpizypp
{

  SUSE_SoftwareIdentityProviderClass::SUSE_SoftwareIdentityProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  {
  }

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareIdentityProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareIdentityProviderClass, SUSE_SoftwareIdentityProvider );
