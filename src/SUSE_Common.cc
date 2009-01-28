
#include <cmpi/CmpiObjectPath.h>
#include <cmpi/CmpiResult.h>
#include <cmpiutil/base.h>

#include <zypp/base/String.h>
#include <zypp/base/LogTools.h>
#include <zypp/base/PtrTypes.h>

#include "SUSE_Common.h"
#include "SUSE_zypp.h"

using namespace zypp;

std::ostream & operator<<( std::ostream & str, const CmpiObjectPath & obj )
{
  CmpiObjectPath op( obj );
  str << op.toString().charPtr() << " " << op.getKeyCount() << " Keys";
  return str;
}

std::ostream & operator<<( std::ostream & str, const CmpiInstance & ci )
{
  CmpiInstance lci( ci );
  str << lci.toString().charPtr();
  return str;
}

namespace cmpizypp
{
  CmpiObjectPath get_this_computersystem(CmpiBroker & broker, const CmpiContext& ctx, const CmpiObjectPath & cop)
  {
    _CMPIZYPP_TRACE(4,("--- get_this_computersystem() called"));

    char hostname[256];
    if(cmpiutilGetHostName(hostname, sizeof(hostname)) == NULL)
    {
      CmpiStatus st(CMPI_RC_ERR_FAILED, "Cannot get hostname");
      _CMPIZYPP_TRACE(1,("--- get_this_computersystem() failed: %s", st.msg()));
      throw st;
    }

    _CMPIZYPP_TRACE(1,("Our Hostname is: %s", hostname));

    CmpiObjectPath op( cop.getNameSpace(), "CIM_ComputerSystem");
    CmpiEnumeration en = broker.enumInstanceNames( ctx, op );
    if( en.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "No CIM_ComputerSystem found.");
      _CMPIZYPP_TRACE(1,("--- get_this_computersystem failed: %s", st.msg() ) );
      throw st;
    }

    CmpiObjectPath data("", "");
    bool found = false;
    while( en.hasNext() )
    {
      data = en.getNext();
      const char *name = data.getKey("Name");

      _CMPIZYPP_TRACE(1,("Found object with Hostname is: %s", name));

      if(name && strcmp(name, hostname) == 0)
      {
	found = true;
	break;
      }
    }

