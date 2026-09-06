add_executable(
    bridge_tests
    core/GameStateTests.cpp
)

target_link_libraries(
    bridge_tests
    PRIVATE
        bridge_core
)

add_test(
    NAME bridge_tests
    COMMAND bridge_tests
)
