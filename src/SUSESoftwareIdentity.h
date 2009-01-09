
#include <cmpi/CmpiInstanceMI.h>

namespace cmpizypp
{
  /**
  */
  class SUSE_SoftwareIdentityProvider : public CmpiInstanceMI
  {
    public:
      SUSE_SoftwareIdentityProvider( const CmpiBroker & mbp, const CmpiContext & ctx );
  };

} // namespace cmpizypp
