const { Canvas: NativeCanvas, CanvasPattern, CanvasGradient, Image, ImageData } = require('bindings')('node-skia')
const { DOMMatrix } = require('./DOMMatrix')
const parseFont = require('./parseFont')
const { colorVectorToHex } = require('./utils')

class CanvasRenderingContext2D {
  static getFromCanvas (nativeCanvas) {
    const nativeContext = nativeCanvas.getContext('2d')
    return new CanvasRenderingContext2D(nativeContext)
  }

  constructor (nativeContext) {
    this.nativeContext = nativeContext

    this._fillStyle = null
    this._strokeStyle = null
  }

  _normalizeRadians (radians) {
    radians = radians / Math.PI * 180

    if (Math.abs(radians) > 360) {
      radians = radians % 360
    }

    if (radians < 0) {
      radians += 360
    }
    
    return radians
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

  get font () {
    return this.nativeContext.font
  }

  set font (str) {
    this.nativeContext.font = parseFont(str)
  }

  get globalAlpha () {
    return this.nativeContext.globalAlpha
  }

  set globalAlpha (val) {
    this.nativeContext.globalAlpha = val
  }

  get imageSmoothingEnabled () {
    return this.nativeContext.imageSmoothingEnabled
  }

  set imageSmoothingEnabled (val) {
    this.nativeContext.imageSmoothingEnabled = val
  }

  get lineCap () {
    return this.nativeContext.lineCap
  }

  set lineCap (val) {
    this.nativeContext.lineCap = val
  }

  get lineDashOffset () {
    return this.nativeContext.lineDashOffset
  }

  set lineDashOffset (val) {
    this.nativeContext.lineDashOffset = val
  }

  get lineJoin () {
    return this.nativeContext.lineJoin
  }

  set lineJoin (val) {
    this.nativeContext.lineJoin = val
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

  get miterLimit () {
    return this.nativeContext.miterLimit
  }

  set miterLimit (val) {
    this.nativeContext.miterLimit = val
  }

  get shadowBlur () {
    return this.nativeContext.shadowBlur
  }

  set shadowBlur (val) {
    this.nativeContext.shadowBlur = val
  }

  get shadowColor () {
    return colorVectorToHex(this.nativeContext.shadowColor)
  }

  set shadowColor (val) {
    this.nativeContext.shadowColor = val
  }

  get shadowOffsetX () {
    return this.nativeContext.shadowOffsetX
  }

  set shadowOffsetX (val) {
    this.nativeContext.shadowOffsetX = val
  }

  get shadowOffsetY () {
    return this.nativeContext.shadowOffsetY
  }

  set shadowOffsetY (val) {
    this.nativeContext.shadowOffsetY = val
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

  get textAlign () {
    return this.nativeContext.textAlign
  }

  set textAlign (val) {
    this.nativeContext.textAlign = val
  }

  get textBaseline () {
    return this.nativeContext.textBaseline
  }

  set textBaseline (val) {
    this.nativeContext.textBaseline = val
  }

  arc (x, y, radius, startAngle, endAngle) {
    return this.nativeContext.arc(
      x, y, radius,
      this._normalizeRadians(startAngle),
      this._normalizeRadians(endAngle)
    )
  }

  arcTo (...args) {
    return this.nativeContext.arcTo(...args)
  }

  beginPath (...args) {
    return this.nativeContext.beginPath(...args)
  }

  bezierCurveTo (...args) {
    return this.nativeContext.bezierCurveTo(...args)
  }

  clearRect (...args) {
    return this.nativeContext.clearRect(...args)
  }

  clip (...args) {
    return this.nativeContext.clip(...args)
  }

  closePath (...args) {
    return this.nativeContext.closePath(...args)
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

  createPattern (image, repetition = 'repeat') {
    const dye = image.nativeCanvas || image

    return new CanvasPattern(dye, repetition)
  }

  createRadialGradient (x0, y0, r0, x1, y1, r1) {
    return new CanvasGradient('radial', x0, y0, x1, y1, r0, r1)
  }

  drawImage (image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight) {
    if ((image instanceof NativeCanvas) || (image.nativeCanvas instanceof NativeCanvas)) {
      const buffer = image.toBuffer('image/png')
      image = new Image()
      image.src = buffer
    }

    if (!(image instanceof Image)) {
      throw new TypeError('image should be instanc of Image')
    }

    if (arguments.length <= 5) {
      dx = sx
      dy = sy
      dWidth = sWidth || image.width
      dHeight = sHeight || image.height
      sx = 0
      sy = 0
      sWidth = image.width
      sHeight = image.height
    }

    return this.nativeContext.drawImage(image.src, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight)
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

  fill (...args) {
    return this.nativeContext.fill(...args)
  }

  fillRect (...args) {
    return this.nativeContext.fillRect(...args)
  }

  fillText (...args) {
    return this.nativeContext.fillText(...args)
  }

  getImageData (sx, sy, sw, sh) {
    if (sw <= 0 || sh <= 0) {
      throw new Error('IndexSizeError: sw or sh need be greater then zero.')
    }

    return this.nativeContext.getImageData(sx, sy, sw, sh)
  }

  getTransform () {
    return new DOMMatrix(
      this.nativeContext.getTransform()
    )
  }

  lineTo (...args) {
    return this.nativeContext.lineTo(...args)
  }

  measureText (...args) {
    return this.nativeContext.measureText(...args)
  }

  moveTo (...args) {
    return this.nativeContext.moveTo(...args)
  }

  putImageData (imageData, dx, dy) {
    if (!(imageData instanceof ImageData)) {
      throw new TypeError('imageData should be instance of ImageData')
    }

    return this.nativeContext.putImageData(imageData, dx, dy)
  }

  quadraticCurveTo (...args) {
    return this.nativeContext.quadraticCurveTo(...args)
  }

  rect (...args) {
    return this.nativeContext.rect(...args)
  }

  resetTransform () {
    return this.nativeContext.resetTransform()
  }

  restore (...args) {
    return this.nativeContext.restore(...args)
  }

  rotate (angle) {
    return this.nativeContext.rotate(+angle)
  }

  save (...args) {
    return this.nativeContext.save(...args)
  }

  scale (...args) {
    return this.nativeContext.scale(...args)
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

  setTransform (matrix) {
    if (matrix instanceof DOMMatrix) {
      this.nativeContext.setTransform(matrix.a, matrix.b, matrix.c, matrix.d, matrix.e, matrix.f)
    }
  }

  stroke (...args) {
    return this.nativeContext.stroke(...args)
  }

  strokeRect (...args) {
    return this.nativeContext.strokeRect(...args)
  }

  strokeText (...args) {
    return this.nativeContext.strokeText(...args)
  }

  transform (...args) {
    return this.nativeContext.transform(...args)
  }

  translate (...args) {
    return this.nativeContext.translate(...args)
  }
}

module.exports = CanvasRenderingContext2D
