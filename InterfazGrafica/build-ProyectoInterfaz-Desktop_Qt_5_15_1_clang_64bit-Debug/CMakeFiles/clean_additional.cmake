# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ProyectoInterfaz_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ProyectoInterfaz_autogen.dir/ParseCache.txt"
  "ProyectoInterfaz_autogen"
  )
endif()
