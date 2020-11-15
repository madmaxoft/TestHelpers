project (TestHelpers LANGUAGES CXX)





add_library(TestHelpers STATIC
	TestHelpers.cpp
	TestHelpers.h
)
target_link_libraries(TestHelpers fmt-header-only)
target_include_directories(TestHelpers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
