find_path(YamlCpp_INCLUDE_DIR yaml.h PATH_SUFFIXES yaml-cpp)

if(YamlCpp_INCLUDE_DIR-NOTFOUND)
   message(ERROR " could not find YamlCpp_INCLUDE_DIR: ${YamlCpp_INCLUDE_DIR}")
else()
   if(NOT EXISTS "${YamlCpp_INCLUDE_DIR}")
      message(STATUS "YamlCpp_INCLUDE_DIR: ${YamlCpp_INCLUDE_DIR} NOT FOUND. Trying...")
      foreach(dir ${CMAKE_PREFIX_PATH};${CMAKE_INCLUDE_PATH})
         if(EXISTS "${dir}/yaml.h")
            set(YamlCpp_INCLUDE_DIR ${dir})
         elseif(EXISTS "${dir}/include/yaml.h" OR EXISTS "${dir}/include/yaml-cpp/yaml.h")
            set(YamlCpp_INCLUDE_DIR "${dir}/include")
         endif()
         if(EXISTS "${YamlCpp_INCLUDE_DIR}")
            message(STATUS "YamlCpp_INCLUDE_DIR: ${YamlCpp_INCLUDE_DIR} FOUND")
         endif()
      endforeach()
   endif()
endif()

find_library(YamlCpp_LIBRARY_RELEASE NAMES yaml-cpp yaml)
find_library(YamlCpp_LIBRARY_DEBUG NAMES yaml-cppd)
if(YamlCpp_LIBRARY_RELEASE-NOTFOUND)
   message(ERROR " could not find YamlCpp_LIBRARY_RELEASE: ${YamlCpp_LIBRARY_RELEASE}")
else()
   if(NOT EXISTS "${YamlCpp_LIBRARY_RELEASE}")
      message(STATUS "YamlCpp_LIBRARY_RELEASE: ${YamlCpp_LIBRARY_RELEASE} NOT FOUND. Trying...")
      foreach(dir ${CMAKE_PREFIX_PATH};${CMAKE_LIBRARY_PATH})
         #message(STATUS "YamlCpp_LIBRARY_RELEASE: ${dir}/${YamlCpp_LIBRARY_RELEASE}")
         if(EXISTS "${dir}/${YamlCpp_LIBRARY_RELEASE}")
            set(YamlCpp_LIBRARY_RELEASE "${dir}/${YamlCpp_LIBRARY_RELEASE}")
         endif()
         if(EXISTS "${YamlCpp_LIBRARY_RELEASE}")
            message(STATUS "YamlCpp_LIBRARY_RELEASE: ${YamlCpp_LIBRARY_RELEASE} FOUND")
         endif()
      endforeach()
   endif()
endif()






include(SelectLibraryConfigurations)
select_library_configurations(YamlCpp) #sets YamlCpp_LIBRARY using YamlCpp_LIBRARY_DEBUG and YamlCpp_LIBRARY_RELEASE

mark_as_advanced(YamlCpp_INCLUDE_DIR)
set(YamlCpp_INCLUDE_DIRS ${YamlCpp_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YamlCpp REQUIRED_VARS YamlCpp_INCLUDE_DIR YamlCpp_LIBRARY)
if(YAMLCPP_FOUND)
   find_file(Yaml_h NAMES yaml.h PATH_SUFFIXES yaml-cpp)
   if(NOT EXISTS ${Yaml_h})
      message(ERROR " could not find YamlCpp header ${Yaml_h}")
   endif()
   find_file(Node_h NAMES node.h PATH_SUFFIXES yaml-cpp)
   if(NOT EXISTS ${Node_h})
      message(ERROR " could not find YamlCpp header ${Node_h}")
   endif()
   find_file(Parser_h NAMES parser.h PATH_SUFFIXES yaml-cpp)
   if(NOT EXISTS ${Parser_h})
      message(ERROR " could not find YamlCpp header ${Parser_h}")
   endif()

  message(DEBUG " YamlCPP headers ${Yaml_h} ${Parser_h} ${Node_h}")
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
      set(YamlCpp_VERSION_STRING "0.2.5")
    else()
      set(YamlCpp_MAJOR_VERSION 0)
      set(YamlCpp_MINOR_VERSION 3)
      set(YamlCpp_PATCH_VERSION 0)
      set(YamlCpp_VERSION_NUMBER 300)
      set(YamlCpp_VERSION_STRING "0.3.0")
    endif()
  else()
    set(YamlCpp_MAJOR_VERSION 0)
    set(YamlCpp_MINOR_VERSION 6)
    set(YamlCpp_PATCH_VERSION 0)
    set(YamlCpp_VERSION_NUMBER 600)
    set(YamlCpp_VERSION_STRING "0.6.0")
  endif()
  message(DEBUG " YAMLCPP_VERSION_NUMBER: ${YamlCpp_VERSION_NUMBER}")
  add_compile_options(-DYAMLCPP_VERSION_NUMBER=${YamlCpp_VERSION_NUMBER})
endif()

if(YAMLCPP_FOUND)
   message(STATUS "Found YamlCpp version ${YamlCpp_VERSION_STRING}")
endif()

set(YamlCpp_FOUND ${YAMLCPP_FOUND})
