find_path(avformat_INCLUDE_DIR avformat.h PATH_SUFFIXES ffmpeg libavformat)
if(avformat_INCLUDE_DIR AND EXISTS "${avformat_INCLUDE_DIR}/avformat.h")
   file(STRINGS "${avformat_INCLUDE_DIR}/avformat.h" avformat_Parsed_Version REGEX "^#define LIBAVFORMAT_VERSION +.+$")
   if(avformat_Parsed_Version)
      string(REGEX REPLACE "^.*LIBAVFORMAT_VERSION +([0-9]+).*$" "\\1" avformat_VERSION_MAJOR "${avformat_Parsed_Version}")
      string(REGEX REPLACE "^.*LIBAVFORMAT_VERSION +[0-9]+\\.([0-9]+).*$" "\\1" avformat_VERSION_MINOR "${avformat_Parsed_Version}")
      string(REGEX REPLACE "^.*LIBAVFORMAT_VERSION +[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" avformat_VERSION_PATCH "${avformat_Parsed_Version}")
   else()
      file(STRINGS "${avformat_INCLUDE_DIR}/version.h" avformat_Parsed_Version REGEX "^#define LIBAVFORMAT_VERSION_MAJOR +.+$")
      string(REGEX REPLACE "^.*LIBAVFORMAT_VERSION_MAJOR +" "" avformat_VERSION_MAJOR "${avformat_Parsed_Version}")
      file(STRINGS "${avformat_INCLUDE_DIR}/version.h" avformat_Parsed_Version REGEX "^#define LIBAVFORMAT_VERSION_MINOR +.+$")
      string(REGEX REPLACE "^.*LIBAVFORMAT_VERSION_MINOR +" "" avformat_VERSION_MINOR "${avformat_Parsed_Version}")
      file(STRINGS "${avformat_INCLUDE_DIR}/version.h" avformat_Parsed_Version REGEX "^#define LIBAVFORMAT_VERSION_MICRO +.+$")
      string(REGEX REPLACE "^.*LIBAVFORMAT_VERSION_MICRO +" "" avformat_VERSION_PATCH "${avformat_Parsed_Version}")
   endif()
   set(avformat_VERSION_STRING "${avformat_VERSION_MAJOR}.${avformat_VERSION_MINOR}.${avformat_VERSION_PATCH}")
   set(avformat_MAJOR_VERSION "${avformat_VERSION_MAJOR}")
   set(avformat_MINOR_VERSION "${avformat_VERSION_MINOR}")
   set(avformat_PATCH_VERSION "${avformat_VERSION_PATCH}")
endif()

find_library(avformat_LIBRARY_RELEASE NAMES avformat)
find_library(avformat_LIBRARY_DEBUG NAMES avformatd)

include(SelectLibraryConfigurations)
select_library_configurations(avformat)

set(avformat_DEFINITIONS -DOFFSET_T_DEFINED)
if(WIN32)
    list(APPEND avformat_DEFINITIONS -DEMULATE_INTTYPES -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_ISOC9X_SOURCE -DMSVC8 -DBUILD_SHARED_AV)
endif()
set(avformat_DEFINITIONS "${avformat_DEFINITIONS}" CACHE STRING "avformat definitions")

set(avformat_INCLUDE_DIRS ${avformat_INCLUDE_DIR} ${avformat_INCLUDE_DIR}/..)
mark_as_advanced(avformat_INCLUDE_DIR)
mark_as_advanced(avformat_DEFINITIONS)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(avformat REQUIRED_VARS avformat_INCLUDE_DIR avformat_LIBRARY VERSION_VAR avformat_VERSION_STRING)
set(avformat_FOUND ${AVFORMAT_FOUND})