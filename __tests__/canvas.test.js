const { Canvas, createCanvas } = require('../')

describe('Canvas', () => {
  it('Prototype and ctor are well-shaped, don\'t hit asserts on accessors (GH-803)', function () {
    const c = new Canvas(10, 10)
    // eslint-disable-next-line no-prototype-builtins
    expect(!c.hasOwnProperty('width'))
    expect('width' in c)
    // eslint-disable-next-line no-prototype-builtins
    expect(Canvas.prototype.hasOwnProperty('width'))
  })

  it('Canvas#{width,height}=', function () {
    const canvas = new Canvas(100, 200)
    const context = canvas.getContext('2d')

    expect(canvas.width).toBe(100)
    expect(canvas.height).toBe(200)

    context.globalAlpha = 0.5
    context.fillStyle = '#0f0'
    context.strokeStyle = '#0f0'
    context.font = '20px arial'
    context.fillRect(0, 0, 1, 1)

    canvas.width = 50
    canvas.height = 70
    expect(canvas.width).toBe(50)
    expect(canvas.height).toBe(70)

    canvas.width |= 0

    expect(context.lineWidth).toBe(1)
    expect(context.globalAlpha).toBe(1)
    expect(context.fillStyle).toBe('#000000')
    expect(context.strokeStyle).toBe('#000000')
    expect(context.font).toBe('10px sans-serif')
    expect(context.getImageData(0, 0, 1, 1).data.join(',')).toBe('0,0,0,0')
  })

  it('Canvas#getContext("invalid")', function () {
    expect(new Canvas(200, 300).getContext('invalid')).toBeNull()
  })

  it('Context2d#lineWidth=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    ctx.lineWidth = 10.0
    expect(ctx.lineWidth).toBe(10)
    ctx.lineWidth = Infinity
    expect(ctx.lineWidth).toBe(10)
    ctx.lineWidth = -Infinity
    expect(ctx.lineWidth).toBe(10)
    ctx.lineWidth = -5
    expect(ctx.lineWidth).toBe(10)
    ctx.lineWidth = 0
    expect(ctx.lineWidth).toBe(10)
  })

  // it('Context2d#antiAlias=', function () {
  //   const canvas = new Canvas(200, 200)
  //   const ctx = canvas.getContext('2d')

  //   expect(ctx.antialias).toBe('default')
  //   ctx.antialias = 'none'
  //   expect(ctx.antialias).toBe('none')
  //   ctx.antialias = 'gray'
  //   expect(ctx.antialias).toBe('gray')
  //   ctx.antialias = 'subpixel'
  //   expect(ctx.antialias).toBe('subpixel')
  //   ctx.antialias = 'invalid'
  //   expect(ctx.antialias).toBe('subpixel')
  //   ctx.antialias = 1
  //   expect(ctx.antialias).toBe('subpixel')
  // })

  it('Context2d#lineCap=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    expect(ctx.lineCap).toBe('butt')
    ctx.lineCap = 'round'
    expect(ctx.lineCap).toBe('round')
  })

  it('Context2d#lineJoin=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    expect(ctx.lineJoin).toBe('miter')
    ctx.lineJoin = 'round'
    expect(ctx.lineJoin).toBe('round')
  })

  it('Context2d#globalAlpha=', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    expect(ctx.globalAlpha).toBe(1)
    ctx.globalAlpha = 0.5
    expect(ctx.globalAlpha).toBe(0.5)
  })

  // it('Context2d#isPointInPath()', function () {
  //   const canvas = new Canvas(200, 200)
  //   const ctx = canvas.getContext('2d')

  //   ctx.rect(5, 5, 100, 100)
  //   ctx.rect(50, 100, 10, 10)
  //   expect(ctx.isPointInPath(10, 10)).toBe(true)

  //   expect(ctx.isPointInPath(10, 50)).toBe(true)
  //   expect(ctx.isPointInPath(100, 100)).toBe(true)
  //   expect(ctx.isPointInPath(105, 105)).toBe(true)
  //   expect(ctx.isPointInPath(106, 105)).toBe(true)
  //   expect(ctx.isPointInPath(150, 150)).toBe(true)
  //   expect(ctx.isPointInPath(50, 110)).toBe(true)
  //   expect(ctx.isPointInPath(60, 110)).toBe(true)
  //   expect(ctx.isPointInPath(70, 110)).toBe(true)
  //   expect(ctx.isPointInPath(50, 120)).toBe(true)
  // })

  it('Context2d#textAlign', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    expect(ctx.textAlign).toBe('start')
    ctx.textAlign = 'center'
    expect(ctx.textAlign).toBe('center')
    ctx.textAlign = 'right'
    expect(ctx.textAlign).toBe('right')
    ctx.textAlign = 'end'
    expect(ctx.textAlign).toBe('end')
    ctx.textAlign = 'fail'
    expect(ctx.textAlign).toBe('end')
  })

  describe('#toBuffer', function () {
    it('Canvas#toBuffer()', function () {
      const canvas = new Canvas(200, 200)
      expect(canvas.toBuffer().slice(1, 4).toString()).toBe('PNG')
    })

    it('Canvas#toBuffer("image/png")', function () {
      const buf = new Canvas(200, 200).toBuffer('image/png')
      expect(buf.slice(1, 4).toString()).toBe('PNG')
    })

    it('Canvas#toBuffer("image/jpeg")', function () {
      const buf = new Canvas(200, 200).toBuffer('image/jpeg')

      expect(buf[0]).toBe(0xff)
      expect(buf[1]).toBe(0xd8)
      expect(buf[buf.byteLength - 2]).toBe(0xff)
      expect(buf[buf.byteLength - 1]).toBe(0xd9)
    })
  })

  it('Context2d#createImageData(width, height)', function () {
    const canvas = new Canvas(20, 20)
    const ctx = canvas.getContext('2d')

    const imageData = ctx.createImageData(2, 6)
    expect(imageData.width).toBe(2)
    expect(imageData.height).toBe(6)
    expect(imageData.data.length).toBe(2 * 6 * 4)

    expect(imageData.data[0]).toBe(0)
    expect(imageData.data[1]).toBe(0)
    expect(imageData.data[2]).toBe(0)
    expect(imageData.data[3]).toBe(0)
  })

  describe('Context2d#measureText()', function () {
    it('Context2d#measureText().width', function () {
      const canvas = new Canvas(20, 20)
      const ctx = canvas.getContext('2d')

      expect(ctx.measureText('foo').width).toBeTruthy()
      expect(ctx.measureText('foo').width !== ctx.measureText('foobar').width).toBe(true)
      expect(ctx.measureText('foo').width !== ctx.measureText('  foo').width).toBe(true)
    })

    it('works', function () {
      const canvas = new Canvas(20, 20)
      const ctx = canvas.getContext('2d')
      ctx.font = '20px Arial'

      ctx.textBaseline = 'alphabetic'
      let metrics = ctx.measureText('Alphabet')
      // Actual value depends on font library version. Have observed values
      // between 0 and 0.769.
      expect(metrics.alphabeticBaseline >= 0 && metrics.alphabeticBaseline <= 1).toBeTruthy()
      // Positive = going up from the baseline
      expect(metrics.actualBoundingBoxAscent > 0).toBeTruthy()
      // Positive = going down from the baseline
      expect(metrics.actualBoundingBoxDescent > 0).toBeTruthy() // ~4-5

      ctx.textBaseline = 'bottom'
      metrics = ctx.measureText('Alphabet')
      expect(ctx.textBaseline).toBe('bottom')
      expect(metrics.alphabeticBaseline > 0).toBeTruthy() // ~4-5
      expect(metrics.actualBoundingBoxAscent > 0).toBeTruthy()
      // On the baseline or slightly above
      expect(metrics.actualBoundingBoxDescent <= 0).toBeTruthy()
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

      expect(ctx.getImageData(0, 0, 20, 20).data.some(a => a)).toBe(shouldDraw)
    })
  })

  it('Context2d#currentTransform', function () {
    const canvas = new Canvas(20, 20)
    const ctx = canvas.getContext('2d')

    ctx.scale(0.1, 0.3)
    const mat1 = ctx.getTransform()
    expect(mat1.a).toBe(0.1)
    expect(mat1.b).toBe(0)
    expect(mat1.c).toBe(0)
    expect(mat1.d).toBe(0.3)
    expect(mat1.e).toBe(0)
    expect(mat1.f).toBe(0)

    ctx.resetTransform()
    const mat2 = ctx.getTransform()
    expect(mat2.a).toBe(1)
    expect(mat2.d).toBe(1)

    ctx.currentTransform = mat1
    const mat3 = ctx.currentTransform
    expect(mat3.a).toBe(0.1)
    expect(mat3.d).toBe(0.3)

    expect(ctx.currentTransform).toBe(ctx.getTransform())

    ctx.setTransform(ctx.getTransform())
    expect(mat3).toBe(ctx.getTransform())

    ctx.setTransform(mat3.a, mat3.b, mat3.c, mat3.d, mat3.e, mat3.f)
    expect(mat3).toBe(ctx.getTransform())
  })

  it('Context2d#createImageData(ImageData)', function () {
    const canvas = new Canvas(20, 20)
    const ctx = canvas.getContext('2d')

    const imageData = ctx.createImageData(ctx.createImageData(2, 6))
    expect(imageData.width).toBe(2)
    expect(imageData.height).toBe(6)
    expect(imageData.data.length).toBe(2 * 6 * 4)
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

      expect(imageData.width).toBe(3)
      expect(imageData.height).toBe(6)
      expect(imageData.data.length).toBe(3 * 6 * 4)

      expect(imageData.data[0]).toBe(255)
      expect(imageData.data[1]).toBe(0)
      expect(imageData.data[2]).toBe(0)
      expect(imageData.data[3]).toBe(255)

      expect(imageData.data[4]).toBe(0)
      expect(imageData.data[5]).toBe(255)
      expect(imageData.data[6]).toBe(0)
      expect(imageData.data[7]).toBe(255)

      expect(imageData.data[8]).toBe(0)
      expect(imageData.data[9]).toBe(0)
      expect(imageData.data[10]).toBe(255)
      expect(imageData.data[11]).toBe(255)
    })

    it('works, slice, RGBA32', function () {
      const ctx = createTestCanvas()
      const imageData = ctx.getImageData(0, 0, 2, 1)
      expect(imageData.width).toBe(2)
      expect(imageData.height).toBe(1)
      expect(imageData.data.length).toBe(8)

      expect(imageData.data[0]).toBe(255)
      expect(imageData.data[1]).toBe(0)
      expect(imageData.data[2]).toBe(0)
      expect(imageData.data[3]).toBe(255)

      expect(imageData.data[4]).toBe(0)
      expect(imageData.data[5]).toBe(255)
      expect(imageData.data[6]).toBe(0)
      expect(imageData.data[7]).toBe(255)
    })

    it('works, assignment', function () {
      const ctx = createTestCanvas()
      const data = ctx.getImageData(0, 0, 5, 5).data
      data[0] = 50
      expect(data[0]).toBe(50)
      data[0] = 280
      expect(data[0]).toBe(255)
      data[0] = -4444
      expect(data[0]).toBe(0)
    })

    it('throws if indexes are invalid', function () {
      const ctx = createTestCanvas()
      expect(() => ctx.getImageData(0, 0, 0, 0)).toThrow(/IndexSizeError/)
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
    assert.equal(2, imageData.width)
    assert.equal(2, imageData.height)
    assert.equal(16, imageData.data.length)

    // (0,0) black
    assert.equal(0, imageData.data[0])
    assert.equal(0, imageData.data[1])
    assert.equal(0, imageData.data[2])
    assert.equal(255, imageData.data[3])

    // (1,0) white
    assert.equal(255, imageData.data[4])
    assert.equal(255, imageData.data[5])
    assert.equal(255, imageData.data[6])
    assert.equal(255, imageData.data[7])

    // (0,1) white
    assert.equal(255, imageData.data[8])
    assert.equal(255, imageData.data[9])
    assert.equal(255, imageData.data[10])
    assert.equal(255, imageData.data[11])

    // (1,1) black
    assert.equal(0, imageData.data[12])
    assert.equal(0, imageData.data[13])
    assert.equal(0, imageData.data[14])
    assert.equal(255, imageData.data[15])

    const canvas = createCanvas(20, 20)
    const ctx = canvas.getContext('2d')
    const ptt = ctx.createPattern(pattern)

    ctx.fillStyle = ptt
    ctx.fillRect(0, 0, 20, 20)

    const data = ctx.getImageData(0, 0, 20, 20)
    assert.equal(20, data.width)
    assert.equal(20, data.height)
    assert.equal(1600, data.data.length)

    let i = 0; let b = true
    while (i < data.data.length) {
      if (b) {
        assert.equal(0, data.data[i++])
        assert.equal(0, data.data[i++])
        assert.equal(0, data.data[i++])
        assert.equal(255, data.data[i++])
      } else {
        assert.equal(255, data.data[i++])
        assert.equal(255, data.data[i++])
        assert.equal(255, data.data[i++])
        assert.equal(255, data.data[i++])
      }
      // alternate b, except when moving to a new row
      b = i % (data.width * 4) == 0 ? b : !b
    }
  })

  it('Context2d#createPattern(Canvas).setTransform()', function () {
    const DOMMatrix = require('../').DOMMatrix

    // call func with an ImageData-offset and pixel color value appropriate for a 4-quadrant pattern within
    // the width and height that's white in the upper-left & lower-right and black in the other corners
    function eachPixel (bmp, func) {
      const { width, height } = bmp
      for (let x = 0; x < width; x++) {
        for (let y = 0; y < height; y++) {
          const i = y * 4 * width + x * 4
          const clr = (x < width / 2 && y < height / 2 || x >= width / 2 && y >= height / 2) ? 255 : 0
          func(i, clr)
        }
      }
    }

    // create a canvas with a single repeat of the pattern within its dims
    function makeCheckerboard (w, h) {
      const check = new Canvas(w, h)
      const ctx = check.getContext('2d')
      const bmp = ctx.createImageData(w, h)
      eachPixel(bmp, (i, clr) => bmp.data.set([clr, clr, clr, 255], i))
      ctx.putImageData(bmp, 0, 0)
      return check
    }

    // verify that the region looks like a single 4-quadrant checkerboard cell
    function isCheckerboard (ctx, w, h) {
      const bmp = ctx.getImageData(0, 0, w, h)
      eachPixel(bmp, (i, clr) => {
        const [r, g, b, a] = bmp.data.slice(i, i + 4)
        assert.ok(r == clr && g == clr && b == clr && a == 255)
      })
    }

    const w = 160; const h = 160
    const canvas = createCanvas(w, h)
    const ctx = canvas.getContext('2d')
    const pat = ctx.createPattern(makeCheckerboard(w, h), 'repeat')
    let mat = new DOMMatrix()

    ctx.patternQuality = 'nearest'
    ctx.fillStyle = pat;

    // draw a single repeat of the pattern at each scale and then confirm that
    // the transformation succeeded
    [1, 0.5, 0.25, 0.125, 0.0625].forEach(mag => {
      mat = new DOMMatrix().scale(mag)
      pat.setTransform(mat)
      ctx.fillRect(0, 0, w * mag, h * mag)
      isCheckerboard(ctx, w * mag, h * mag)
    })
  })

  it('Context2d#createPattern(Image)', function () {
    return loadImage(`${__dirname}/fixtures/checkers.png`).then((img) => {
      const canvas = createCanvas(20, 20)
      const ctx = canvas.getContext('2d')
      const pattern = ctx.createPattern(img)

      ctx.fillStyle = pattern
      ctx.fillRect(0, 0, 20, 20)

      const imageData = ctx.getImageData(0, 0, 20, 20)
      assert.equal(20, imageData.width)
      assert.equal(20, imageData.height)
      assert.equal(1600, imageData.data.length)

      let i = 0; let b = true
      while (i < imageData.data.length) {
        if (b) {
          assert.equal(0, imageData.data[i++])
          assert.equal(0, imageData.data[i++])
          assert.equal(0, imageData.data[i++])
          assert.equal(255, imageData.data[i++])
        } else {
          assert.equal(255, imageData.data[i++])
          assert.equal(255, imageData.data[i++])
          assert.equal(255, imageData.data[i++])
          assert.equal(255, imageData.data[i++])
        }
        // alternate b, except when moving to a new row
        b = i % (imageData.width * 4) == 0 ? b : !b
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
    assert.equal(20, imageData.width)
    assert.equal(1, imageData.height)
    assert.equal(80, imageData.data.length)

    // (0,0) white
    assert.equal(255, imageData.data[0])
    assert.equal(255, imageData.data[1])
    assert.equal(255, imageData.data[2])
    assert.equal(255, imageData.data[3])

    // (20,0) black
    const i = imageData.data.length - 4
    assert.equal(0, imageData.data[i + 0])
    assert.equal(0, imageData.data[i + 1])
    assert.equal(0, imageData.data[i + 2])
    assert.equal(255, imageData.data[i + 3])
  })

  describe('Context2d#putImageData()', function () {
    it('throws for invalid arguments', function () {
      const canvas = createCanvas(2, 1)
      const ctx = canvas.getContext('2d')
      assert.throws(function () { ctx.putImageData({}, 0, 0) }, TypeError)
      assert.throws(function () { ctx.putImageData(undefined, 0, 0) }, TypeError)
    })

    it('works for negative source values', function () {
      const canvas = createCanvas(2, 2)
      const ctx = canvas.getContext('2d')
      const srcImageData = createImageData(new Uint8ClampedArray([
        1, 2, 3, 255, 5, 6, 7, 255,
        0, 1, 2, 255, 4, 5, 6, 255
      ]), 2)

      ctx.putImageData(srcImageData, -1, -1)

      const resImageData = ctx.getImageData(0, 0, 2, 2)
      assert.deepEqual(resImageData.data, new Uint8ClampedArray([
        4, 5, 6, 255, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
      ]))
    })

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

    it('works, RGB24/alpha:false', function () {
      const canvas = createCanvas(2, 1)
      const ctx = canvas.getContext('2d', { pixelFormat: 'RGB24' })
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

    it('works, A8', function () {
      const canvas = createCanvas(2, 1)
      const ctx = canvas.getContext('2d', { pixelFormat: 'A8' })

      const imgData = ctx.getImageData(0, 0, 2, 1)
      imgData.data[0] = 4
      imgData.data[1] = 21
      ctx.putImageData(imgData, 0, 0)

      const pixel = ctx.getImageData(0, 0, 2, 1)

      assert.equal(pixel.data[0], 4)
      assert.equal(pixel.data[1], 21)
    })

    it('works, RGB16_565', function () {
      const canvas = createCanvas(2, 1)
      const ctx = canvas.getContext('2d', { pixelFormat: 'RGB16_565' })

      const imgData = ctx.getImageData(0, 0, 2, 1)
      imgData.data[0] = 65535 // 2**16 - 1
      imgData.data[1] = 65500
      ctx.putImageData(imgData, 0, 0)

      const pixel = ctx.getImageData(0, 0, 2, 1)

      assert.equal(pixel.data[0], 65535)
      assert.equal(pixel.data[1], 65500)
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

  it('Context2d#IsPointInPath()', function () {
    const canvas = createCanvas(4, 4)
    const ctx = canvas.getContext('2d')

    ctx.rect(0, 0, 4, 2)
    ctx.rect(2, 0, 2, 4)
    ctx.stroke()

    assert.ok(ctx.isPointInPath(1, 1, 'evenodd'))
    assert.ok(!ctx.isPointInPath(3, 1, 'evenodd'))
    assert.ok(ctx.isPointInPath(3, 1))
    assert.ok(!ctx.isPointInPath(1, 3, 'evenodd'))
    assert.ok(ctx.isPointInPath(3, 3, 'evenodd'))
  })

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

      const mat = ctx.currentTransform
      const sin = Math.sin(expected)
      const cos = Math.cos(expected)

      assert.ok(Math.abs(mat.m11 - cos) < Number.EPSILON)
      assert.ok(Math.abs(mat.m12 - sin) < Number.EPSILON)
      assert.ok(Math.abs(mat.m21 + sin) < Number.EPSILON)
      assert.ok(Math.abs(mat.m22 - cos) < Number.EPSILON)
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

    for (var i = 0; i < 500 * 500 * 4; i += 4) {
      if (data[i] === 0 && data[i + 1] === 0 && data[i + 2] === 0) { count++ }
    }

    assert.strictEqual(count, 10 * 10 * 2)

    // Drawing zero-width image
    ctx.drawImage(canvas, 0, 0, 0, 0, 0, 0, 0, 0)
    ctx.drawImage(canvas, 0, 0, 0, 0, 1, 1, 1, 1)
    ctx.drawImage(canvas, 1, 1, 1, 1, 0, 0, 0, 0)
    ctx.fillStyle = 'white'
    ctx.fillRect(0, 0, 500, 500)

    imgd = ctx.getImageData(0, 0, 500, 500)
    data = imgd.data
    count = 0

    for (i = 0; i < 500 * 500 * 4; i += 4) {
      if (data[i] === 255 && data[i + 1] === 255 && data[i + 2] === 255) { count++ }
    }

    assert.strictEqual(count, 500 * 500)
  })
})
