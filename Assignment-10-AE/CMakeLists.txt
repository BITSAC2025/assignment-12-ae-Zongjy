add_library(a10lib AELib.cpp AEHelper.cpp)
target_link_libraries(a10lib PRIVATE
        ${Z3_LIBRARIES}
        )

add_executable(a10test A10-test.cpp)
target_link_libraries(a10test PRIVATE
        ${SVF_LIB}
        ${LLVM_LIB}
        ${Z3_LIBRARIES}
        a10lib
        )
set_target_properties(a10test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})