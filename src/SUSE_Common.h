#ifndef SUSE_COMMON_H
#define SUSE_COMMON_H

#include <iosfwd>
#include "SUSE_AssocFilter.h"

using std::endl;

class CmpiObjectPath;
std::ostream & operator<<( std::ostream & str, const CmpiObjectPath & obj );
std::ostream & operator<<( std::ostream & str, const CmpiInstance & ci );

namespace cmpizypp
{

  CmpiObjectPath get_this_computersystem(CmpiBroker & broker,
                                         const CmpiContext& ctx,
					 const CmpiObjectPath & cop);

  bool assoc_create_refs_1toN_ST( CmpiBroker & broker,
                                  const CmpiContext & ctx,
                                  CmpiResult & rslt,
                                  const CmpiObjectPath & cop,
                                  const char * _ClassName,
                                  const char * _RefSource,
                                  const char * _RefTarget,
                                  const char * _RefSourceClass,
                                  const char * _RefTargetClass,
                                  int inst,
                                  int associators,
			          const SUSE_AssocFilter &filter = SUSE_AssocFilter() );

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
                               int associators,
			       const SUSE_AssocFilter &filter = SUSE_AssocFilter() );


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

  bool assoc_create_inst_1toN( CmpiBroker & broker,
                               const CmpiContext & ctx,
                               CmpiResult & rslt,
                               const CmpiObjectPath & cop,
                               const char * _ClassName,
                               const char * _RefLeft,
                               const char * _RefRight,
                               const char * _RefLeftClass,
                               const char * _RefRightClass,
                               int left,
                               int inst,
			       const SUSE_AssocFilter &filter = SUSE_AssocFilter() );

  CmpiInstance assoc_get_inst( CmpiBroker & broker,
                               const CmpiContext & ctx,
                               const CmpiObjectPath & cop,
                               const char * _ClassName,
                               const char * _RefLeft,
                               const char * _RefRight);

}
#endif // SUSE_COMMON_H