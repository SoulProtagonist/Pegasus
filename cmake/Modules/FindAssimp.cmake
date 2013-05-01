# Locate assimp library
# This module defines
# ASSIMP_LIBRARY, the name of the library to link against
# ASSIMP_FOUND, if false, do not try to link to SDL
# ASSIMP_INCLUDE_DIR, where to find SDL.h
#

FIND_PATH(ASSIMP_INCLUDE_DIR Importer.hpp
  PATH_SUFFIXES assimp
)

FIND_LIBRARY(ASSIMP_LIBRARY 
  NAMES assimp
  PATH_SUFFIXES lib
)

SET(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
SET(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(assimp
                                  DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)
