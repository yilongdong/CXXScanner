# tudumper

[![standard-readme compliant](https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

输入一条编译指令以及C++源文件，对编译指令对应的翻译转换单元的代码进行AST分析，并将分析结果输出为json分析模型

TODO: Fill out this long description.

## Table of Contents

- [Install](#install)
- [Usage](#usage)
- [API](#api)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Install

### 安装vcpkg
[vcpkg](https://vcpkg.io/en/index.html)
```shell
# vcpkg integrate bash
# vcpkg integrate x-fish
vcpkg integrate zsh
```

### 安装ninja
```shell
brew install ninja
```

### 编译
```shell

cmake -B build/debug -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$(dirname $(which vcpkg))/scripts/buildsystems/vcpkg.cmake
cmake --build build/debug --parallel
# cd build/debug/
# ninja install
```
或者运行脚本
```shell
./build.sh
# cd build/debug/
# ninja install
```

## Usage

```shell
./build/debug/tudumper
# tudumper
```

## API

## Maintainers

[@yilongdong](https://github.com/yilongdong)

## Contributing

PRs accepted.

Small note: If editing the README, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

MIT © 2023 yilongdong
