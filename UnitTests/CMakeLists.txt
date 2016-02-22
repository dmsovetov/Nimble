# Add include directories
include_directories(..)

# Disable the CRT secure warnings
if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Add unit tests executable
add_executable(UnitTests Tests.cpp)