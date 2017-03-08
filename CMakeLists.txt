# Set the minimum CMake version
cmake_minimum_required(VERSION 3.2)

# Declare the project
project(Nimble)

# Available options
option(NIMBLE_BUILD_TESTS "Build Nimble unit tests" OFF)

# Build the source files list
file(GLOB BV_SRCS "Bv/*.h")
file(GLOB COLOR_SRCS "Color/*.h")
file(GLOB MATH_SRCS "Math/*.h")
file(GLOB PREPROCESSOR_SRCS "Preprocessor/*.h")
file(GLOB POINTERS_SRCS "Pointers/*.h")
file(GLOB CONTAINERS_SRCS "Containers/*.h")
file(GLOB ALLOCATORS_SRCS "Allocators/*.h")
file(GLOB CLOSURE_SRCS "Closure/*.h")
file(GLOB TYPE_TRAITS_SRCS "TypeTraits/*.h")
file(GLOB DEBUG_SRCS "Debug/*.h")
file(GLOB HASHING_SRCS "Hashing/*.h")
file(GLOB PARSER_SRCS "Parser/*.h")
file(GLOB PATTERNS_SRCS "Patterns/*.h")
file(GLOB STRINGS_SRCS "Strings/*.h")
file(GLOB ROOT_SRCS "*.h")

# Add files to source groups
source_group("Code\\Bv" FILES ${BV_SRCS})
source_group("Code\\Debug" FILES ${DEBUG_SRCS})
source_group("Code\\Color" FILES ${COLOR_SRCS})
source_group("Code\\Math" FILES ${MATH_SRCS})
source_group("Code\\Preprocessor" FILES ${PREPROCESSOR_SRCS})
source_group("Code\\Pointers" FILES ${POINTERS_SRCS})
source_group("Code\\Containers" FILES ${CONTAINERS_SRCS})
source_group("Code\\Allocators" FILES ${ALLOCATORS_SRCS})
source_group("Code\\Closure" FILES ${CLOSURE_SRCS})
source_group("Code\\TypeTraits" FILES ${TYPE_TRAITS_SRCS})
source_group("Code\\Hashing" FILES ${HASHING_SRCS})
source_group("Code\\Parser" FILES ${PARSER_SRCS})
source_group("Code\\Patterns" FILES ${PATTERNS_SRCS})
source_group("Code\\Strings" FILES ${STRINGS_SRCS})
source_group("Code" FILES ${ROOT_SRCS})

# Add the headers-only target
add_custom_target(Nimble SOURCES
    ${BV_SRCS}
    ${COLOR_SRCS}
    ${MATH_SRCS}
    ${PREPROCESSOR_SRCS}
    ${POINTERS_SRCS}
    ${CONTAINERS_SRCS}
    ${ALLOCATORS_SRCS}
    ${ROOT_SRCS}
    ${CLOSURE_SRCS}
    ${TYPE_TRAITS_SRCS}
    ${DEBUG_SRCS}
    ${HASHING_SRCS}
    ${PARSER_SRCS}
    ${STRINGS_SRCS}
	${PATTERNS_SRCS}
    )

# Add unit tests target
if(NIMBLE_BUILD_TESTS)
    add_subdirectory(UnitTests)
endif()