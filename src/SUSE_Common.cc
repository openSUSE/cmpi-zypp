
#include <cmpi/CmpiObjectPath.h>
#include <zypp/base/LogTools.h>
#include <zypp/base/String.h>
#include <zypp/base/PtrTypes.h>

#include <cmpi/CmpiResult.h>

#include "SUSE_Common.h"
#include "SUSE_zypp.h"

using namespace zypp;

std::ostream & operator<<( std::ostream & str, const CmpiObjectPath & obj )
{
  str << obj.toString().charPtr() << " " << obj.getKeyCount() << " Keys";
  return str;
}

namespace cmpizypp
{
  /* ---------------------------------------------------------------------------*/
  /*                      _assoc_targetClass_Name()                             */
  /* ---------------------------------------------------------------------------*/
  /*                  method to get the name of the target class                */
  /*                                                                            */
  /* return value NULL : association is not responsible for this request        */
  /* ---------------------------------------------------------------------------*/

  const char * _assoc_targetClass_Name( const CmpiObjectPath & cop,
                                        const char * _RefLeftClass,
                                        const char * _RefRightClass )
  {
    _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_Name() called"));

    CmpiString sourceClass( cop.getClassName() );

    if ( sourceClass.charPtr() == NULL )
    {
      _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_Name() failed : Could not get classname of source object path."));
      return NULL;
    }
    _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_Name() : source class %s",sourceClass.charPtr()));

    if ( cop.classPathIsA( _RefLeftClass ) )
    {
      /* pathName = left end -> get right end */
      _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_Name() exited : %s",_RefRightClass));
      return _RefRightClass;
    }

    if ( cop.classPathIsA( _RefRightClass ) )
    {
      /* pathName = right end -> get left end */
      _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_Name() exited : %s",_RefLeftClass));
      return _RefLeftClass;
    }

    _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_Name() exited : no target class found"));
    return NULL;
  }


  /* ---------------------------------------------------------------------------*/
  /*                       _assoc_targetClass_OP()                              */
  /* ---------------------------------------------------------------------------*/
  /*        method to create an empty CMPIObjectPath of the target class        */
  /*                                                                            */
  /* return value NULL : association is not responsible for this request        */
  /* ---------------------------------------------------------------------------*/

