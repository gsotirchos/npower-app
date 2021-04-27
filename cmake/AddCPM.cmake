set(CPM_URL "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake")

set(GET_CPM_SCRIPT "${CMAKE_BINARY_DIR}/cmake/get_cpm.cmake")

## download and include CPM
if(NOT EXISTS "${CPM_SCRIPT}")
    file(DOWNLOAD
        "${CPM_URL}" "${GET_CPM_SCRIPT}"
        SHOW_PROGRESS
    )
endif()

include("${GET_CPM_SCRIPT}")
