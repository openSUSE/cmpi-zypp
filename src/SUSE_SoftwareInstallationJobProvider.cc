
#include <iostream>
#include <time.h>

#include <cmpi/cmpimacs.h>
#include <cmpi/cmpidt.h>
#include <cmpi/CmpiResult.h>
#include <cmpi/CmpiBroker.h>
#include <cmpi/CmpiArray.h>
#include <cmpi/CmpiBooleanData.h>

#include <zypp/base/String.h>
#include <zypp/base/LogTools.h>

#include "SUSE_zypp.h"
#include "SUSE_Common.h"
#include "SUSE_SoftwareInstallationJobProvider.h"

using namespace zypp;
using std::endl;

namespace cmpizypp
{
  namespace
  {
    const char * _ClassName  = "SUSE_SoftwareInstallationJob";
  } // namespace


SUSE_SoftwareInstallationJobProviderClass::SUSE_SoftwareInstallationJobProviderClass( const CmpiBroker & mbp, const CmpiContext & ctx )
  : CmpiBaseMI( mbp, ctx )
  , CmpiInstanceMI( mbp, ctx )
  , CmpiMethodMI( mbp, ctx )
{
}

CmpiStatus SUSE_SoftwareInstallationJobProviderClass::enumInstanceNames( const CmpiContext& ctx, CmpiResult& rslt, const CmpiObjectPath & cop )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  op.setKey( "InstanceID", "SUSE:__dummy__");
  rslt.returnData( op );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstanceNames() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareInstallationJobProviderClass::enumInstances( const CmpiContext & ctx, CmpiResult & rslt, const CmpiObjectPath & cop, const char** properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );


  const char * keys[] = { "InstanceID" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "InstanceID", "SUSE:__dummy__" );

  ci.setProperty( "Name", "Zypp Software Installation Job");
  ci.setProperty( "JobState", 4 ); //Running
  ci.setProperty( "PercentComplete", 12);
  ci.setProperty( "TimeBeforeRemoval", CmpiDateTime() );
  /*
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  */

  rslt.returnData( ci );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI EnumInstances() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}


CmpiStatus SUSE_SoftwareInstallationJobProviderClass::getInstance( const CmpiContext &ctx, CmpiResult &rslt, const CmpiObjectPath &cop, const char **properties )
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() called",_ClassName));

  CmpiObjectPath op( cop.getNameSpace(), _ClassName );
  CmpiInstance   ci( op );

  const char * keys[] = { "InstanceID" };
  ci.setPropertyFilter( properties, keys );

  ci.setProperty( "InstanceID", "SUSE:__dummy__" );

  ci.setProperty( "Name", "Zypp Software Installation Job");
  ci.setProperty( "JobState", 4 ); //Running
  ci.setProperty( "PercentComplete", 12);
  ci.setProperty( "TimeBeforeRemoval", CmpiDateTime() );
  /*
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  ci.setProperty( "", "");
  */

  rslt.returnData( ci );

  rslt.returnDone();
  _CMPIZYPP_TRACE(1,("--- %s CMPI GetInstance() exited",_ClassName));
  return CmpiStatus(CMPI_RC_OK);
}

CmpiStatus SUSE_SoftwareInstallationJobProviderClass::invokeMethod (const CmpiContext &ctx, CmpiResult &rslt,
                                                                    const CmpiObjectPath &ref, const char *methodName,
                                                                    const CmpiArgs &in, CmpiArgs &out)
{
  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() called with method: %s",_ClassName, methodName));

  if( ::strcasecmp( ref.getClassName().charPtr(), _ClassName) != 0 )
  {
    return CmpiStatus(CMPI_RC_ERR_NOT_SUPPORTED, "invalid classname");
  }
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);

  if( ::strcasecmp( methodName, "ProvideAnswer" ) == 0)
  {
    st = this->provideAnswer(ctx, rslt, ref, in, out);
  }
  _CMPIZYPP_TRACE(1,("--- %s CMPI invokeMethod() exited",_ClassName));
  return st;
}

/* ========================= private ================================== */
CmpiStatus SUSE_SoftwareInstallationJobProviderClass::provideAnswer(const CmpiContext &ctx,
                                                                    CmpiResult &rslt,
                                                                    const CmpiObjectPath &ref,
                                                                    const CmpiArgs &in, CmpiArgs &out)
{
  CmpiStatus st(CMPI_RC_ERR_NOT_SUPPORTED);
  _CMPIZYPP_TRACE(1,("--- %s CMPI provideAnswer() called.",_ClassName));


  _CMPIZYPP_TRACE(1,("--- %s CMPI provideAnswer() exited.",_ClassName));
  return st;
}

} // namespace cmpizypp

CMProviderBase( SUSE_SoftwareInstallationJobProvider );

CMInstanceMIFactory( cmpizypp::SUSE_SoftwareInstallationJobProviderClass, SUSE_SoftwareInstallationJobProvider );
CMMethodMIFactory( cmpizypp::SUSE_SoftwareInstallationJobProviderClass, SUSE_SoftwareInstallationJobProvider );
