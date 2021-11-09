const { Canvas: NativeCanvas } = require('bindings')('node-skia')
const CanvasRenderingContext2D = require('./CanvasRenderingContext2D')

class Canvas {
  constructor (width, height) {
    if (typeof width !== 'number' || width <= 0) {
      throw new Error('construct canvas error: width should be a number and greater than 0')
    }

    if (typeof height !== 'number' || height <= 0) {
      throw new Error('construct canvas error: height should be a number and greater than 0')
    }

    this.nativeCanvas = new NativeCanvas(width, height)
    this._ctx2d = null;
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
      if (this._ctx2d == null) {
        this._ctx2d = CanvasRenderingContext2D.getFromCanvas(this.nativeCanvas)
      }

      return this._ctx2d
    }

    return null
  }

  toBuffer (...args) {
    return this.nativeCanvas.toBuffer(...args)
  }
}

module.exports = Canvas
