find_path(Qwt_INCLUDE_DIR qwt.h PATH_SUFFIXES qt5/qwt)
if(Qwt_INCLUDE_DIR AND EXISTS "${Qwt_INCLUDE_DIR}/qwt_global.h")
    file(STRINGS "${Qwt_INCLUDE_DIR}/qwt_global.h" Qwt_Parsed_Version REGEX "^#define QWT_VERSION_STR +\"[^\"]+\"$")
    string(REGEX REPLACE "^.*QWT_VERSION_STR +\"([0-9]+).*$" "\\1" Qwt_VERSION_MAJOR "${Qwt_Parsed_Version}")
    string(REGEX REPLACE "^.*QWT_VERSION_STR +\"[0-9]+\\.([0-9]+).*$" "\\1" Qwt_VERSION_MINOR "${Qwt_Parsed_Version}")
    string(REGEX REPLACE "^.*QWT_VERSION_STR +\"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" Qwt_VERSION_PATCH "${Qwt_Parsed_Version}")

    set(Qwt_VERSION_STRING "${Qwt_VERSION_MAJOR}.${Qwt_VERSION_MINOR}.${Qwt_VERSION_PATCH}")
    set(Qwt_MAJOR_VERSION "${Qwt_VERSION_MAJOR}")
    set(Qwt_MINOR_VERSION "${Qwt_VERSION_MINOR}")
    set(Qwt_PATCH_VERSION "${Qwt_VERSION_PATCH}")
endif()

find_library(Qwt_LIBRARY_RELEASE NAMES qwt5 qwt-qt4 qwt)
find_library(Qwt_LIBRARY_DEBUG NAMES qwt5d qwt-qt4d qwt)

include(SelectLibraryConfigurations)
select_library_configurations(Qwt) #sets Qwt_LIBRARY using Qwt_LIBRARY_DEBUG and Qwt_LIBRARY_RELEASE
if(NOT Qwt_LIBRARY)
   set(Qwt_LIBRARY optimized ${Qwt_LIBRARY_RELEASE} debug ${Qwt_LIBRARY_DEBUG})
   set(Qwt_LIBRARY ${Qwt_LIBRARY} CACHE FILEPATH "The Qwt library")
endif()

set(Qwt_INCLUDE_DIRS ${Qwt_INCLUDE_DIR})
mark_as_advanced(Qwt_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Qwt REQUIRED_VARS Qwt_INCLUDE_DIR Qwt_LIBRARY VERSION_VAR Qwt_VERSION_STRING)
set(Qwt_FOUND ${QWT_FOUND})

