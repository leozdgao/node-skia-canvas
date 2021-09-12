const { CanvasGradient } = require('bindings')('node-skia')
const { DOMMatrix } = require('./DOMMatrix')
const parseFont = require('./parseFont')
const { colorVectorToHex, createProxyToNative } = require('./utils')

class CanvasRenderingContext2D {
  static getFromCanvas (nativeCanvas) {
    const nativeContext = nativeCanvas.getContext('2d')
    return new ProxyCanvasRenderingContext2D(nativeContext)
  }

  constructor (nativeContext) {
    this.nativeContext = nativeContext

    this._fillStyle = null
    this._strokeStyle = null
  }

  get fillStyle () {
    return this._fillStyle || colorVectorToHex(this.nativeContext.fillStyle)
  }

  set fillStyle (val) {
    if (typeof val === 'string') {
      val = val.toLowerCase()
    }

    this._fillStyle = typeof val === 'string' ? null : val
    this.nativeContext.fillStyle = val
  }

  get strokeStyle () {
    return this._strokeStyle || colorVectorToHex(this.nativeContext.strokeStyle)
  }

  set strokeStyle (val) {
    if (typeof val === 'string') {
      val = val.toLowerCase()
    }

    this._strokeStyle = typeof val === 'string' ? null : val
    this.nativeContext.strokeStyle = val
  }

  get shadowColor () {
    return colorVectorToHex(this.nativeContext.shadowColor)
  }

  get font () {
    return this.nativeContext.font
  }

  set font (str) {
    this.nativeContext.font = parseFont(str)
  }

  get lineWidth () {
    return this.nativeContext.lineWidth
  }

  set lineWidth (num) {
    // filter 0, negative, Infinity and NaN
    if (typeof num !== 'number') {
      return
    }

    if (num <= 0 || Number.isNaN(num) || !Number.isFinite(num)) {
      return
    }

    this.nativeContext.lineWidth = num
  }

  createImageData (width, height) {
    if (typeof width === 'object') {
      const imgData = width
      width = imgData.width
      height = imgData.height
    }

    if (width <= 0) {
      throw new Error('width for createImageData should greater then zero')
    }

    if (height <= 0) {
      throw new Error('height for createImageData should greater then zero')
    }

    return this.nativeContext.createImageData(width, height)
  }

  createLinearGradient (x0, y0, x1, y1) {
    return new CanvasGradient('linear', x0, y0, x1, y1)
  }

  createRadialGradient (x0, y0, r0, x1, y1, r1) {
    return new CanvasGradient('radial', x0, y0, x1, y1, r0, r1)
  }

  ellipse (x, y, radiusX, radiusY, rotation, startAngle, endAngle, counterclockwise = false) {
    const tau = 2 * Math.PI
    let newStartAngle = startAngle % tau

    if (newStartAngle < 0) {
      newStartAngle += tau
    }

    const delta = newStartAngle - startAngle
    let newEndAngle = endAngle + delta

    if (!counterclockwise && (newEndAngle - newStartAngle) >= tau) {
      newEndAngle = newStartAngle + tau
    } else if (counterclockwise && (newStartAngle - newEndAngle) >= tau) {
      newEndAngle = newStartAngle - tau
    } else if (!counterclockwise && newStartAngle > newEndAngle) {
      newEndAngle = newStartAngle + (tau - (newStartAngle - newEndAngle) % tau)
    } else if (counterclockwise && newStartAngle < newEndAngle) {
      newEndAngle = newStartAngle - (tau - (newEndAngle - newStartAngle) % tau)
    }

    return this.nativeContext.ellipse(x, y, radiusX, radiusY, rotation, newStartAngle, newEndAngle, counterclockwise)
  }

  getTransform () {
    return new DOMMatrix(
      this.nativeContext.getTransform()
    )
  }

  setTransform (matrix) {
    if (matrix instanceof DOMMatrix) {
      this.nativeContext.setTransform(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f)
    }
  }

  setLineDash (pattern) {
    if (!Array.isArray(pattern)) {
      return
    }

    if (pattern.some(v => typeof v !== 'number')) {
      return
    }

    if (pattern.length % 2 === 1) {
      pattern = [
        ...pattern,
        ...pattern
      ]
    }

    return this.nativeContext.setLineDash(pattern)
  }

  rotate (angle) {
    return this.nativeContext.rotate(angle / Math.PI * 180)
  }
}

const ProxyCanvasRenderingContext2D = createProxyToNative(
  CanvasRenderingContext2D,
  (target, prop) => { return target.nativeContext[prop] },
  (target, prop, value) => { target.nativeContext[prop] = value },
  (target) => target.nativeContext
)

module.exports = ProxyCanvasRenderingContext2D
