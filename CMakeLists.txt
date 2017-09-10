cmake_minimum_required (VERSION 2.8)

project (superbible7)

LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/lib )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

find_package(OpenGL REQUIRED)

set (CMAKE_DEBUG_POSTFIX "_d")

if(WIN32)
set(COMMON_LIBS sb7 optimized glfw debug glfw3_d ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
elseif(UNIX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
#set(COMMON_LIBS sb7 glfw Xrandr X11 Xinerama Xi Xxf86vm Xcursor GL rt dl)
set(COMMON_LIBS sb7 glfw ${GLFW_LIBRARIES})
else()
set(COMMON_LIBS sb7)
endif()

IF(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(EXTRA_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY})
  SET(EXTRA_LIBS ${EXTRA_LIBS} /usr/local/lib/libglfw.3.2.dylib /usr/local/opt/llvm/lib/libomp.dylib)
ENDIF(APPLE)

set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

add_library(sb7
            src/sb7/sb7.cpp
            src/sb7/sb7color.cpp
            src/sb7/sb7ktx.cpp
            src/sb7/sb7object.cpp
            src/sb7/sb7shader.cpp
            src/sb7/sb7textoverlay.cpp
            src/sb7/gl3w.c
)

set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(EXAMPLES
  # Chapter 2
  basic_app
  basic_animation
  basic_shaders
  basic_triangle
)
# set(EXAMPLES
#   alienrain
#   basicfbo
#   bindlesstex
#   blendmatrix
#   blinnphong
#   bumpmapping
#   clipdistance
#   csflocking
#   compressrgtc
#   cubemapenv
#   cubicbezier
#   cullindirect
#   deferredshading
#   depthclamp
#   dflandscape
#   dispmap
#   dof
#   envmapsphere
#   equirectangular
#   fontdemo
#   fragcolorfrompos
#   fragmentlist
#   grass
#   gsculling
#   gslayered
#   gsquads
#   gstessellate
#   hdrbloom
#   hdrexposure
#   hdrtonemap
#   hqfilter
#   indexedcube
#   indirectmaterial
#   instancedattribs
#   julia
#   ktxview
#   linesmooth
#   mirrorclampedge
#   movingtri
#   msaanative
#   multidrawindirect
#   multiscissor
#   multiviewport
#   noperspective
#   normalviewer
#   objectexploder
#   ompparticles
#   packetbuffer
#   perpixelgloss
#   phonglighting
#   polygonsmooth
#   prefixsum
#   pmbfractal
#   pmbstreaming
#   prefixsum2d
#   programinfo
#   raytracer
#   rimlight
#   sampleshading
# # sb6mrender
#   sdfdemo
#   shadowmapping
#   shapedpoints
#   simpleclear
#   simpletexcoords
#   simpletexture
#   singlepoint
#   singletri
#   sparsetexture
#   spinnycube
#   springmass
#   ssao
#   starfield
#   stereo
#   subroutines
#   tessellatedcube
#   tessellatedgstri
#   tessellatedtri
#   tessmodes
#   tesssubdivmodes
#   toonshading
#   tunnel
#   wrapmodes
# )

# OPTION(USE_OPENMP "Enable OpenMP" ON)
# find_package(OpenMP REQUIRED)
# if (OPENMP_FOUND)
#     set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# endif()

foreach(EXAMPLE ${EXAMPLES})
  add_executable(${EXAMPLE} WIN32 src/${EXAMPLE}/${EXAMPLE}.cpp)
  set_property(TARGET ${EXAMPLE} PROPERTY DEBUG_POSTFIX _d)
  target_link_libraries(${EXAMPLE} ${COMMON_LIBS})
  if(MSVC)
    configure_file(${PROJECT_SOURCE_DIR}/build/templates/vs2013.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${EXAMPLE}.vcxproj.user @ONLY)
  endif(MSVC)
endforeach(EXAMPLE)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX -std=c++0x")
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories(include)
include_directories(extern/glfw-3.2/include)

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})
