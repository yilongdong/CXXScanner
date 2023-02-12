# shellcheck disable=SC2046
cmake -B build/debug -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE=$(dirname $(which vcpkg))/scripts/buildsystems/vcpkg.cmake
cmake --build build/debug --parallel
