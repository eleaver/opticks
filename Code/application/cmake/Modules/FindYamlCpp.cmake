find_path(YamlCpp_INCLUDE_DIR yaml.h PATH_SUFFIXES yaml-cpp)

find_library(YamlCpp_LIBRARY_RELEASE NAMES yaml-cpp yaml)
find_library(YamlCpp_LIBRARY_DEBUG NAMES yaml-cppd)

include(SelectLibraryConfigurations)
select_library_configurations(YamlCpp) #sets YamlCpp_LIBRARY using YamlCpp_LIBRARY_DEBUG and YamlCpp_LIBRARY_RELEASE

mark_as_advanced(YamlCpp_INCLUDE_DIR)
set(YamlCpp_INCLUDE_DIRS ${YamlCpp_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YamlCpp REQUIRED_VARS YamlCpp_INCLUDE_DIR YamlCpp_LIBRARY)
if(YAMLCPP_FOUND)

  set(Node_h ${YamlCpp_INCLUDE_DIR}/node.h)
  if(NOT EXISTS ${Node_h})
    set(Node_h ${YamlCpp_INCLUDE_DIR}/yaml-cpp/node.h)
    if(NOT EXISTS ${Node_h})
      message(ERROR " could not find YamlCpp header node.h")
    endif()
  endif()

  set(Parser_h ${YamlCpp_INCLUDE_DIR}/parser.h)
  if(NOT EXISTS ${Parser_h})
    set(Parser_h ${YamlCpp_INCLUDE_DIR}/yaml-cpp/parser.h)
    if(NOT EXISTS ${Parser_h})
      message(ERROR " could not find YamlCpp header parser.h")
    endif()
  endif()

  message(DEBUG " YamlCPP headers ${Parser_h} ${Node_h}")
  file(STRINGS ${Parser_h} Parser_GetNextDocument_Str REGEX "GetNextDocument")
  file(STRINGS ${Node_h}   Node_GetType_Str REGEX "GetType")

  # Integer XYZ_VERSION_NUMBER is typically (10000*(MAJOR_VERSION) + 100*(MINOR_VERSION) + (PATCH_VERSION))
  # dotted  XYZ_VERSION_STRING is "MAJOR_VERSION.MINOR_VERSION.PATCH_VERSION"
  # We usually only need VERSION_NUMBER, so the preprocessor can do integer arithmetic to determine version
  if(Parser_GetNextDocument_Str)
    if(Node_GetType_Str)
      set(YamlCpp_MAJOR_VERSION 0)
      set(YamlCpp_MINOR_VERSION 2)
      set(YamlCpp_PATCH_VERSION 5)
      set(YamlCpp_VERSION_NUMBER 205)
    else()
      set(YamlCpp_MAJOR_VERSION 0)
      set(YamlCpp_MINOR_VERSION 3)
      set(YamlCpp_PATCH_VERSION 0)
      set(YamlCpp_VERSION_NUMBER 300)
    endif()
  else()
    set(YamlCpp_MAJOR_VERSION 0)
    set(YamlCpp_MINOR_VERSION 6)
    set(YamlCpp_PATCH_VERSION 0)
    set(YamlCpp_VERSION_NUMBER 600)
  endif()
  message(DEBUG " YAMLCPP_VERSION_NUMBER: ${YamlCpp_VERSION_NUMBER}")
  add_compile_options(-DYAMLCPP_VERSION_NUMBER=${YamlCpp_VERSION_NUMBER})
endif()

set(YamlCpp_FOUND ${YAMLCPP_FOUND})
