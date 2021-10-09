const assert = require('assert')
const fs = require('fs')
const path = require('path')
const util = require('util')
const { Canvas, createCanvas, Image } = require('../lib')

const readFile = util.promisify(fs.readFile)
const loadImage = async (src) => {
  const img = new Image()
  img.src = await readFile(src)

  return img
}

describe('Canvas', () => {
  it('Prototype and ctor are well-shaped, don\'t hit asserts on accessors (GH-803)', function () {
    const c = new Canvas(10, 10)
    // eslint-disable-next-line no-prototype-builtins
    assert(!c.hasOwnProperty('width'))
    assert(c instanceof Canvas)
    assert('width' in c)
    // eslint-disable-next-line no-prototype-builtins
    assert(Canvas.prototype.hasOwnProperty('width'))
  })

  it('Canvas#{width,height}=', function () {
    const canvas = new Canvas(100, 200)
    const context = canvas.getContext('2d')

    assert.equal(canvas.width, 100)
    assert.equal(canvas.height, 200)

    context.globalAlpha = 0.5
    context.fillStyle = '#0f0'
    context.strokeStyle = '#0f0'
    context.font = '20px arial'
    context.fillRect(0, 0, 1, 1)

    canvas.width = 50
    canvas.height = 70
    assert.equal(canvas.width, 50)
    assert.equal(canvas.height, 70)

    canvas.width |= 0

    assert.equal(context.lineWidth, 1)
    assert.equal(context.globalAlpha, 1)
    assert.equal(context.fillStyle, '#000000')
    assert.equal(context.strokeStyle, '#000000')
    assert.equal(context.font, '10px sans-serif')
    assert.equal(context.getImageData(0, 0, 1, 1).data.join(','), '0,0,0,0')
  })

  it('Canvas#getContext("invalid")', function () {
    assert(new Canvas(200, 300).getContext('invalid') == null)
  })

  it('Context2d#lineWidth=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    ctx.lineWidth = 10.0
    assert.equal(ctx.lineWidth, 10)
    ctx.lineWidth = Infinity
    assert.equal(ctx.lineWidth, 10)
    ctx.lineWidth = -Infinity
    assert.equal(ctx.lineWidth, 10)
    ctx.lineWidth = -5
    assert.equal(ctx.lineWidth, 10)
    ctx.lineWidth = 0
    assert.equal(ctx.lineWidth, 10)
  })

  // it('Context2d#antiAlias=', function () {
  //   const canvas = new Canvas(200, 200)
  //   const ctx = canvas.getContext('2d')

  //   assert.equal(ctx.antialias, 'default')
  //   ctx.antialias = 'none'
  //   assert.equal(ctx.antialias, 'none')
  //   ctx.antialias = 'gray'
  //   assert.equal(ctx.antialias, 'gray')
  //   ctx.antialias = 'subpixel'
  //   assert.equal(ctx.antialias, 'subpixel')
  //   ctx.antialias = 'invalid'
  //   assert.equal(ctx.antialias, 'subpixel')
  //   ctx.antialias = 1
  //   assert.equal(ctx.antialias, 'subpixel')
  // })

  it('Context2d#lineCap=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    assert.equal(ctx.lineCap, 'butt')
    ctx.lineCap = 'round'
    assert.equal(ctx.lineCap, 'round')
  })

  it('Context2d#lineJoin=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    assert.equal(ctx.lineJoin, 'miter')
    ctx.lineJoin = 'round'
    assert.equal(ctx.lineJoin, 'round')
  })

  it('Context2d#globalAlpha=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    assert.equal(ctx.globalAlpha, 1)
    ctx.globalAlpha = 0.5
    assert.equal(ctx.globalAlpha, 0.5)
  })

  // it('Context2d#isPointInPath()', function () {
  //   const canvas = new Canvas(200, 200)
  //   const ctx = canvas.getContext('2d')

  //   ctx.rect(5, 5, 100, 100)
  //   ctx.rect(50, 100, 10, 10)
  //   assert.equal(ctx.isPointInPath(10, 10), true)

  //   assert.equal(ctx.isPointInPath(10, 50), true)
  //   assert.equal(ctx.isPointInPath(100, 100), true)
  //   assert.equal(ctx.isPointInPath(105, 105), true)
  //   assert.equal(ctx.isPointInPath(106, 105), true)
  //   assert.equal(ctx.isPointInPath(150, 150), true)
  //   assert.equal(ctx.isPointInPath(50, 110), true)
  //   assert.equal(ctx.isPointInPath(60, 110), true)
  //   assert.equal(ctx.isPointInPath(70, 110), true)
  //   assert.equal(ctx.isPointInPath(50, 120), true)
  // })

  it('Context2d#textAlign', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    assert.equal(ctx.textAlign, 'start')
    ctx.textAlign = 'center'
    assert.equal(ctx.textAlign, 'center')
    ctx.textAlign = 'right'
    assert.equal(ctx.textAlign, 'right')
    ctx.textAlign = 'end'
    assert.equal(ctx.textAlign, 'end')
    ctx.textAlign = 'fail'
    assert.equal(ctx.textAlign, 'end')
  })

  describe('#toBuffer', function () {
    it('Canvas#toBuffer()', function () {
      const canvas = new Canvas(200, 200)
      assert.equal(canvas.toBuffer().slice(1, 4).toString(), 'PNG')
    })

    it('Canvas#toBuffer("image/png")', function () {
      const buf = new Canvas(200, 200).toBuffer('image/png')
      assert.equal(buf.slice(1, 4).toString(), 'PNG')
    })

    it('Canvas#toBuffer("image/jpeg")', function () {
      const buf = new Canvas(200, 200).toBuffer('image/jpeg')

      assert.equal(buf[0], 0xff)
      assert.equal(buf[1], 0xd8)
      assert.equal(buf[buf.byteLength - 2], 0xff)
      assert.equal(buf[buf.byteLength - 1], 0xd9)
    })
  })

  it('Context2d#createImageData(width, height)', function () {
    const canvas = new Canvas(20, 20)
    const ctx = canvas.getContext('2d')

    const imageData = ctx.createImageData(2, 6)
    assert.equal(imageData.width, 2)
    assert.equal(imageData.height, 6)
    assert.equal(imageData.data.length, 2 * 6 * 4)

    assert.equal(imageData.data[0], 0)
    assert.equal(imageData.data[1], 0)
    assert.equal(imageData.data[2], 0)
    assert.equal(imageData.data[3], 0)
  })

  describe('Context2d#measureText()', function () {
    it('Context2d#measureText().width', function () {
      const canvas = new Canvas(20, 20)
      const ctx = canvas.getContext('2d')

      assert(ctx.measureText('foo').width)
      assert.equal(ctx.measureText('foo').width !== ctx.measureText('foobar').width, true)
      assert.equal(ctx.measureText('foo').width !== ctx.measureText('  foo').width, true)
    })

    it('works', function () {
      const canvas = new Canvas(20, 20)
      const ctx = canvas.getContext('2d')
      ctx.font = '20px Arial'

      ctx.textBaseline = 'alphabetic'
      let metrics = ctx.measureText('Alphabet')
      // Actual value depends on font library version. Have observed values
      // between 0 and 0.769.
      assert(metrics.alphabeticBaseline >= 0 && metrics.alphabeticBaseline <= 1)
      // Positive = going up from the baseline
      assert(metrics.actualBoundingBoxAscent > 0)
      // Positive = going down from the baseline
      assert(metrics.actualBoundingBoxDescent > 0) // ~4-5

      ctx.textBaseline = 'bottom'
      metrics = ctx.measureText('Alphabet')
      assert.equal(ctx.textBaseline, 'bottom')
      assert(metrics.alphabeticBaseline > 0) // ~4-5
      assert(metrics.actualBoundingBoxAscent > 0)
      // On the baseline or slightly above
      assert(metrics.actualBoundingBoxDescent <= 1)
    })
  })

  it('Context2d#fillText()', function () {
    [
      [['A', 10, 10], true],
      [['A', 10, 10, undefined], true],
      [['A', 10, 10, NaN], false]
    ].forEach(([args, shouldDraw]) => {
      const canvas = new Canvas(20, 20)
      const ctx = canvas.getContext('2d')

      ctx.textBaseline = 'middle'
      ctx.textAlign = 'center'
      ctx.fillText(...args)

      assert.equal(ctx.getImageData(0, 0, 20, 20).data.some(a => a), shouldDraw)
    })
  })

  it('Context2d#currentTransform', function () {
    const canvas = new Canvas(20, 20)
    const ctx = canvas.getContext('2d')

    ctx.scale(0.1, 0.3)
    const mat1 = ctx.getTransform()
    assert.equal(Math.abs(mat1.a - 0.1) < 1e-6, true)
    assert.equal(Math.abs(mat1.b - 0) < 1e-6, true)
    assert.equal(Math.abs(mat1.c - 0) < 1e-6, true)
    assert.equal(Math.abs(mat1.d - 0.3) < 1e-6, true)
    assert.equal(Math.abs(mat1.e - 0) < 1e-6, true)
    assert.equal(Math.abs(mat1.f - 0) < 1e-6, true)

    ctx.resetTransform()
    const mat2 = ctx.getTransform()
    assert.equal(Math.abs(mat2.a - 1) < 1e-6, true)
    assert.equal(Math.abs(mat2.d - 1) < 1e-6, true)

    const mat3 = mat1
    assert.equal(Math.abs(mat3.a - 0.1) < 1e-6, true)
    assert.equal(Math.abs(mat3.d - 0.3) < 1e-6, true)

    ctx.setTransform(mat3)
    assert.deepEqual(mat3, ctx.getTransform())

    ctx.setTransform(mat3.a, mat3.b, mat3.c, mat3.d, mat3.e, mat3.f)
    assert.deepEqual(mat3, ctx.getTransform())
  })

  it('Context2d#createImageData(ImageData)', function () {
    const canvas = new Canvas(20, 20)
    const ctx = canvas.getContext('2d')

    const imageData = ctx.createImageData(ctx.createImageData(2, 6))
    assert.equal(imageData.width, 2)
    assert.equal(imageData.height, 6)
    assert.equal(imageData.data.length, 2 * 6 * 4)
  })

  describe('Context2d#getImageData()', function () {
    function createTestCanvas (useAlpha, attributes) {
      const canvas = new Canvas(3, 6)
      const ctx = canvas.getContext('2d', attributes)

      ctx.fillStyle = useAlpha ? 'rgba(255,0,0,0.25)' : '#f00'
      ctx.fillRect(0, 0, 1, 6)

      ctx.fillStyle = useAlpha ? 'rgba(0,255,0,0.5)' : '#0f0'
      ctx.fillRect(1, 0, 1, 6)

      ctx.fillStyle = useAlpha ? 'rgba(0,0,255,0.75)' : '#00f'
      ctx.fillRect(2, 0, 1, 6)

      return ctx
    }

    it('works, full width, RGBA32', function () {
      const ctx = createTestCanvas()
      const imageData = ctx.getImageData(0, 0, 3, 6)

      assert.equal(imageData.width, 3)
      assert.equal(imageData.height, 6)
      assert.equal(imageData.data.length, 3 * 6 * 4)

      assert.equal(imageData.data[0], 255)
      assert.equal(imageData.data[1], 0)
      assert.equal(imageData.data[2], 0)
      assert.equal(imageData.data[3], 255)

      assert.equal(imageData.data[4], 0)
      assert.equal(imageData.data[5], 255)
      assert.equal(imageData.data[6], 0)
      assert.equal(imageData.data[7], 255)

      assert.equal(imageData.data[8], 0)
      assert.equal(imageData.data[9], 0)
      assert.equal(imageData.data[10], 255)
      assert.equal(imageData.data[11], 255)
    })

    it('works, slice, RGBA32', function () {
      const ctx = createTestCanvas()
      const imageData = ctx.getImageData(0, 0, 2, 1)
      assert.equal(imageData.width, 2)
      assert.equal(imageData.height, 1)
      assert.equal(imageData.data.length, 8)

      assert.equal(imageData.data[0], 255)
      assert.equal(imageData.data[1], 0)
      assert.equal(imageData.data[2], 0)
      assert.equal(imageData.data[3], 255)

      assert.equal(imageData.data[4], 0)
      assert.equal(imageData.data[5], 255)
      assert.equal(imageData.data[6], 0)
      assert.equal(imageData.data[7], 255)
    })

    it('works, assignment', function () {
      const ctx = createTestCanvas()
      const data = ctx.getImageData(0, 0, 5, 5).data
      data[0] = 50
      assert.equal(data[0], 50)
      data[0] = 280
      assert.equal(data[0], 255)
      data[0] = -4444
      assert.equal(data[0], 0)
    })

    it('throws if indexes are invalid', function () {
      const ctx = createTestCanvas()
      assert.throws(() => ctx.getImageData(0, 0, 0, 0), /IndexSizeError/)
    })
  })

  it('Context2d#createPattern(Canvas)', function () {
    const pattern = new Canvas(2, 2)
    const checkers = pattern.getContext('2d')

    // white
    checkers.fillStyle = '#fff'
    checkers.fillRect(0, 0, 2, 2)

    // black
    checkers.fillStyle = '#000'
    checkers.fillRect(0, 0, 1, 1)
    checkers.fillRect(1, 1, 1, 1)

    const imageData = checkers.getImageData(0, 0, 2, 2)
    assert.equal(imageData.width, 2)
    assert.equal(imageData.height, 2)
    assert.equal(imageData.data.length, 16)

    const data = imageData.data

    // (0,0) black
    assert.equal(data[0], 0)
    assert.equal(data[1], 0)
    assert.equal(data[2], 0)
    assert.equal(data[3], 255)

    // (1,0) white
    assert.equal(data[4], 255)
    assert.equal(data[5], 255)
    assert.equal(data[6], 255)
    assert.equal(data[7], 255)

    // (0,1) white
    assert.equal(data[8], 255)
    assert.equal(data[9], 255)
    assert.equal(data[10], 255)
    assert.equal(data[11], 255)

    // (1,1) black
    assert.equal(data[12], 0)
    assert.equal(data[13], 0)
    assert.equal(data[14], 0)
    assert.equal(data[15], 255)

    const canvas = createCanvas(20, 20)
    const ctx = canvas.getContext('2d')
    const ptt = ctx.createPattern(pattern)

    ctx.fillStyle = ptt
    ctx.fillRect(0, 0, 20, 20)

    const imageData1 = ctx.getImageData(0, 0, 20, 20)
    assert.equal(imageData1.width, 20)
    assert.equal(imageData1.height, 20)
    assert.equal(imageData1.data.length, 1600)

    const data1 = imageData1.data
    let i = 0; let b = true
    while (i < data1.length) {
      if (b) {
        assert.equal(data1[i++], 0)
        assert.equal(data1[i++], 0)
        assert.equal(data1[i++], 0)
        assert.equal(data1[i++], 255)
      } else {
        assert.equal(data1[i++], 255)
        assert.equal(data1[i++], 255)
        assert.equal(data1[i++], 255)
        assert.equal(data1[i++], 255)
      }
      // alternate b, except when moving to a new row
      b = i % (imageData1.width * 4) === 0 ? b : !b
    }
  })

  // FIXME: segmentfault issue
  // it('Context2d#createPattern(Canvas).setTransform()', function () {
  //   const DOMMatrix = require('../').DOMMatrix

  //   // call func with an ImageData-offset and pixel color value appropriate for a 4-quadrant pattern within
  //   // the width and height that's white in the upper-left & lower-right and black in the other corners
  //   function eachPixel (bmp, func) {
  //     const { width, height } = bmp
  //     for (let x = 0; x < width; x++) {
  //       for (let y = 0; y < height; y++) {
  //         const i = y * 4 * width + x * 4
  //         const clr = ((x < width / 2 && y < height / 2) || (x >= width / 2 && y >= height / 2)) ? 255 : 0
  //         func(i, clr)
  //       }
  //     }
  //   }

  //   // create a canvas with a single repeat of the pattern within its dims
  //   function makeCheckerboard (w, h) {
  //     const check = new Canvas(w, h)
  //     const ctx = check.getContext('2d')
  //     const bmp = ctx.createImageData(w, h)
  //     eachPixel(bmp, (i, clr) => bmp.data.set([clr, clr, clr, 255], i))
  //     ctx.putImageData(bmp, 0, 0)
  //     return check
  //   }

  //   // verify that the region looks like a single 4-quadrant checkerboard cell
  //   function isCheckerboard (ctx, w, h) {
  //     const bmp = ctx.getImageData(0, 0, w, h)
  //     eachPixel(bmp, (i, clr) => {
  //       const [r, g, b, a] = bmp.data.slice(i, i + 4)
  //       assert.equal(r === clr && g === clr && b === clr && a === 255, true)
  //     })
  //   }

  //   const w = 160
  //   const h = 160
  //   const canvas = createCanvas(w, h)
  //   const ctx = canvas.getContext('2d')
  //   const pat = ctx.createPattern(makeCheckerboard(w, h), 'repeat')
  //   let mat = new DOMMatrix()

  //   ctx.patternQuality = 'nearest'
  //   ctx.fillStyle = pat;

  //   // draw a single repeat of the pattern at each scale and then confirm that
  //   // the transformation succeeded
  //   [1, 0.5, 0.25, 0.125, 0.0625].forEach(mag => {
  //     mat = new DOMMatrix().scale(mag)
  //     pat.setTransform(mat)
  //     ctx.fillRect(0, 0, w * mag, h * mag)
  //     isCheckerboard(ctx, w * mag, h * mag)
  //   })
  // })

  it('Context2d#createPattern(Image)', function () {
    return loadImage(path.join(__dirname, './fixtures/checkers.png')).then((img) => {
      const canvas = createCanvas(20, 20)
      const ctx = canvas.getContext('2d')
      const pattern = ctx.createPattern(img)

      ctx.fillStyle = pattern
      ctx.fillRect(0, 0, 20, 20)

      const imageData = ctx.getImageData(0, 0, 20, 20)
      assert.equal(imageData.width, 20)
      assert.equal(imageData.height, 20)
      assert.equal(imageData.data.length, 1600)

      const data = imageData.data

      let i = 0; let b = true
      while (i < data.length) {
        if (b) {
          assert.equal(data[i++], 0)
          assert.equal(data[i++], 0)
          assert.equal(data[i++], 0)
          assert.equal(data[i++], 255)
        } else {
          assert.equal(data[i++], 255)
          assert.equal(data[i++], 255)
          assert.equal(data[i++], 255)
          assert.equal(data[i++], 255)
        }
        // alternate b, except when moving to a new row
        b = i % (imageData.width * 4) === 0 ? b : !b
      }
    })
  })

  it('Context2d#createLinearGradient()', function () {
    const canvas = createCanvas(20, 1)
    const ctx = canvas.getContext('2d')
    const gradient = ctx.createLinearGradient(1, 1, 19, 1)

    gradient.addColorStop(0, '#fff')
    gradient.addColorStop(1, '#000')

    ctx.fillStyle = gradient
    ctx.fillRect(0, 0, 20, 1)

    const imageData = ctx.getImageData(0, 0, 20, 1)
    assert.equal(imageData.width, 20)
    assert.equal(imageData.height, 1)
    assert.equal(imageData.data.length, 80)

    // (0,0) white
    assert.equal(imageData.data[0], 255)
    assert.equal(imageData.data[1], 255)
    assert.equal(imageData.data[2], 255)
    assert.equal(imageData.data[3], 255)

    // (20,0) black
    const i = imageData.data.length - 4
    assert.equal(imageData.data[i + 0], 0)
    assert.equal(imageData.data[i + 1], 0)
    assert.equal(imageData.data[i + 2], 0)
    assert.equal(imageData.data[i + 3], 255)
  })

  describe('Context2d#putImageData()', function () {
    it('throws for invalid arguments', function () {
      const canvas = createCanvas(2, 1)
      const ctx = canvas.getContext('2d')

      assert.throws(() => ctx.putImageData({}, 0, 0), TypeError)
      assert.throws(() => ctx.putImageData(undefined, 0, 0), TypeError)
    })

    // it('works for negative source values', function () {
    //   const canvas = createCanvas(2, 2)
    //   const ctx = canvas.getContext('2d')
    //   const srcImageData = ctx.createImageData(new Uint8ClampedArray([
    //     1, 2, 3, 255, 5, 6, 7, 255,
    //     0, 1, 2, 255, 4, 5, 6, 255
    //   ]), 2)

    //   ctx.putImageData(srcImageData, -1, -1)

    //   const resImageData = ctx.getImageData(0, 0, 2, 2)
    //   assert.equal(resImageData.data, new Uint8ClampedArray([
    //     4, 5, 6, 255, 0, 0, 0, 0,
    //     0, 0, 0, 0, 0, 0, 0, 0
    //   ]))
    // })

    it('works, RGBA32', function () {
      const canvas = createCanvas(2, 1)
      const ctx = canvas.getContext('2d')
      ctx.fillStyle = '#f00'
      ctx.fillRect(0, 0, 1, 1)

      // Copy left pixel to the right pixel
      ctx.putImageData(ctx.getImageData(0, 0, 1, 1), 1, 0)

      const pixel = ctx.getImageData(1, 0, 1, 1)

      assert.equal(pixel.data[0], 255)
      assert.equal(pixel.data[1], 0)
      assert.equal(pixel.data[2], 0)
      assert.equal(pixel.data[3], 255)
    })
  })

  it('Context2d#fill()', function () {
    const canvas = createCanvas(2, 2)
    const ctx = canvas.getContext('2d')

    // fill whole canvas with white
    ctx.fillStyle = '#fff'
    ctx.fillRect(0, 0, 2, 2)

    let imageData, n

    // black
    ctx.fillStyle = '#000'
    ctx.rect(0, 0, 2, 1)
    ctx.rect(1, 0, 1, 2)

    ctx.fill('evenodd')
    // b | w
    // -----
    // w | b
    imageData = ctx.getImageData(0, 0, 2, 2)
    // (0, 0) black
    n = 0
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (0, 1) white
    n = 1
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 0) white
    n = 2
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 1) black
    n = 3
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)

    // should not retain previous value 'evenodd'
    ctx.fill()
    // b | b
    // -----
    // w | b
    imageData = ctx.getImageData(0, 0, 2, 2)
    // (0, 0) black
    n = 0
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (0, 1) black
    n = 1
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 0) white
    n = 2
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 1) black
    n = 3
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
  })

  it('Context2d#clip()', function () {
    const canvas = createCanvas(2, 2)
    const ctx = canvas.getContext('2d')

    // fill whole canvas with white
    ctx.fillStyle = '#fff'
    ctx.fillRect(0, 0, 2, 2)

    let imageData, n

    // black
    ctx.fillStyle = '#000'
    ctx.rect(0, 0, 2, 1)
    ctx.rect(1, 0, 1, 2)

    ctx.clip('evenodd')
    ctx.fillRect(0, 0, 2, 2)
    // b | w
    // -----
    // w | b
    imageData = ctx.getImageData(0, 0, 2, 2)
    // (0, 0) black
    n = 0
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (0, 1) white
    n = 1
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 0) white
    n = 2
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 1) black
    n = 3
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)

    ctx.clip()
    ctx.fillRect(0, 0, 2, 2)
    // b | b
    // -----
    // w | b
    imageData = ctx.getImageData(0, 0, 2, 2)
    // (0, 0) black
    n = 0
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (0, 1) white
    n = 1
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 0) white
    n = 2
    assert.equal(imageData.data[n * 4 + 0], 255)
    assert.equal(imageData.data[n * 4 + 1], 255)
    assert.equal(imageData.data[n * 4 + 2], 255)
    assert.equal(imageData.data[n * 4 + 3], 255)
    // (1, 1) black
    n = 3
    assert.equal(imageData.data[n * 4 + 0], 0)
    assert.equal(imageData.data[n * 4 + 1], 0)
    assert.equal(imageData.data[n * 4 + 2], 0)
    assert.equal(imageData.data[n * 4 + 3], 255)
  })

  // it('Context2d#IsPointInPath()', function () {
  //   const canvas = createCanvas(4, 4)
  //   const ctx = canvas.getContext('2d')

  //   ctx.rect(0, 0, 4, 2)
  //   ctx.rect(2, 0, 2, 4)
  //   ctx.stroke()

  //   assert.ok(ctx.isPointInPath(1, 1, 'evenodd'))
  //   assert.ok(!ctx.isPointInPath(3, 1, 'evenodd'))
  //   assert.ok(ctx.isPointInPath(3, 1))
  //   assert.ok(!ctx.isPointInPath(1, 3, 'evenodd'))
  //   assert.ok(ctx.isPointInPath(3, 3, 'evenodd'))
  // })

  it('Context2d#rotate(angle)', function () {
    const canvas = createCanvas(4, 4)
    const ctx = canvas.getContext('2d')

    // Number
    ctx.resetTransform()
    testAngle(1.23, 1.23)

    // String
    ctx.resetTransform()
    testAngle('-4.56e-1', -0.456)

    // Boolean
    ctx.resetTransform()
    testAngle(true, 1)

    // Array
    ctx.resetTransform()
    testAngle([7.8], 7.8)

    // Object
    const obj = Object.create(null)
    if (+process.version.match(/\d+/) >= 6) { obj[Symbol.toPrimitive] = function () { return 0.89 } } else { obj.valueOf = function () { return 0.89 } }
    ctx.resetTransform()
    testAngle(obj, 0.89)

    // NaN
    ctx.resetTransform()
    ctx.rotate(0.91)
    testAngle(NaN, 0.91)

    // Infinite value
    ctx.resetTransform()
    ctx.rotate(0.94)
    testAngle(-Infinity, 0.94)

    function testAngle (angle, expected) {
      ctx.rotate(angle)

      const mat = ctx.getTransform()
      const sin = Math.sin(expected)
      const cos = Math.cos(expected)

      assert.equal(Math.abs(mat.m11 - cos) < 1e-6, true)
      assert.equal(Math.abs(mat.m12 - sin) < 1e-6, true)
      assert.equal(Math.abs(mat.m21 + sin) < 1e-6, true)
      assert.equal(Math.abs(mat.m22 - cos) < 1e-6, true)
    }
  })

  it('Context2d#drawImage()', function () {
    const canvas = createCanvas(500, 500)
    const ctx = canvas.getContext('2d')

    // Drawing canvas to itself
    ctx.fillStyle = 'white'
    ctx.fillRect(0, 0, 500, 500)
    ctx.fillStyle = 'black'
    ctx.fillRect(5, 5, 10, 10)
    ctx.drawImage(canvas, 20, 20)

    let imgd = ctx.getImageData(0, 0, 500, 500)
    let data = imgd.data
    let count = 0

    for (let i = 0; i < 500 * 500 * 4; i += 4) {
      if (data[i] === 0 && data[i + 1] === 0 && data[i + 2] === 0) { count++ }
    }

    assert.equal(count, 10 * 10 * 2)

    // Drawing zero-width image
    ctx.drawImage(canvas, 0, 0, 0, 0, 0, 0, 0, 0)
    ctx.drawImage(canvas, 0, 0, 0, 0, 1, 1, 1, 1)
    ctx.drawImage(canvas, 1, 1, 1, 1, 0, 0, 0, 0)
    ctx.fillStyle = 'white'
    ctx.fillRect(0, 0, 500, 500)

    imgd = ctx.getImageData(0, 0, 500, 500)
    data = imgd.data
    count = 0

    for (let i = 0; i < 500 * 500 * 4; i += 4) {
      if (data[i] === 255 && data[i + 1] === 255 && data[i + 2] === 255) { count++ }
    }

    assert.equal(count, 500 * 500)
  })
})
