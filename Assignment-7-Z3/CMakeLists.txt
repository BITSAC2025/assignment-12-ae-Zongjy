add_library(a8lib Z3Mgr.cpp)
target_link_libraries(a8lib PRIVATE
        ${Z3_LIBRARIES}
        )

add_executable(z3tests Z3Tests.cpp)
target_link_libraries(z3tests PRIVATE
        ${SVF_LIB}
        ${LLVM_LIB}
        ${Z3_LIBRARIES}
        a8lib
        )
set_target_properties(z3tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})