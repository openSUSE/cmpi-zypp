####################################################################
# SOURCES                                                          #
####################################################################

ADD_DEFINITIONS(-DLOCALEDIR="${CMAKE_INSTALL_PREFIX}/share/locale" -DTEXTDOMAIN="cmpizypp" )

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
#FILE(WRITE filename "message to write"... )

SET( cmpizypp_SRCS
)
SET( cmpizypp_HEADERS
)

INSTALL( FILES ${cmpizypp_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cmpizypp" )

ADD_LIBRARY(cmpizypp SHARED ${cmpizypp_SRCS})
SET_TARGET_PROPERTIES( cmpizypp PROPERTIES VERSION "${CMPIZYPP_VERSION_INFO}" )
SET_TARGET_PROPERTIES( cmpizypp PROPERTIES SOVERSION "${CMPIZYPP_SOVERSION_INFO}" )

# System libraries
TARGET_LINK_LIBRARIES(cmpizypp ${ZYPP_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpizypp ${SBLIMCMPI_LIBRARY} )
TARGET_LINK_LIBRARIES(cmpizypp ${GETTEXT_LIBRARIES} )

INSTALL(TARGETS cmpizypp LIBRARY DESTINATION ${LIB_INSTALL_DIR} )

