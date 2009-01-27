####################################################################
# SOURCES                                                          #
####################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_DEFINITIONS(-DLOCALEDIR="${CMAKE_INSTALL_PREFIX}/share/locale" -DTEXTDOMAIN="cmpi-zypp" )

SET( CMPIZYPP_SRCS
  SUSE_Common.cc
  SUSE_zypp.cc
  SUSE_SoftwareIdentityProvider.cc
  SUSE_InstalledSoftwareIdentityProvider.cc
  SUSE_SystemSpecificCollectionProvider.cc
  SUSE_MemberOfCollectionProvider.cc
  SUSE_HostedCollectionProvider.cc
)
SET( CMPIZYPP_HEADERS
  SUSE_Common.h
  SUSE_zypp.h
  SUSE_SoftwareIdentityProvider.h
  SUSE_InstalledSoftwareIdentityProvider.h
  SUSE_SystemSpecificCollectionProvider.h
  SUSE_MemberOfCollectionProvider.h
  SUSE_HostedCollectionProvider.h
  SUSE_AssocFilter.h
)

ADD_LIBRARY(cmpi-zypp SHARED ${CMPIZYPP_SRCS})
#SET_TARGET_PROPERTIES( cmpi-zypp PROPERTIES VERSION "${CMPIZYPP_VERSION_INFO}" )
#SET_TARGET_PROPERTIES( cmpi-zypp PROPERTIES SOVERSION "${CMPIZYPP_SOVERSION_INFO}" )

# System libraries
TARGET_LINK_LIBRARIES(cmpi-zypp ${ZYPP_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpi-zypp ${SBLIMCMPI_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpi-zypp ${GETTEXT_LIBRARIES} )
TARGET_LINK_LIBRARIES(cmpi-zypp ${CMPIUTIL_LIBRARY} )

INSTALL(TARGETS cmpi-zypp LIBRARY DESTINATION ${LIB_INSTALL_DIR}/cmpi )
