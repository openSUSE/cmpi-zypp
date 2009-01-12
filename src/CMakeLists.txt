####################################################################
# SOURCES                                                          #
####################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_DEFINITIONS(-DLOCALEDIR="${CMAKE_INSTALL_PREFIX}/share/locale" -DTEXTDOMAIN="cmpi-zypp" )

SET( CMPIZYPP_SRCS
  SUSE_SoftwareIdentityProvider.cc
)
SET( CMPIZYPP_HEADERS
  SUSE_SoftwareIdentityProvider.h
)

ADD_LIBRARY(cmpi-zypp SHARED ${CMPIZYPP_SRCS})
#SET_TARGET_PROPERTIES( cmpi-zypp PROPERTIES VERSION "${CMPIZYPP_VERSION_INFO}" )
#SET_TARGET_PROPERTIES( cmpi-zypp PROPERTIES SOVERSION "${CMPIZYPP_SOVERSION_INFO}" )

# System libraries
TARGET_LINK_LIBRARIES(cmpi-zypp ${ZYPP_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpi-zypp ${SBLIMCMPI_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpi-zypp ${GETTEXT_LIBRARIES} )

INSTALL(TARGETS cmpi-zypp LIBRARY DESTINATION ${LIB_INSTALL_DIR}/cmpi )