    if(!found)
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "No CIM_ComputerSystem found.");
      _CMPIZYPP_TRACE(1,("--- get_this_computersystem failed: %s", st.msg() ) );
      throw st;
    }
    return data;
  }

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

  bool _assoc_create_refs_1toN_ST( CmpiBroker & broker,
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
                                   const SUSE_AssocFilter &filter )
  {
    CmpiInstance cis( broker.getInstance( ctx, cop, NULL ) ); // source instance

    if( !cis.instanceIsA(_RefSourceClass) )
    {
      return false;
    }

    CmpiObjectPath op("",""); // target op
    try
    {
      op = _assoc_targetClass_OP( cop, _RefSourceClass, _RefTargetClass );
    }
    catch ( CmpiStatus & rc )
    {
      return true;
    }

    CmpiObjectPath rop( cop.getNameSpace(), _ClassName ); // associations op

    if( (associators == 1) && (inst == 1) )
    {
            /* associators() */
      CmpiEnumeration en(broker.enumInstances( ctx, op, NULL ) );
      if( en.isNull() )
      {
        CmpiStatus st(CMPI_RC_ERR_FAILED, "EnumInstances failed.");
        _CMPIZYPP_TRACE(1,("--- %s", st.msg()));
        return false;
      }
      while (en.hasNext() )
      {
        CmpiInstance ci = en.getNext();
	if( filter.filterInstance(cop, _RefSource, ci, true) )
	  rslt.returnData( ci );
      }
    }
    else
    {
      CmpiEnumeration en( broker.enumInstanceNames( ctx, op ) );
      if( en.isNull() )
      {
        CmpiStatus st(CMPI_RC_ERR_FAILED, "EnumInstanceNames failed.");
        _CMPIZYPP_TRACE(1,("--- %s", st.msg()));
        return false;
      }
      while (en.hasNext() )
      {
        CmpiData data = en.getNext();
        if( associators == 0 )
        {
                /* references() || referenceNames() */
          CmpiInstance ci( rop );
          if( ci.isNull() )
          {
            _CMPIZYPP_TRACE(1,("--- Create CmpiInstance failed."));
            return false;
          }
          ci.setProperty( _RefSource, cop );
          ci.setProperty( _RefTarget, data );

          if( inst == 0 )
          {
                /* associators = 0 && inst = 0 */
            CmpiObjectPath tmp = ci.getObjectPath();
            tmp.setNameSpace(cop.getNameSpace());
	    if( filter.filterObjectPath(cop, _RefSource, tmp, false) )
              rslt.returnData(tmp);
          }
          else
          {
                /* associators = 0 && inst = 1 */
	    if( filter.filterInstance(cop, _RefSource, ci, false) )
              rslt.returnData(ci);
          }
        }
        else if(inst == 0)
        {
                /* associators = 1 && inst = 0 */
          //rslt.returnData(data);
          CmpiObjectPath tmp = (CmpiObjectPath)data;
          tmp.setNameSpace(cop.getNameSpace());
	  if( filter.filterObjectPath(cop, _RefSource, tmp, true) )
            rslt.returnData(tmp);
        }
      }
    }
    return true;
  }

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
                                  const SUSE_AssocFilter &filter )
  {
    _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN_ST called"));
    if ( _assoc_create_refs_1toN_ST( broker, ctx, rslt, cop,
                                  _ClassName, _RefSource, _RefTarget, _RefSourceClass, _RefTargetClass,
                                  inst, associators, filter ) )
    {
      _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN_ST exited"));
      return true;
    }
    _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN_ST failed"));
    return false;
  }

  /* ---------------------------------------------------------------------------*/
  /*                      assoc_create_refs_1toN()                             */
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
                                 const SUSE_AssocFilter &filter )
    {
      _CMPIZYPP_TRACE(1,("--- assoc_create_refs_1toN called"));
      bool result = false;
      if( cop.classPathIsA(_RefLeftClass) )
      {
        result = _assoc_create_refs_1toN_ST(broker, ctx, rslt, cop,
                                            _ClassName, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass,
                                            inst, associators, filter);
      }
      else
      {
        result = _assoc_create_refs_1toN_ST(broker, ctx, rslt, cop,
                                            _ClassName, _RefRight, _RefLeft, _RefRightClass, _RefLeftClass,
                                            inst, associators, filter);
      }

      if ( result )
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

  // FIXME: currently not used. Perhaps we can remove it
  bool _assoc_create_inst_1toN( CmpiBroker & broker,
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
                                const SUSE_AssocFilter &filter )
  {
    _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN called"));
    bool ret = false;
    CmpiObjectPath op("","");

    if( left == 0)
    {
      op = CmpiObjectPath(cop.getNameSpace(), _RefRightClass );
    }
    else
    {
      op = CmpiObjectPath(cop.getNameSpace(), _RefLeftClass );
    }

    if( op.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed.");
      _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN failed: %s", st.msg() ) );
      throw st;
    }

    CmpiEnumeration en = broker.enumInstanceNames( ctx, op );
    if( en.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "EnumInstanceNames failed.");
      _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN failed: %s", st.msg() ) );
      throw st;
    }

   /* this approach works only for 1 to N relations
   * int <left> contains the information, which side of the association has
   * only one instance
   */
    while( en.hasNext() )
    {
      CmpiData data = en.getNext();
      ret = assoc_create_refs_1toN( broker, ctx, rslt, data,
                                     _ClassName,_RefLeft,_RefRight,
                                     _RefLeftClass,_RefRightClass,inst,0, filter);
    }
    _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN exited"));
    return ret;
  }

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
                               const SUSE_AssocFilter &filter)
  {
    _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN called"));
    if ( _assoc_create_inst_1toN(broker, ctx, rslt, cop,
                                 _ClassName, _RefLeft, _RefRight, _RefLeftClass, _RefRightClass, left, inst, filter) )
    {
      _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN exited: successful"));
      return true;
    }
    _CMPIZYPP_TRACE(1,("--- _assoc_create_inst_1toN exited: failed"));
    return false;
  }

  CmpiInstance assoc_get_inst( CmpiBroker & broker,
                               const CmpiContext & ctx,
                               const CmpiObjectPath & cop,
                               const char * _ClassName,
                               const char * _RefLeft,
                               const char * _RefRight)
  {
    CmpiObjectPath tcop( cop );
    _CMPIZYPP_TRACE(1,("--- assoc_get_inst called: %s", tcop.toString().charPtr()));

    CmpiData dtl = cop.getKey( _RefLeft );
    CmpiObjectPath opl(dtl);
    opl.setNameSpace( cop.getNameSpace() );

    _CMPIZYPP_TRACE(1,("call getInstance on : LEFT"));

    CmpiInstance ci = broker.getInstance( ctx, opl, NULL );
    if( ci.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "GetInstance of left reference failed.");
      _CMPIZYPP_TRACE(1,("--- _assoc_get_inst failed: %s", st.msg() ) );
      throw st;
    }

    CmpiData dtr = cop.getKey( _RefRight );
    CmpiObjectPath opr(dtr);
    opr.setNameSpace( cop.getNameSpace() );

    _CMPIZYPP_TRACE(1,("call getInstance on : RIGHT"));

    ci = broker.getInstance( ctx, opr, NULL );
    if( ci.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "GetInstance of right reference failed.");
      _CMPIZYPP_TRACE(1,("--- _assoc_get_inst failed: %s", st.msg() ) );
      throw st;
    }

    CmpiObjectPath op( cop.getNameSpace(), _ClassName );
    if( op.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "Create CMPIObjectPath failed.");
      _CMPIZYPP_TRACE(1,("--- _assoc_get_inst failed: %s", st.msg() ) );
      throw st;
    }

    ci = CmpiInstance( op );
    if( ci.isNull() )
    {
      CmpiStatus st( CMPI_RC_ERR_FAILED, "Create CMPIInstance failed.");
      _CMPIZYPP_TRACE(1,("--- _assoc_get_inst failed: %s", st.msg() ) );
      throw st;
    }

    ci.setProperty( _RefLeft, dtl );
    ci.setProperty( _RefRight, dtr );

    _CMPIZYPP_TRACE(1,("--- assoc_get_inst exited"));
    return ci;
  }

} // end namespace cmpizypp
