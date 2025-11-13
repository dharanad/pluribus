BUILD_TYPE=Release

BUILD_DIR=build

CMAKE_FLAGS := -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

ifndef USE_CCACHE
ifneq ($(shell which ccache), )
USE_CCACHE=-DCMAKE_CXX_COMPILER_LAUNCHER=ccache
endif
endif

.PHONY: all submodules cmake build release clean

all: release

submodules:
	git submodule sync --recursive
	git submodule update --init --recursive

cmake: submodules
	mkdir -p $(BUILD_DIR) && \
	cmake \
		-B $(BUILD_DIR) \
		$(CMAKE_FLAGS) \
		$(USE_CCACHE)

build: cmake
	cmake --build $(BUILD_DIR) -- -j$(shell nproc)

release: build

clean:
	rm -rf $(BUILD_DIR)