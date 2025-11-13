include_guard(GLOBAL)

set(VELOX_ANTLR4-RUNTIME_VERSION 4.13.2)
set(
  VELOX_ANTLR4-RUNTIME_BUILD_SHA256_CHECKSUM
  0ed13668906e86dbc0dcddf30fdee68c10203dea4e83852b4edb810821bee3c4
)
set(
  VELOX_ANTLR4-RUNTIME_SOURCE_URL
  "https://www.antlr.org/download/antlr4-cpp-runtime-${VELOX_ANTLR4-RUNTIME_VERSION}-source.zip"
)

velox_resolve_dependency_url(ANTLR4-RUNTIME)

message(STATUS "Building antlr4-runtime from source")

FetchContent_Declare(
  antlr4-runtime
  URL ${VELOX_ANTLR4-RUNTIME_SOURCE_URL}
  URL_HASH ${VELOX_ANTLR4-RUNTIME_BUILD_SHA256_CHECKSUM}
)

set(ANTLR4_INSTALL OFF CACHE BOOL "Disable installation for antlr4-runtime")
set(ANTLR_BUILD_CPP_TESTS OFF CACHE BOOL "Disable tests for antlr4-runtime")

FetchContent_MakeAvailable(antlr4-runtime)