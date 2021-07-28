#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "seeta::seeta" for configuration ""
set_property(TARGET seeta::seeta APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(seeta::seeta PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libseeta.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS seeta::seeta )
list(APPEND _IMPORT_CHECK_FILES_FOR_seeta::seeta "${_IMPORT_PREFIX}/lib/libseeta.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
