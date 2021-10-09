const { Canvas: NativeCanvas } = require('bindings')('node-skia')
const CanvasRenderingContext2D = require('./CanvasRenderingContext2D')

class Canvas {
  constructor (width, height) {
    this.nativeCanvas = new NativeCanvas(width, height)
  }

  get width () {
    return this.nativeCanvas.width
  }

  set width (val) {
    this.nativeCanvas.width = val
  }

  get height () {
    return this.nativeCanvas.height
  }

  set height (val) {
    this.nativeCanvas.height = val
  }

  getContext (ctx) {
    if (ctx === '2d') {
      return CanvasRenderingContext2D.getFromCanvas(this.nativeCanvas)
    }

    return null
  }

  toBuffer (...args) {
    return this.nativeCanvas.toBuffer(...args)
  }
}

module.exports = Canvas
