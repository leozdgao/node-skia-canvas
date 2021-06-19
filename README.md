# node-skia
Canvas work on node.js base skia.

https://skia.org/user/build

deps for env:
* libjpeg-turbo
* libpng
* libwebp
* icu4c
* harfbuzz

## Setup

#### Node header files
This project build with cmake-js, so if your node header files has never been installed, you could command below to install header fils first:

```bash
npx node-gyp install
```

#### Cmake



#### Build skia
This project deeply depends on skia, it needs to be built first. To build skia, you should have ninja first, to install ninja, check https://ninja-build.org/ please. Then you could use command:

```bash
cd skia
python2 tools/git-sync-deps
bin/gn gen out/Debug
ninja -C out/Debug
```

#### Use docker image

