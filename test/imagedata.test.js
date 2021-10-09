
const assert = require('assert')
const { Canvas } = require('../lib')

describe('ImageData', function () {
  const canvas = new Canvas(20, 20)
  const ctx = canvas.getContext('2d')

  it('should throw with invalid numeric arguments', function () {
    assert.throws(() => { ctx.createImageData(0, 0) }, /width for/)
    assert.throws(() => { ctx.createImageData(1, 0) }, /height for/)
    assert.throws(() => { ctx.createImageData(0) }, Error)
  })

  it('should construct with width and height', function () {
    const imageData = ctx.createImageData(2, 3)

    assert.equal(imageData.width, 2)
    assert.equal(imageData.height, 3)

    assert.equal(imageData.data instanceof Uint8ClampedArray, true)
    assert.equal(imageData.data.length, 24)
  })

  it('should construct with width and height', function () {
    const anotherImageData = ctx.createImageData(4, 5)
    const imageData = ctx.createImageData(anotherImageData)

    assert.equal(imageData.width, 4)
    assert.equal(imageData.height, 5)

    assert.equal(imageData.data instanceof Uint8ClampedArray, true)
    assert.equal(imageData.data.length, 80)
  })
})
