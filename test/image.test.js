
const assert = require('assert')
const fs = require('fs')
const path = require('path')
const util = require('util')
const { createCanvas, Image } = require('../lib')

// const PNG_CHECKERS = path.join(__dirname, './fixtures/checkers.png')
const PNG_CLOCK = path.join(__dirname, './fixtures/clock.png')
const JPG_CHROME = path.join(__dirname, './fixtures/chrome.jpg')
const JPG_FACE = path.join(__dirname, './fixtures/face.jpeg')
// const SVG_TREE = path.join(__dirname, './fixtures/tree.svg')
const BMP_DIR = path.join(__dirname, './fixtures/bmp')

const readFile = util.promisify(fs.readFile)
const loadImage = async (src) => {
  const img = new Image()
  img.src = await readFile(src)

  return img
}

describe('Image', function () {
  it('Prototype and ctor are well-shaped, don\'t hit asserts on accessors (GH-803)', function () {
    const img = new Image()

    // eslint-disable-next-line no-prototype-builtins
    assert.equal(!img.hasOwnProperty('width'), true)
    assert.equal('width' in img, true)
    // eslint-disable-next-line no-prototype-builtins
    assert(Image.prototype.hasOwnProperty('width'))
  })

  it('loads JPEG image', function () {
    return loadImage(JPG_FACE).then((img) => {
      assert.equal(img.width, 485)
      assert.equal(img.height, 401)
    })
  })

  it('loads PNG image', function () {
    return loadImage(PNG_CLOCK).then((img) => {
      assert.equal(img.width, 320)
      assert.equal(img.height, 320)
    })
  })

  // it('detects invalid PNG', function () {
  //   if (process.platform === 'win32') this.skip() // TODO
  //   const img = new Image()
  //   img.onerror = () => {
  //     // assert.strictEqual(img.complete, true)
  //     done()
  //   }
  //   img.src = Buffer.from('89504E470D', 'hex')
  // })

  // it('propagates exceptions thrown by onload', function () {
  //   class MyError extends Error {}
  //   const img = new Image()
  //   img.onload = () => {
  //     throw new MyError()
  //   }
  //   assert.throws(() => {
  //     img.src = jpg_face
  //   }, MyError)
  // })

  // it('propagates exceptions thrown by onerror', function () {
  //   class MyError extends Error {}
  //   const img = new Image()
  //   img.onerror = () => {
  //     throw new MyError()
  //   }
  //   assert.throws(() => {
  //     img.src = Buffer.from('', 'hex')
  //   }, MyError)
  // })

  // it('loads SVG data URL base64', function () {
  //   if (!HAVE_SVG) this.skip()
  //   const base64Enc = fs.readFileSync(svg_tree, 'base64')
  //   const dataURL = `data:image/svg+xml;base64,${base64Enc}`
  //   return loadImage(dataURL).then((img) => {
  //     assert.strictEqual(img.onerror, null)
  //     assert.strictEqual(img.onload, null)
  //     assert.strictEqual(img.width, 200)
  //     assert.strictEqual(img.height, 200)
  //     assert.strictEqual(img.complete, true)
  //   })
  // })

  // it('loads SVG data URL utf8', function () {
  //   if (!HAVE_SVG) this.skip()
  //   const utf8Encoded = fs.readFileSync(svg_tree, 'utf8')
  //   const dataURL = `data:image/svg+xml;utf8,${utf8Encoded}`
  //   return loadImage(dataURL).then((img) => {
  //     assert.strictEqual(img.onerror, null)
  //     assert.strictEqual(img.onload, null)
  //     assert.strictEqual(img.width, 200)
  //     assert.strictEqual(img.height, 200)
  //     assert.strictEqual(img.complete, true)
  //   })
  // })

  // it('calls Image#onload multiple times', function () {
  //   return loadImage(png_clock).then((img) => {
  //     let onloadCalled = 0

  //     img.onload = () => { onloadCalled += 1 }

  //     img.src = png_checkers
  //     assert.strictEqual(img.src, png_checkers)
  //     assert.strictEqual(img.complete, true)
  //     assert.strictEqual(img.width, 2)
  //     assert.strictEqual(img.height, 2)

  //     img.src = png_clock
  //     assert.strictEqual(img.src, png_clock)
  //     assert.strictEqual(true, img.complete)
  //     assert.strictEqual(320, img.width)
  //     assert.strictEqual(320, img.height)

  //     assert.strictEqual(onloadCalled, 2)

  //     onloadCalled = 0
  //     img.onload = () => { onloadCalled += 1 }

  //     img.src = png_clock
  //     assert.strictEqual(onloadCalled, 1)
  //   })
  // })

  // it('calls Image#onerror multiple times', function () {
  //   return loadImage(png_clock).then((img) => {
  //     let onloadCalled = 0
  //     let onerrorCalled = 0

  //     img.onload = () => { onloadCalled += 1 }
  //     img.onerror = () => { onerrorCalled += 1 }

  //     img.src = `${png_clock}s1`
  //     assert.strictEqual(img.src, `${png_clock}s1`)

  //     img.src = `${png_clock}s2`
  //     assert.strictEqual(img.src, `${png_clock}s2`)

  //     assert.strictEqual(onerrorCalled, 2)

  //     onerrorCalled = 0
  //     img.onerror = () => { onerrorCalled += 1 }

  //     img.src = `${png_clock}s3`
  //     assert.strictEqual(img.src, `${png_clock}s3`)

  //     assert.strictEqual(onerrorCalled, 1)
  //     assert.strictEqual(onloadCalled, 0)
  //   })
  // })

  it('Image#src set null', function () {
    return loadImage(PNG_CLOCK).then((img) => {
      img.src = null
      assert.equal(img.width, 0)
      assert.equal(img.height, 0)
    })
  })

  // it('Image#src set empty buffer', function () {
  //   return loadImage(png_clock).then((img) => {
  //     let onerrorCalled = 0

  //     img.onerror = () => { onerrorCalled += 1 }

  //     img.src = Buffer.alloc(0)
  //     assert.strictEqual(img.width, 0)
  //     assert.strictEqual(img.height, 0)
  //     assert.strictEqual(img.complete, true)

  //     assert.strictEqual(onerrorCalled, 1)
  //   })
  // })

  // it('should unbind Image#onload', function () {
  //   return loadImage(png_clock).then((img) => {
  //     let onloadCalled = 0

  //     img.onload = () => { onloadCalled += 1 }

  //     img.src = png_checkers
  //     assert.strictEqual(img.src, png_checkers)
  //     assert.strictEqual(img.complete, true)
  //     assert.strictEqual(img.width, 2)
  //     assert.strictEqual(img.height, 2)

  //     assert.strictEqual(onloadCalled, 1)

  //     onloadCalled = 0
  //     img.onload = null

  //     img.src = png_clock
  //     assert.strictEqual(img.src, png_clock)
  //     assert.strictEqual(img.complete, true)
  //     assert.strictEqual(img.width, 320)
  //     assert.strictEqual(img.height, 320)

  //     assert.strictEqual(onloadCalled, 0)
  //   })
  // })

  // it('should unbind Image#onerror', function () {
  //   return loadImage(png_clock).then((img) => {
  //     let onloadCalled = 0
  //     let onerrorCalled = 0

  //     img.onload = () => { onloadCalled += 1 }
  //     img.onerror = () => { onerrorCalled += 1 }

  //     img.src = `${png_clock}s1`
  //     assert.strictEqual(img.src, `${png_clock}s1`)

  //     img.src = `${png_clock}s2`
  //     assert.strictEqual(img.src, `${png_clock}s2`)

  //     assert.strictEqual(onerrorCalled, 2)

  //     onerrorCalled = 0
  //     img.onerror = null

  //     img.src = `${png_clock}s3`
  //     assert.strictEqual(img.src, `${png_clock}s3`)

  //     assert.strictEqual(onloadCalled, 0)
  //     assert.strictEqual(onerrorCalled, 0)
  //   })
  // })

  it('does not crash on invalid images', function () {
    function withIncreasedByte (source, index) {
      const copy = source.slice(0)

      copy[index] += 1

      return copy
    }

    const source = fs.readFileSync(JPG_CHROME)

    const corruptSources = [
      withIncreasedByte(source, 0),
      withIncreasedByte(source, 1),
      withIncreasedByte(source, 1060),
      withIncreasedByte(source, 1061),
      withIncreasedByte(source, 1062),
      withIncreasedByte(source, 1063),
      withIncreasedByte(source, 1064),
      withIncreasedByte(source, 1065),
      withIncreasedByte(source, 1066),
      withIncreasedByte(source, 1067),
      withIncreasedByte(source, 1068),
      withIncreasedByte(source, 1069)
    ]

    return Promise.all(corruptSources.map(src => loadImage(src).catch(() => null)))
  })

  describe('supports BMP', function () {
    it('parses 1-bit image', function () {
      return loadImage(path.join(BMP_DIR, '1-bit.bmp')).then((img) => {
        assert.equal(img.width, 111)
        assert.equal(img.height, 72)
      })
    })

    it('parses 4-bit image', function () {
      return loadImage(path.join(BMP_DIR, '4-bit.bmp')).then((img) => {
        assert.equal(img.width, 32)
        assert.equal(img.height, 32)
      })
    })

    it('parses 24-bit image', function () {
      return loadImage(path.join(BMP_DIR, '24-bit.bmp')).then((img) => {
        assert.equal(img.width, 2)
        assert.equal(img.height, 2)

        testImgd(img, [
          0, 0, 255, 255,
          0, 255, 0, 255,
          255, 0, 0, 255,
          255, 255, 255, 255
        ])
      })
    })

    it('parses 32-bit image', function () {
      return loadImage(path.join(BMP_DIR, '32-bit.bmp')).then((img) => {
        assert.equal(img.width, 4)
        assert.equal(img.height, 2)

        testImgd(img, [
          0, 0, 255, 255,
          0, 255, 0, 255,
          255, 0, 0, 255,
          255, 255, 255, 255,
          0, 0, 255, 127,
          0, 255, 0, 127,
          255, 0, 0, 127,
          255, 255, 255, 127
        ])
      })
    })

    it('parses minimal BMP', function () {
      return loadImage(path.join(BMP_DIR, 'min.bmp')).then((img) => {
        assert.equal(img.width, 1)
        assert.equal(img.height, 1)

        testImgd(img, [
          255, 0, 0, 255
        ])
      })
    })

    it('properly handles negative height', function () {
      return loadImage(path.join(BMP_DIR, 'negative-height.bmp')).then((img) => {
        assert.equal(img.width, 1)
        assert.equal(img.height, 2)

        testImgd(img, [
          255, 0, 0, 255,
          0, 255, 0, 255
        ])
      })
    })

    it('color palette', function () {
      return loadImage(path.join(BMP_DIR, 'palette.bmp')).then((img) => {
        assert.equal(img.width, 32)
        assert.equal(img.height, 32)
      })
    })

    it('V3 header', function () {
      return loadImage(path.join(BMP_DIR, 'v3-header.bmp')).then((img) => {
        assert.equal(img.width, 256)
        assert.equal(img.height, 192)
      })
    })

    // it('catches BMP errors', function (done) {
    //   const img = new Image()

    //   img.onload = () => {
    //     throw new Error('Invalid image should not be loaded properly')
    //   }

    //   img.onerror = err => {
    //     const msg = 'Error while processing file header - unexpected end of file'
    //     assert.strictEqual(err.message, msg)
    //     done()
    //   }

    //   img.src = Buffer.from('BM')
    // })

    it('BMP bomb', function () {
      return loadImage(path.join(BMP_DIR, 'bomb.bmp')).then((img) => {
        assert.equal(img.width, 4135)
        assert.equal(img.height, 4135)
      })
    })

    function testImgd (img, data) {
      const ctx = createCanvas(img.width, img.height).getContext('2d')
      ctx.drawImage(img, 0, 0)
      const actualData = ctx.getImageData(0, 0, img.width, img.height).data

      assert.equal(String(actualData), String(data))
    }
  })
})
