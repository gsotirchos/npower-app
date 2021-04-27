include("${CMAKE_SOURCE_DIR}/cmake/AddCPM.cmake")

## CPM dependencies
CPMAddPackage(
    GITHUB_REPOSITORY 7555G/npower-sensors
    GIT_TAG master
)
