#ifndef SUSE_ASSOCFILTER_H
#define SUSE_ASSOCFILTER_H

#include "SUSE_zypp.h"

namespace cmpizypp
{
  class SUSE_AssocFilter
  {
    public:
      SUSE_AssocFilter() {};
      virtual ~SUSE_AssocFilter() {};

      virtual bool filterInstance(const CmpiObjectPath &scop, const char * _RefSource, const CmpiInstance &ci, bool associators) const
      {
        _CMPIZYPP_TRACE(1,("--- AssocFilter::filterInstance() called"));
        return true;
      };
      virtual bool filterObjectPath(const CmpiObjectPath &scop, const char * _RefSource, const CmpiObjectPath &op, bool associators) const
      {
        _CMPIZYPP_TRACE(1,("--- AssocFilter::filterObjectPath() called"));
        return true;
      }
  };

} // namespace cmpizypp

#endif // SUSE_ASSOCFILTER_H