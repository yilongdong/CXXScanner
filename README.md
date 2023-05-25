# CXXScanner

[![standard-readme compliant](https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)
使用LibTooling对C++代码进行ATS分析获取项目的分析模型文件。
分析模型包含类模型，方法模型，调用关系，代码行数信息，圈复杂度等等。

## Table of Contents

- [Install](#install)
- [Usage](#usage)
- [API](#api)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Install

### 安装cmake
略
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

./build.sh
ln -s /usr/local/lib/clang/  ./install/lib/clang # 如果运行分析时输出错误信息，大概率是因为没有做这一步
# 接着编辑toml配置文件，最后运行
./install/bin/CXXScanner
```


https://github.com/sourcegraph/lsif-clang
If you get missing header warnings (possible due to divergence between lsif-clang and Apple Clang), use an extra flag:
--extra-arg="-resource-dir=$(clang -print-resource-dir)" compile_commands.json
## API

## Maintainers

[@yilongdong](https://github.com/yilongdong)

## Contributing

PRs accepted.

Small note: If editing the README, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

MIT © 2023 yilongdong
