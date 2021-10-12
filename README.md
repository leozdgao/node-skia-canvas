<div align="center">

<image src="https://z3.ax1x.com/2021/10/12/5eGh36.png" width=300 />

`node-skia-canvas` is a node binding for [skia](https://skia.org/) with <b>Node-API</b>, and expose [standard Canvas API](https://developer.mozilla.org/zh-CN/docs/Web/API/Canvas_API) for using.


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

const canvas = createCanvas(256, 256)
const ctx = canvas.getContext('2d')

ctx.fillStyle = '#FFF'
ctx.fillRect(0, 0, 256, 256)

ctx.fillStyle = '#4285F4'
ctx.fillRect(10, 10, 100, 160)

ctx.fillStyle = '#0F9D58'
ctx.beginPath()
ctx.arc(180, 50, 25, 0, 2 * Math.PI)
ctx.closePath()
ctx.fill()

ctx.fillStyle = '#DB4437'
ctx.beginPath()
ctx.ellipse(100, 160, 50, 80, 0, 0, 2 * Math.PI)
ctx.closePath()
ctx.fill()

ctx.lineWidth = 5
ctx.strokeStyle = '#F4B400'
ctx.strokeRect(80, 50, 100, 160)
```

And get result:

![5uiU2R.png](https://z3.ax1x.com/2021/10/13/5uiU2R.png)

You could run example by the script in `scripts` folder:

```bash
node scripts/run_example.js skia_showcase
```

## API Documentation

Implement the Canvas API and have some extension:


## License

MIT
