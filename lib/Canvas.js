const { Canvas: NativeCanvas } = require('bindings')('node-skia')
const CanvasRenderingContext2D = require('./CanvasRenderingContext2D')

class Canvas {
  static createCanvas (width, height) {
    return new Canvas(width, height)
  }

  constructor (width, height) {
    this.nativeCanvas = new NativeCanvas(width, height)
  }

  get width () {
    return this.nativeCanvas.width
  }

  get height () {
    return this.nativeCanvas.height
  }

  getContext (ctx) {
    if (ctx === '2d') {
      return CanvasRenderingContext2D.getFromCanvas(this.nativeCanvas)
    }

    return null
  }

  toBuffer (mimeType = 'image/png') {
    return this.nativeCanvas.toBuffer(mimeType)
  }
}

module.exports = Canvas
