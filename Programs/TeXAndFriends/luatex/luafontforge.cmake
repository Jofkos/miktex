## luafontforge.cmake
##
## Copyright (C) 2010-2023 Christian Schenk
## 
## This file is free software; the copyright holder gives
## unlimited permission to copy and/or distribute it, with or
## without modifications, as long as this notice is preserved.

configure_file(
    ff-config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ff-config.h
)

set(luafontforge_sources
    source/luafontloader/fontforge/fontforge/PfEd.h
    source/luafontloader/fontforge/fontforge/autohint.c
    source/luafontloader/fontforge/fontforge/baseviews.h
    source/luafontloader/fontforge/fontforge/clipnoui.c
    source/luafontloader/fontforge/fontforge/configure-pfaedit.h
    source/luafontloader/fontforge/fontforge/cvundoes.c
    source/luafontloader/fontforge/fontforge/dumppfa.c
    source/luafontloader/fontforge/fontforge/edgelist.h
    source/luafontloader/fontforge/fontforge/edgelist2.h
    source/luafontloader/fontforge/fontforge/encoding.c
    source/luafontloader/fontforge/fontforge/encoding.h
    source/luafontloader/fontforge/fontforge/featurefile.c
    source/luafontloader/fontforge/fontforge/fontforgevw.h
    source/luafontloader/fontforge/fontforge/fontviewbase.c
    source/luafontloader/fontforge/fontforge/fvcomposit.c
    source/luafontloader/fontforge/fontforge/fvfonts.c
    source/luafontloader/fontforge/fontforge/lookups.c
    source/luafontloader/fontforge/fontforge/lookups.h
    source/luafontloader/fontforge/fontforge/macbinary.c
    source/luafontloader/fontforge/fontforge/macenc.c
    source/luafontloader/fontforge/fontforge/mathconstants.c
    source/luafontloader/fontforge/fontforge/memory.c
    source/luafontloader/fontforge/fontforge/mm.c
    source/luafontloader/fontforge/fontforge/mm.h
    source/luafontloader/fontforge/fontforge/namelist.c
    source/luafontloader/fontforge/fontforge/noprefs.c
    source/luafontloader/fontforge/fontforge/nouiutil.c
    source/luafontloader/fontforge/fontforge/parsepfa.c
    source/luafontloader/fontforge/fontforge/parsettf.c
    source/luafontloader/fontforge/fontforge/parsettfatt.c
    source/luafontloader/fontforge/fontforge/pfaedit.h
    source/luafontloader/fontforge/fontforge/plugins.h
    source/luafontloader/fontforge/fontforge/psfont.h
    source/luafontloader/fontforge/fontforge/psread.c
    source/luafontloader/fontforge/fontforge/pua.c
    source/luafontloader/fontforge/fontforge/python.c
    source/luafontloader/fontforge/fontforge/scripting.h
    source/luafontloader/fontforge/fontforge/sd.h
    source/luafontloader/fontforge/fontforge/sfd1.c
    source/luafontloader/fontforge/fontforge/sfd1.h
    source/luafontloader/fontforge/fontforge/splinechar.c
    source/luafontloader/fontforge/fontforge/splinefill.c
    source/luafontloader/fontforge/fontforge/splinefont.c
    source/luafontloader/fontforge/fontforge/splinefont.h
    source/luafontloader/fontforge/fontforge/splineorder2.c
    source/luafontloader/fontforge/fontforge/splineoverlap.c
    source/luafontloader/fontforge/fontforge/splinerefigure.c
    source/luafontloader/fontforge/fontforge/splinesave.c
    source/luafontloader/fontforge/fontforge/splinesaveafm.c
    source/luafontloader/fontforge/fontforge/splinestroke.c
    source/luafontloader/fontforge/fontforge/splineutil.c
    source/luafontloader/fontforge/fontforge/splineutil2.c
    source/luafontloader/fontforge/fontforge/start.c
    source/luafontloader/fontforge/fontforge/stemdb.c
    source/luafontloader/fontforge/fontforge/stemdb.h
    source/luafontloader/fontforge/fontforge/tottf.c
    source/luafontloader/fontforge/fontforge/tottfgpos.c
    source/luafontloader/fontforge/fontforge/ttf.h
    source/luafontloader/fontforge/fontforge/ttfspecial.c
    source/luafontloader/fontforge/fontforge/uiinterface.h
    source/luafontloader/fontforge/fontforge/unicoderange.h
    source/luafontloader/fontforge/gutils/fsys.c
    source/luafontloader/fontforge/inc/basics.h
    source/luafontloader/fontforge/inc/gfile.h
    source/luafontloader/fontforge/inc/gimage.h
    source/luafontloader/fontforge/inc/intl.h
)

add_library(luatex-luafontforge-objects OBJECT ${luafontforge_sources})

prevent_all_warnings_on_bad_code_target(luatex-luafontforge-objects)

set_property(TARGET luatex-luafontforge-objects PROPERTY FOLDER ${MIKTEX_CURRENT_FOLDER})

target_compile_definitions(luatex-luafontforge-objects
    PRIVATE
        -DUSE_OUR_MEMORY=1
        -DX_DISPLAY_MISSING=1
        -D_NO_PYTHON=1
    PUBLIC  
        -DLUA_FF_LIB=1
)

target_include_directories(luatex-luafontforge-objects
    PRIVATE
        source/luafontloader/fontforge/fontforge
        source/luafontloader/fontforge/inc
)

target_link_libraries(luatex-luafontforge-objects
    PUBLIC
        ${core_dll_name}
        ${kpsemu_dll_name}
        ${lua53_target_name}
        ${w2cemu_dll_name}
        luatex-unilib-objects
)

if(MIKTEX_NATIVE_WINDOWS)
    target_link_libraries(luatex-luafontforge-objects
        PUBLIC
        ${utf8wrap_dll_name}
    )
endif()
