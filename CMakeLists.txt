cmake_minimum_required(VERSION 3.10)

set(Mayo_VersionMajor 0)
set(Mayo_VersionMinor 7)
set(Mayo_VersionPatch 0)
set(Mayo_VersionCommit 0)
set(Mayo_VersionRevNum 0)
set(Mayo_Version ${Mayo_VersionMajor}.${Mayo_VersionMinor}.${Mayo_VersionPatch})

project(
    mayo
    VERSION "${Mayo_Version}"
    DESCRIPTION "Mayo - Opensource 3D CAD viewer and converter"
    LANGUAGES CXX
)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Find bit size of the target architecture
# math(EXPR Mayo_TargetArchBitSize "8 * ${CMAKE_SIZEOF_VOID_P}")
# message(STATUS "Mayo_TargetArchBitSize = ${Mayo_TargetArchBitSize}")

##########
# Compiler config
##########

set(CMAKE_CXX_STANDARD 17)

set(
    Mayo_CompileDefinitions
    QT_DISABLE_DEPRECATED_BEFORE=0x050F00
    QT_IMPLICIT_QFILEINFO_CONSTRUCTION
)

if(MSVC)
    string(APPEND Mayo_CompileDefinitions " NOMINMAX")
    string(APPEND CMAKE_CXX_FLAGS " /we4150") # Deletion of pointer to incomplete type 'XXXX'; no destructor called
elseif(APPLE)
    string(APPEND Mayo_CompileDefinitions " GL_SILENCE_DEPRECATION")
endif()

##########
# Linker config
##########

set(Mayo_LinkLibraries)

if(MSVC)
    list(APPEND Mayo_LinkLibraries Opengl32 User32)
elseif(APPLE)
    list(APPEND Mayo_LinkLibraries iconv)
endif()

##########
# Qt
##########

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
# TODO Add openglwidget module for Qt6
message(STATUS "Qt version = ${QT_VERSION}")

#qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

##########
# Source files
##########

file(
    GLOB Mayo_SourceFiles
    ${PROJECT_SOURCE_DIR}/src/app/*.cpp
    ${PROJECT_SOURCE_DIR}/src/base/*.cpp
    ${PROJECT_SOURCE_DIR}/src/graphics/*.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_dxf/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_gmio/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_image/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_occ/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_off/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_ply/*.cpp
    ${PROJECT_SOURCE_DIR}/src/io_measure/*.cpp
    ${PROJECT_SOURCE_DIR}/src/3rdparty/fmt/src/format.cc
)

##########
# Header files
##########

file(
    GLOB Mayo_HeaderFiles
    ${PROJECT_SOURCE_DIR}/src/app/*.h
    ${PROJECT_SOURCE_DIR}/src/base/*.h
    ${PROJECT_SOURCE_DIR}/src/graphics/*.h
    ${PROJECT_SOURCE_DIR}/src/gui/*.h
    ${PROJECT_SOURCE_DIR}/src/io_dxf/*.h
    ${PROJECT_SOURCE_DIR}/src/io_gmio/*.h
    ${PROJECT_SOURCE_DIR}/src/io_image/*.h
    ${PROJECT_SOURCE_DIR}/src/io_occ/*.h
    ${PROJECT_SOURCE_DIR}/src/io_off/*.h
    ${PROJECT_SOURCE_DIR}/src/io_ply/*.h
    ${PROJECT_SOURCE_DIR}/src/io_measure/*.h
)

##########
# UI files
##########

file(
    GLOB Mayo_QtUiFiles
    ${PROJECT_SOURCE_DIR}/src/app/*.ui
)

##########
# OpenCascade
##########

find_package(OpenCASCADE REQUIRED)
message(STATUS "OpenCascade version = ${OpenCASCADE_VERSION}")

list(
    APPEND Mayo_LinkLibraries
    ${OpenCASCADE_FoundationClasses_LIBRARIES}
    ${OpenCASCADE_ModelingData_LIBRARIES}
    # OpenCascade ModelingAlgorithms libs
    TKBO TKBool TKGeomAlgo TKHLR TKMesh TKPrim TKShHealing TKTopAlgo
    # OpenCascade Visualization libs
    TKMeshVS TKOpenGl TKService TKV3d
    # OpenCascade ApplicationFramework libs
    TKBin TKBinL TKBinXCAF TKCAF TKCDF TKLCAF TKVCAF TKXml TKXmlL
    # OpenCascade DataExchange libs
    TKXCAF TKXmlXCAF TKXSBase
    # OpenCascade Draw libs
    TKXSDRAW # Seems to be required on macOS(see https://github.com/fougue/mayo/issues/41#issuecomment-742732322)
)

# IGES Support
list(APPEND Mayo_LinkLibraries TKIGES TKXDEIGES)

# STEP Support
list(APPEND Mayo_LinkLibraries TKSTEP TKSTEP209 TKSTEPAttr TKSTEPBase TKXDESTEP)

# STL Support
list(APPEND Mayo_LinkLibraries TKSTL)

# OBJ/glTF support
if("${OpenCASCADE_VERSION}" VERSION_GREATER_EQUAL "7.4.0")
    list(APPEND Mayo_LinkLibraries TKRWMesh)
else()
    list(
        REMOVE_ITEM Mayo_SourceFiles
        ${PROJECT_SOURCE_DIR}/src/io_occ/io_occ_base_mesh.cpp
        ${PROJECT_SOURCE_DIR}/src/io_occ/io_occ_gltf_reader.cpp
        ${PROJECT_SOURCE_DIR}/src/io_occ/io_occ_obj_reader.cpp
    )
endif()

if("${OpenCASCADE_VERSION}" VERSION_LESS "7.5.0")
    list(REMOVE_ITEM Mayo_SourceFiles ${PROJECT_SOURCE_DIR}/src/io_occ/io_occ_gltf_writer.cpp)
endif()

if("${OpenCASCADE_VERSION}" VERSION_LESS "7.6.0")
    list(REMOVE_ITEM Mayo_SourceFiles ${PROJECT_SOURCE_DIR}/src/io_occ/io_occ_obj_writer.cpp)
endif()

# VRML support
list(APPEND Mayo_LinkLibraries TKVRML)
if("${OpenCASCADE_VERSION}" VERSION_LESS "7.7.0")
    list(REMOVE_ITEM Mayo_SourceFiles ${PROJECT_SOURCE_DIR}/src/io_occ/io_occ_vrml_reader.cpp)
endif()

##########
# Targets
##########

add_executable(
    mayo
    ${Mayo_SourceFiles}
    ${Mayo_HeaderFiles}
    ${Mayo_QtUiFiles}
    ${Mayo_QtI18nFiles}
    ${Mayo_QtQmFiles}
    mayo.qrc
)

configure_file(${PROJECT_SOURCE_DIR}/src/app/version.h.cmake app/version.h @ONLY)

target_include_directories(
    mayo PRIVATE
    src/app
    src/3rdparty
)

target_compile_definitions(
    mayo PRIVATE ${Mayo_CompileDefinitions}
)

target_link_libraries(
    mayo PRIVATE ${Mayo_LinkLibraries}
)
