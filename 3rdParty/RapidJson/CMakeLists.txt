#RapidJSON CMakeLists.txt
#correct definition of this library.
add_library(rapidjson INTERFACE)
set(JSONIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)