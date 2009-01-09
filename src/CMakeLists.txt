####################################################################
# SOURCES                                                          #
####################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_DEFINITIONS(-DLOCALEDIR="${CMAKE_INSTALL_PREFIX}/share/locale" -DTEXTDOMAIN="cmpizypp" )

SET( cmpizypp_SRCS
  SUSESoftwareIdentity.cc
)
SET( cmpizypp_HEADERS
  SUSESoftwareIdentity.h
)

ADD_LIBRARY(cmpizypp SHARED ${cmpizypp_SRCS})
#SET_TARGET_PROPERTIES( cmpizypp PROPERTIES VERSION "${CMPIZYPP_VERSION_INFO}" )
#SET_TARGET_PROPERTIES( cmpizypp PROPERTIES SOVERSION "${CMPIZYPP_SOVERSION_INFO}" )

# System libraries
TARGET_LINK_LIBRARIES(cmpizypp ${ZYPP_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpizypp ${SBLIMCMPI_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpizypp ${GETTEXT_LIBRARIES} )

INSTALL(TARGETS cmpizypp LIBRARY DESTINATION ${LIB_INSTALL_DIR}/cmpi )
