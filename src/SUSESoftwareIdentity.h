
#include <cmpi/CmpiInstanceMI.h>

namespace cmpizypp
{
  /**
  */
  class SUSESoftwareIdentity : public CmpiInstanceMI
  {
    public:
      SUSESoftwareIdentity( const CmpiBroker & mbp, const CmpiContext & ctx );
  };

} // namespace cmpizypp
