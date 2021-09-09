const { Canvas: NativeCanvas } = require('bindings')('node-skia')
const CanvasRenderingContext2D = require('./CanvasRenderingContext2D')
const { createProxyToNative } = require('./utils')

class Canvas {
  constructor (width, height) {
    this.nativeCanvas = new NativeCanvas(width, height)
  }

  getContext (ctx) {
    if (ctx === '2d') {
      return CanvasRenderingContext2D.getFromCanvas(this.nativeCanvas)
    }

    return null
  }
}

module.exports = createProxyToNative(
  Canvas,
  (target, prop) => { return target.nativeCanvas[prop] },
  (target, prop, value) => { target.nativeCanvas[prop] = value }
)
