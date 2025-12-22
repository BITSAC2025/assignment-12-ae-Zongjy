add_library(a9lib AEMgr.cpp)
target_link_libraries(a9lib PRIVATE
        ${Z3_LIBRARIES}
        )

add_executable(aetest AETest.cpp)
target_link_libraries(aetest PRIVATE
        ${SVF_LIB}
        ${LLVM_LIB}
        ${Z3_LIBRARIES}
        a9lib
        )
set_target_properties(aetest PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})