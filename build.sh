# shellcheck disable=SC2046
# shellcheck disable=SC2215
cmake -B build/debug -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug \                                                                                           [23:55:14]
  -DCMAKE_TOOLCHAIN_FILE=$(dirname $(which vcpkg))/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_INSTALL_PREFIX=$(pwd)/install
cmake --build build/debug --parallel
