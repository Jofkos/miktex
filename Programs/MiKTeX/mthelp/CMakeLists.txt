## CMakeLists.txt                                       -*- CMake -*-
##
## Copyright (C) 2006-2022 Christian Schenk
## 
## This file is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation; either version 2, or (at your
## option) any later version.
## 
## This file is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this file; if not, write to the Free Software
## Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
## USA.

set(MIKTEX_CURRENT_FOLDER "${MIKTEX_IDE_MIKTEX_PROGRAMS_FOLDER}/${MIKTEX_PROG_NAME_MTHELP}")

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}
)

if(MIKTEX_NATIVE_WINDOWS)
  add_definitions(
    -DUNICODE
    -D_UNICODE
  )
endif()

set(mthelp_sources
  ${CMAKE_CURRENT_BINARY_DIR}/template.html.h
  mthelp-version.h
  mthelp.cpp
)

add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/template.html.h
  COMMAND
    bintoc
    templateHtml
    < ${CMAKE_CURRENT_SOURCE_DIR}/template.html
    > ${CMAKE_CURRENT_BINARY_DIR}/template.html.h
  MAIN_DEPENDENCY
    template.html
  DEPENDS
    bintoc
  VERBATIM
)

if(MIKTEX_NATIVE_WINDOWS)
  list(APPEND mthelp_sources
    ${MIKTEX_COMMON_MANIFEST}
    mthelp.rc
  )
endif()

add_executable(${MIKTEX_PROG_NAME_MTHELP} ${mthelp_sources})

set_property(TARGET ${MIKTEX_PROG_NAME_MTHELP} PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})

if(USE_SYSTEM_FMT)
  target_link_libraries(${MIKTEX_PROG_NAME_MTHELP} MiKTeX::Imported::FMT)
else()
  target_link_libraries(${MIKTEX_PROG_NAME_MTHELP} ${fmt_dll_name})
endif()

target_link_libraries(${MIKTEX_PROG_NAME_MTHELP}
  ${app_dll_name}
  ${core_dll_name}
  ${mpm_dll_name}
  miktex-popt-wrapper
)

install(TARGETS ${MIKTEX_PROG_NAME_MTHELP}
    ARCHIVE DESTINATION "${MIKTEX_LIBRARY_DESTINATION_DIR}"
    LIBRARY DESTINATION "${MIKTEX_LIBRARY_DESTINATION_DIR}"
    RUNTIME DESTINATION "${MIKTEX_BINARY_DESTINATION_DIR}"
)
