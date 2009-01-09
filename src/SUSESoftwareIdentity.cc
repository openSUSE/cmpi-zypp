
#include "SUSESoftwareIdentity.h"
#include <cmpi/cmpimacs.h>

namespace cmpizypp
{

  SUSE_SoftwareIdentityProvider::SUSE_SoftwareIdentityProvider( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  {
  }

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareIdentity );
CMInstanceMIFactory( cmpizypp::SUSE_SoftwareIdentityProvider, SUSE_SoftwareIdentity );
