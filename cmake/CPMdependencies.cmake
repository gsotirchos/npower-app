include("${CMAKE_SOURCE_DIR}/cmake/AddCPM.cmake")

## CPM dependencies
CPMAddPackage(
    GITHUB_REPOSITORY gsotirchos/npower-sensors
    GIT_TAG master
)
