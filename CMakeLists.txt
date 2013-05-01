
#=============================================================================
#
# Pegasus GPL Source Code
# Copyright (C) 2012 Leo Walsh
#
# This file is part of the Pegasus GPL Source Code.  
#
# Pegasus Source Code is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Pegasus Source Code is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Pegasus Source Code.  If not, see <http://www.gnu.org/licenses/>.
#
#==============================================================================


CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Pegasus)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

IF(WIN32)
	SET(CMAKE_PREFIX_PATH
		 $ENV{SDLDIR}/lib/x86
		 $ENV{SDLDIR}/include
		 $ENV{DEVILDIR}/lib
		 $ENV{DEVILDIR}/include
		 $ENV{ASSIMPDIR}/include
		 $ENV{ASSIMPDIR}/lib/assimp_release-dll_win32
		 )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG bin)
ENDIF()

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(DevIL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(Assimp REQUIRED)

INCLUDE_DIRECTORIES(
	Include
	${SDL_INCLUDE_DIR}
	${IL_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIR}
	$ENV{GLMDIR}
)

SET(SRC
	Source/Engine.cpp
	Source/GameRenderer.cpp
	Source/Main.cpp
	Source/Shader.cpp
	Source/PegasusGL.cpp
	Source/Model.cpp
)

SET(LIBS
	${SDL_LIBRARY}
	${IL_LIBRARIES}
	${OPENGL_LIBRARIES}
	${ASSIMP_LIBRARY}
)

FILE(GLOB all_headers Include/*)
ADD_EXECUTABLE(Pegasus
	${SRC}
	${all_headers}
	
)

TARGET_LINK_LIBRARIES(Pegasus ${LIBS})

IF(WIN32)
	ADD_CUSTOM_COMMAND(TARGET Pegasus POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{SDLDIR}/lib/x86/sdl.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
	ADD_CUSTOM_COMMAND(TARGET Pegasus POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{DEVILDIR}/lib/devIL.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
	ADD_CUSTOM_COMMAND(TARGET Pegasus POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{ASSIMPDIR}/bin/assimp_release-dll_win32/Assimp32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
ENDIF(WIN32)