  CmpiObjectPath _assoc_targetClass_OP( const CmpiObjectPath & cop,
                                        const char * _RefLeftClass,
                                        const char * _RefRightClass )
  {
    _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_OP() called"));

    const char * targetName = _assoc_targetClass_Name( cop, _RefLeftClass, _RefRightClass );

    if( targetName == NULL )
    {
      _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_OP() failed"));
      throw( CmpiStatus( CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed." ) );
    }

    _CMPIZYPP_TRACE(4,("--- _assoc_targetClass_OP() exited"));
    return CmpiObjectPath( cop.getNameSpace(), targetName );
  }



  /* ---------------------------------------------------------------------------*/
  /*                      _assoc_create_refs_1toN()                             */
  /* ---------------------------------------------------------------------------*/
  /* method to return CMPIInstance(s) / CMPIObjectPath(s) of related objects    */
  /* and the association itself                                                 */
  /*                                                                            */
  /* combination of int <inst> and int <associators> :
  * 0 0 -> referenceNames()
  * 1 0 -> references()
  * 0 1 -> associatorNames()
  * 1 1 -> associators()
  */
  /* return value : SUCCESS = 0 ; FAILED = -1                                   */
  /* !!! this method returns each found CMPIInstance / CMPIObjectPath object to */
  /* the Object Manager (OM)                                                    */
  /* ---------------------------------------------------------------------------*/
  bool _assoc_create_refs_1toN( CmpiBroker & broker,
                                const CmpiContext & ctx,
                                CmpiResult & rslt,
                                const CmpiObjectPath & cop,
                                const char * _ClassName,
                                const char * _RefLeft,
                                const char * _RefRight,
                                const char * _RefLeftClass,
                                const char * _RefRightClass,
                                int inst,
                                int associators )
  {
    CmpiInstance cis( broker.getInstance( ctx, cop, NULL ) ); // source instance

    CmpiObjectPath op("",""); // target op
    try
    {
      op = _assoc_targetClass_OP( cop, _RefLeftClass, _RefRightClass );
    }
    catch ( CmpiStatus & rc )
    {
      return true;
    }

    CmpiObjectPath rop( cop.getNameSpace(), _ClassName ); // associations op





    return false;
  }

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
                               int associators )
  {
    _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN called"));
    if ( _assoc_create_refs_1toN( broker, ctx, rslt, cop,
                                  _ClassName, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                  inst, associators ) )
    {
      _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN exited"));
      return true;
    }
    _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN failed"));
    return false;
  }


  /** Method to check the input parameter resultClass, role and resultRole
   *  submitted to the methods of the association interface.
  */
  bool _assoc_check_parameter_const( const CmpiObjectPath & cop,
                                     const char * _RefLeft,
                                     const char * _RefRight,
                                     const char * _RefLeftClass,
                                     const char * _RefRightClass,
                                     const char * resultClass,
                                     const char * role,
                                     const char * resultRole )
  {
    const CmpiObjectPath & scop( cop );

    DBG << "scop            " << scop << endl;
    DBG << "_RefLeft        " << zypp::C_Str(_RefLeft) << endl;
    DBG << "_RefRight       " << zypp::C_Str(_RefRight) << endl;
    DBG << "_RefLeftClass   " << zypp::C_Str(_RefLeftClass) << endl;
    DBG << "_RefRightClass  " << zypp::C_Str(_RefRightClass) << endl;

    DBG << "resultClass     " << zypp::C_Str(resultClass) << endl;
    DBG << "role            " << zypp::C_Str(role) << endl;
    DBG << "resultRole      " << zypp::C_Str(resultRole) << endl;

    if ( ! ( scop.classPathIsA( _RefLeftClass ) || scop.classPathIsA( _RefRightClass ) ) )
      return false;

    if ( ! ( resultClass || role || resultRole ) )
      return true;

    /* check if resultClass is parent or the class itself of the target class */
    if ( resultClass )
    {
      CmpiObjectPath op = CmpiObjectPath( cop.getNameSpace(),
                                          ( scop.classPathIsA( _RefLeftClass ) ? _RefRightClass : _RefLeftClass ) );
      CmpiObjectPath rcop = CmpiObjectPath( cop.getNameSpace(), resultClass );

      if ( ! ( op.classPathIsA( resultClass )
           || ( rcop.classPathIsA( _RefRightClass ) &&  scop.classPathIsA( _RefLeftClass ) )
           || ( rcop.classPathIsA( _RefLeftClass ) &&  scop.classPathIsA( _RefRightClass ) ) ) )
        return false;
    }

    /* check if the source object (cop) plays the Role ( specified in input
     * parameter 'role' within this association */
    if ( role )
    {
      if ( ! (  ( scop.classPathIsA( _RefLeftClass ) && ::strcasecmp( role, _RefLeft ) == 0 )
             || ( scop.classPathIsA( _RefRightClass ) && ::strcasecmp( role, _RefRight ) == 0 ) ) )
        return false;
    }

    /* check if the target object plays the Role ( specified in input
     * parameter 'resultRole' within this association */
    if ( resultRole )
    {
      if ( ! (  ( scop.classPathIsA( _RefLeftClass ) && ::strcasecmp( resultRole, _RefRight ) == 0 )
             || ( scop.classPathIsA( _RefRightClass ) && ::strcasecmp( resultRole, _RefLeft ) == 0 ) ) )
        return false;
    }

    return true;
  }

  bool assoc_check_parameter_const( const CmpiObjectPath & cop,
                                    const char * _RefLeft,
                                    const char * _RefRight,
                                    const char * _RefLeftClass,
                                    const char * _RefRightClass,
                                    const char * resultClass,
                                    const char * role,
                                    const char * resultRole )
  {
    _CMPIZYPP_TRACE(1,("--- _assoc_check_parameter_const called"));
    if ( _assoc_check_parameter_const(cop,_RefLeft, _RefRight, _RefLeftClass, _RefRightClass, resultClass, role, resultRole) )
    {
      _CMPIZYPP_TRACE(1,("--- _assoc_check_parameter_const exited: responsible"));
      return true;
    }
    _CMPIZYPP_TRACE(1,("--- _assoc_check_parameter_const exited: not responsible"));
    return false;
  }
}
