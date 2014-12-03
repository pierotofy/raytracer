cmake_minimum_required (VERSION 2.8)
project (raytracer)

include_directories(${CMAKE_SOURCE_DIR}/cs5721Code/cs5721GraphicsLib/src)

IF(WIN32)
INCLUDE_DIRECTORIES("C:/Program Files/libxml2/include")
LINK_DIRECTORIES("C:/Program Files/libxml2")
ELSE()
INCLUDE_DIRECTORIES(/usr/include/libxml2)
ENDIF()

# Attempt to locate the PNG include directories, link directories and
# libraries.
find_package(PNG REQUIRED)
if (PNG_FOUND)
  MESSAGE(STATUS "Found png: ${PNG_INCLUDE_DIR}, ${PNG_LIBRARY}")
  INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})
endif (PNG_FOUND)

# If you want to get more info on what FindBoost is doing...
# SET(Boost_DEBUG 1)

# Necessary to get Win32 version to work.
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost REQUIRED)
IF(${Boost_FOUND})
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
IF(${Boost_PROGRAM_OPTIONS_FOUND})
  MESSAGE(${Boost_PROGRAM_OPTIONS_LIBRARIES})
  # TARGET_LINK_LIBRARIES(boostThreadExample 
ENDIF()

FIND_PACKAGE(Boost COMPONENTS system REQUIRED)
IF(${Boost_SYSTEM_FOUND})
  MESSAGE(${Boost_SYSTEM_LIBRARIES})
ENDIF()

FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)
IF(${Boost_FILESYSTEM_FOUND})
  MESSAGE(${Boost_FILESYSTEM_LIBRARIES})
ENDIF()

#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE RelWithDebInfo)
#SET(CMAKE_BUILD_TYPE MinSizeRel)

IF(${CMAKE_BUILD_TYPE} MATCHES Release)
  set(CMAKE_CXX_FLAGS_RELEASE   "-O4")
ENDIF()

ADD_EXECUTABLE (raytracer
  main.cpp
  Scene.cpp Scene.h
  Camera.cpp Camera.h
  Light.cpp Light.h
  PointLight.cpp PointLight.h
  AreaLight.cpp AreaLight.h
  OrthNormBasis.cpp OrthNormBasis.h Utils.h
  Object.cpp Object.h
  Shader.cpp Shader.h
  LambertianShader.cpp LambertianShader.h
  BlinnphongShader.cpp BlinnphongShader.h
  NormalMapShader.cpp NormalMapShader.h
  AmbientShader.cpp AmbientShader.h
  GlazeShader.cpp GlazeShader.h
  Utils.cpp Utils.h
  SphereObject.cpp SphereObject.h
  BoxObject.cpp BoxObject.h
  PyramidObject.cpp PyramidObject.h
  TriangleObject.cpp TriangleObject.h
  CylinderObject.cpp CylinderObject.h
  Globals.h
  ShaderArgs.h
  Ray.h Ray.cpp
  Matrix4.h Matrix4.cpp
  Transform.h Transform.cpp
  Texture.h Texture.cpp
  TextureManager.h TextureManager.cpp
  InstanceManager.h InstanceManager.cpp
  InstanceObject.h InstanceObject.cpp
  Instance.h Instance.cpp
  SkySphereObject.h SkySphereObject.cpp
  BoundingBox.h BoundingBox.cpp
  MeshObject.h MeshObject.cpp
  Thread.cpp Thread.h
  RenderThread.cpp RenderThread.h
  BvhNode.cpp BvhNode.h
  )
target_link_libraries(raytracer cs5721Graphics ${Boost_SYSTEM_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARIES} ${PNG_LIBRARY} z  xml2 pthread)

add_subdirectory(cs5721Code/cs5721GraphicsLib/src)

