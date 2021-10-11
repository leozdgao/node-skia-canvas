<div align="center">

<image src="https://z3.ax1x.com/2021/10/12/5eGh36.png" width=400 />

`node-skia-canvas` is a node binding for [skia](https://skia.org/) with <b>Node-API</b>, and expose API [standard Canvas API](https://developer.mozilla.org/zh-CN/docs/Web/API/Canvas_API) for using.


[![xx](https://img.shields.io/github/workflow/status/leozdgao/node-skia-canvas/CI)]()
[![badge](https://img.shields.io/npm/v/node-skia-canvas.svg?maxAge=300&color=2254f4)](./package.json)
[![badge](https://img.shields.io/badge/code_style-standard-brightgreen.svg)](https://standardjs.com)
[![MIT](https://img.shields.io/npm/l/node-skia-canvas)]()

</div>

## Introduction

There is already a Canvas works on node, but here is some enhancement, and take this package as a replacemnt of `node-canvas`:

* Using Node-API to solve the ABI issue
* Some skia module extension, for example: skparagraph, pathkit, sklottie
* Working in node **worker-thread** to provide the possibility of multi-thread processing (Working in progress)
* GPU context support (Working in progress)

The project is written by C++ style API with `node-addon-api`, and built with `cmake-js`, and use `prebuild` and `prebuild-install` to prevent user enviroment building.

Consider about the complex host environment, so this package decide to let skia deps be static linked, compiling features could check the build script `scripts/build_tools/build_skia.js` in this project.

## Install

```bash
npm install --save node-canvas-skia
```

## Example

```js
const { createCanvas } = require('node-skia-canvas')
```

## API Documentation

Implement the Canvas API and have some extension:


## License

MIT
