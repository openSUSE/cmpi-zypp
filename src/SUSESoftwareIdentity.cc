
#include "SUSESoftwareIdentity.h"

namespace cmpizypp
{

  SUSESoftwareIdentity::SUSESoftwareIdentity( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiInstanceMI( mbp, ctx )
  , CmpiBaseMI( mbp, ctx )
  {
  }

} // namespace cmpizypp
