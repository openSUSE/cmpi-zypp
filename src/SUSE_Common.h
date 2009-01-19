#ifndef SUSE_COMMON_H
#define SUSE_COMMON_H

#include <iosfwd>

using std::endl;

class CmpiObjectPath;
std::ostream & operator<<( std::ostream & str, const CmpiObjectPath & obj );

namespace cmpizypp
{

  /** Method to create CMPIInstance(s) / CMPIObjectPath(s) of association.
   *
   * Combination of int <inst> and int <associators>:
   * 0 0 -> referenceNames()
   * 1 0 -> references()
   * 0 1 -> associatorNames()
   * 1 1 -> associators()
   * \note This method returns each found CMPIInstance / CMPIObjectPath object to
   * the Object Manager (OM)
   * \return \c true on success.
   */
  bool assoc_create_refs_1toN( CmpiBroker & broker,
                               const CmpiContext & ctx,
                               CmpiResult & rslt,
                               const CmpiObjectPath & cop,
                               const char * _ClassName,
                               const char * _RefLeft,
                               const char * _RefRight,
                               const char * _RefLeftClass,
                               const char * _RefRightClass,
                               int inst,
                               int associators );


  /** Method to check the input parameter resultClass, role and resultRole
   *  submitted to the methods of the association interface.
   *
   * \return \c true if responsible.
  */
  bool assoc_check_parameter_const( const CmpiObjectPath & cop,
                                    const char * _RefLeft,
                                    const char * _RefRight,
                                    const char * _RefLeftClass,
                                    const char * _RefRightClass,
                                    const char * resultClass,
                                    const char * role,
                                    const char * resultRole );
}

#endif // SUSE_COMMON_H