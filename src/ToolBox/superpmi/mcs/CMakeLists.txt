project(mcs)

add_definitions(-DFEATURE_NO_HOST)
add_definitions(-DSELF_NO_HOST)

if(WIN32)
  #use static crt
  add_definitions(-MT)
endif(WIN32)

include_directories(.)
include_directories(../superpmi-shared)

set(MCS_SOURCES
    commandline.cpp
    mcs.cpp
    verbasmdump.cpp
    verbconcat.cpp
    verbdump.cpp
    verbdumpmap.cpp
    verbdumptoc.cpp
    verbfracture.cpp
    verbildump.cpp
    verbinteg.cpp
    verbmerge.cpp
    verbremovedup.cpp
    verbstat.cpp
    verbstrip.cpp
    verbtoc.cpp
    ../superpmi-shared/asmdumper.cpp
    ../superpmi-shared/callutils.cpp
    ../superpmi-shared/compileresult.cpp
    ../superpmi-shared/errorhandling.cpp
    ../superpmi-shared/logging.cpp
    ../superpmi-shared/mclist.cpp
    ../superpmi-shared/methodcontext.cpp
    ../superpmi-shared/methodcontextiterator.cpp
    ../superpmi-shared/methodcontextreader.cpp
    ../superpmi-shared/simpletimer.cpp
    ../superpmi-shared/spmiutil.cpp
    ../superpmi-shared/tocfile.cpp
    ../superpmi-shared/typeutils.cpp
    ../superpmi-shared/spmidumphelper.cpp
)

add_precompiled_header(
    standardpch.h
    ../superpmi-shared/standardpch.cpp
    MCS_SOURCES
)

add_executable(mcs
    ${MCS_SOURCES}
)

if(CLR_CMAKE_PLATFORM_UNIX)
    target_link_libraries(mcs
        utilcodestaticnohost
        mscorrc_debug
        coreclrpal
        palrt
    )
else()
    target_link_libraries(mcs
        advapi32.lib
        ${STATIC_MT_CRT_LIB}
        ${STATIC_MT_CPP_LIB}
    )

    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mcs.pdb DESTINATION PDB)
endif(CLR_CMAKE_PLATFORM_UNIX)

install (TARGETS mcs DESTINATION .)
