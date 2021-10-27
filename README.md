<div align="center">

<image src="https://z3.ax1x.com/2021/10/12/5eGh36.png" width=300 />

`node-skia-canvas` is a W3C Canvas API implementation for Node.js. It is based on Google [skia](https://skia.org/), and written with <b>Node-API</b>, provide similar canvas development just like on the browser.

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

Implement the Standard Canvas API:

* `Canvas`
* `CanvasRenderingContext2D`
* `CanvasGradient` (not expose)
* `CanvasPattern` (not expose)
* `DOMMatrix`
* `TextMetrix` (not expose)

And also provide some useful module:

* `Image`, for image use
* `registerFont`, for font management

### Canvas

Differ from the creation of canvas in DOM, we create canvas by instantiation `Canvas`, or use `createCanvas`:

```js
// Instantication
const canvas = new Canvas(300, 300)
// or use factory method
const canvas = createCanvas(300, 300)
```

Properties:
* [**width**][canvas_width]
* [**height**][canvas_height]

Methods:
* [getContext()][getContext]
* [toBuffer() ✨][toBuffer]
* [toDataURL()][toDataURL_mdn]

[canvas_width]: https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement/width
[canvas_height]: https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement/height
[getContext]: https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement/getContext
[toBuffer]: #tobufferformat
[toDataURL_mdn]: https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement/toDataURL

##### ✨ `toBuffer(format)`


### CanvasRenderingContext2D
Use it just like in browser, the API is almost the same (only support `2d` for now, `gl` context is working in progress):

```js
const canvas = new Canvas(300, 300)
const ctx = canvas.getContext('2d')

ctx.fillStyle = '#FFF'
ctx.fillRect(0, 0, 256, 256)
```

Properties:
* [fillStyle][fillStyle]
* [font][font]
* [globalAlpha][globalAlpha]
* [globalCompositeOperation][globalCompositeOperation]
* [imageSmoothingEnabled][imageSmoothingEnabled]
* [lineCap][lineCap]
* [lineDashOffset][lineDashOffset]
* [lineJoin][lineJoin]
* [lineWidth][lineWidth]
* [miterLimit][miterLimit]
* [shadowBlur][shadowBlur]
* [shadowColor][shadowColor]
* [shadowOffsetX][shadowOffsetX]
* [shadowOffsetY][shadowOffsetY]
* [strokeStyle][strokeStyle]
* [textAlign][textAlign]
* [textBaseline][textBaseline]

Methods:
* [arc()][arc()]
* [arcTo()][arcTo()]
* [beginPath()][beginPath()]
* [bezierCurveTo()][bezierCurveTo()]
* [clearRect()][clearRect()]
* [clip()][clip()]
* [closePath()][closePath()]
* [createImageData()][createImageData()]
* [createLinearGradient()][createLinearGradient()]
* [createPattern()][createPattern()]
* [createRadialGradient()][createRadialGradient()]
* [drawImage()][drawImage()]
* [ellipse()][ellipse()]
* [fill()][fill()]
* [fillRect()][fillRect()]
* [fillText() ✨][fillText()]
* [getImageData()][getImageData()]
* [getLineDash()][getLineDash()]
* [getTransform()][getTransform()]
* [lineTo()][lineTo()]
* [measureText()][measureText()]
* [moveTo()][moveTo()]
* [putImageData()][putImageData()]
* [quadraticCurveTo()][quadraticCurveTo()]
* [rect()][rect()]
* [restore()][restore()]
* [rotate()][rotate()]
* [save()][save()]
* [scale()][scale()]
* [setLineDash()][setLineDash()]
* [setTransform()][setTransform()]
* [stroke()][stroke()]
* [strokeRect()][strokeRect()]
* [strokeText()][strokeText()]
* [transform()][transform()]
* [translate()][translate()]

[fillStyle]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fillStyle
[font]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/font
[globalAlpha]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalAlpha
[globalCompositeOperation]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalCompositeOperation
[imageSmoothingEnabled]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/imageSmoothingEnabled
[lineCap]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/lineCap
[lineDashOffset]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/lineDashOffset
[lineJoin]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/lineJoin
[lineWidth]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/lineWidth
[miterLimit]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/miterLimit
[shadowBlur]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/shadowBlur
[shadowColor]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/shadowColor
[shadowOffsetX]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/shadowOffsetX
[shadowOffsetY]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/shadowOffsetY
[strokeStyle]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/strokeStyle
[textAlign]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/textAlign
[textBaseline]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/textBaseline
[arc()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/arc
[arcTo()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/arcTo
[beginPath()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/beginPath
[bezierCurveTo()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/bezierCurveTo
[clearRect()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/clearRect
[clip()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/clip
[closePath()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/closePath
[createConicGradient()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/createConicGradient
[createImageData()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/createImageData
[createLinearGradient()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/createLinearGradient
[createPattern()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/createPattern
[createRadialGradient()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/createRadialGradient
[drawFocusIfNeeded()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/drawFocusIfNeeded
[drawImage()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/drawImage
[ellipse()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/ellipse
[fill()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fill
[fillRect()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fillRect
[fillText()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fillText
[getImageData()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/getImageData
[getLineDash()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/getLineDash
[getTransform()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/getTransform
[isPointInPath()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/isPointInPath
[isPointInStroke()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/isPointInStroke
[lineTo()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/lineTo
[measureText()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/measureText
[moveTo()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/moveTo
[putImageData()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/putImageData
[quadraticCurveTo()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/quadraticCurveTo
[rect()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/rect
[resetTransform()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/resetTransform
[restore()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/restore
[rotate()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/rotate
[save()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/save
[scale()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/scale
[setLineDash()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/setLineDash
[setTransform()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/setTransform
[stroke()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/stroke
[strokeRect()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/strokeRect
[strokeText()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/strokeText
[transform()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/transform
[translate()]: https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/translate

##### ✨ `fillText(x, y, text, [maxWidth])`
Provide extra parameter `maxWidth` to provide word-wrap typography, it is documented on [MDN](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/fillText), but not implemented in most browser, but this library implemented this extra parameter.

### Image
Here is a helper class for image usage, and used in `drawImage` as parameter:

```js
const fs = require('fs')
const { Image } = require('node-skia-canvas')

const imgData = fs.readFileSync('./examples/leize.jpeg')
const image = new Image()
image.src = imgData

const canvas = createCanvas(image.width, image.height)
const ctx = canvas.getContext('2d')
ctx.drawImage(img, 0, 0, image.width, image.height)
```

Properties
* [width](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/img#attr-width)
* [height](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/img#attr-height)
* [src](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/img#attr-src)

#### About loadImage
There is an API `loadImage` provided by `node-canvas`, but you may have your own favorite network library like: `urllib`、`universal-fetch`, etc. So this `loadImage` helper api may not that helpful, so it will not be provided currently. 

### Font Management
We provide font management API and compat with `node-canvas`:

##### ✨ `registerFont(path, [{ family, weight }])`

Register font to canvas, if `family` or `weight` is not provided, it will be parsed from font file automatically.

```js
const { registerFont } = require('node-skia-canvas')

registerFont(path.join(__dirname, './OswaldRegular.ttf'), {
  family: 'Oswald'
})
registerFont(path.join(__dirname, './OswaldLight.ttf'))
registerFont(path.join(__dirname, './OswaldBold.ttf'))
```

## Acknowledgements

Thanks to `node-canvas` for their test cases.

## License

MIT
