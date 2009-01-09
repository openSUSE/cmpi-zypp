PROJECT(CMPIZYPP)
SET( PACKAGE "cmpi-zypp" )
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
SET( CMAKE_MODULE_PATH ${CMPIZYPP_SOURCE_DIR}/cmake/modules )
cmake_minimum_required(VERSION 2.6)

# allow name libraries by name mixed with full
# paths
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

INCLUDE( ZyppCommon )
INCLUDE( ${CMPIZYPP_SOURCE_DIR}/VERSION.cmake )

MATH( EXPR CMPIZYPP_CURRENT "${CMPIZYPP_MAJOR} * 100 + ${CMPIZYPP_MINOR}" )
MATH( EXPR CMPIZYPP_AGE     "${CMPIZYPP_MINOR} - ${CMPIZYPP_COMPATMINOR}" )
# Libtool wanted current:patch:age
# But cmake is not libtool, it wants the verbatim suffix to libzypp.so
MATH( EXPR CMPIZYPP_SO_FIRST  "${CMPIZYPP_CURRENT}-${CMPIZYPP_AGE}" )
SET( VERSION "${CMPIZYPP_MAJOR}.${CMPIZYPP_MINOR}.${CMPIZYPP_PATCH}" )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g -Wall -Woverloaded-virtual -Wnon-virtual-dtor" )
SET( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC -g -Wall" )
set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3" )

ADD_DEFINITIONS( -D_FILE_OFFSET_BITS=64 )
ADD_DEFINITIONS( -DVERSION="${VERSION}" )
SET( CMPIZYPP_VERSION_INFO "${CMPIZYPP_SO_FIRST}.${CMPIZYPP_AGE}.${CMPIZYPP_PATCH}" )
SET( CMPIZYPP_SOVERSION_INFO "${CMPIZYPP_SO_FIRST}" )

GENERATE_PACKAGING(${PACKAGE} ${VERSION})

INCLUDE(CPack)

MACRO(ADD_TESTS)
  FOREACH( loop_var ${ARGV} )
    SET_SOURCE_FILES_PROPERTIES( ${loop_var}_test.cc COMPILE_FLAGS "-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN -DBOOST_AUTO_TEST_MAIN=\"\" " )
    ADD_EXECUTABLE( ${loop_var}_test ${loop_var}_test.cc )
    TARGET_LINK_LIBRARIES( ${loop_var}_test  zypp boost_unit_test_framework zypp_test_utils)
    ADD_TEST( ${loop_var}_test ${CMAKE_CURRENT_BINARY_DIR}/${loop_var}_test)
  ENDFOREACH( loop_var )
ENDMACRO(ADD_TESTS)

####################################################################

FIND_PACKAGE( Zypp REQUIRED )
IF( ZYPP_FOUND )
INCLUDE_DIRECTORIES(${ZYPP_INCLUDE_DIR})
ELSE( ZYPP_FOUND )
MESSAGE( FATAL_ERROR "libzypp not found" )
ENDIF( ZYPP_FOUND )

FIND_PACKAGE(Gettext REQUIRED)
IF (GETTEXT_FOUND)
  MESSAGE(STATUS "Found Gettext: ${GETTEXT_SOURCE}")
  INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
ELSE (GETTEXT_FOUND)
  MESSAGE( FATAL_ERROR "Gettext not found" )
ENDIF (GETTEXT_FOUND)

FIND_PACKAGE(Sblimcmpi REQUIRED)
IF (SBLIMCMPI_FOUND)
MESSAGE(STATUS "Found Sblim-cmpi: ${SBLIMCMPI_SOURCE}")
INCLUDE_DIRECTORIES(${SBLIMCMPI_INCLUDE_DIR})
ELSE (SBLIMCMPI_FOUND)
MESSAGE( FATAL_ERROR "Sblim-cmpi not found" )
ENDIF (SBLIMCMPI_FOUND)

FIND_PROGRAM( DOXYGEN doxygen )
IF ( NOT DOXYGEN )
  MESSAGE( FATAL_ERROR "doxygen not found: install doxygen to build the documentation." )
ELSE ( NOT DOXYGEN )
  MESSAGE( STATUS "doxygen found: ${DOXYGEN}" )
ENDIF ( NOT DOXYGEN )

MESSAGE(STATUS "soname: ${CMPIZYPP_VERSION_INFO}")
MESSAGE(STATUS "version: ${VERSION}")

####################################################################
# config templates
# (don't forget to mention them in the .spec file)
####################################################################

####################################################################
# SUBDIRECTORIES                                                   #
####################################################################

ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( doc )

#ADD_SUBDIRECTORY( tests EXCLUDE_FROM_ALL )
#INCLUDE(CTest)
#ENABLE_TESTING()
