
const { Canvas } = require('../')

describe('ImageData', function () {
  const canvas = new Canvas(20, 20)
  const ctx = canvas.getContext('2d')

  it('should throw with invalid numeric arguments', function () {
    expect(() => { ctx.createImageData(0, 0) }).toThrow(/width is zero/)
    expect(() => { ctx.createImageData(1, 0) }).toThrow(/height is zero/)
    expect(() => { ctx.createImageData(0) }).toThrow(TypeError)
  })

  it('should construct with width and height', function () {
    const imageData = ctx.createImageData(2, 3)

    expect(imageData.width).toBe(2)
    expect(imageData.height).toBe(3)

    expect(imageData.data instanceof Uint8ClampedArray).toBe(true)
    expect(imageData.data.length).toBe(24)
  })

  it('should throw with invalid typed array', function () {
    expect(() => { ctx.createImageData(new Uint8ClampedArray(0), 0) }, /input data has a zero byte length/)
    expect(() => { ctx.createImageData(new Uint8ClampedArray(3), 0) }, /source width is zero/)
    // Note: Some errors thrown by browsers are not thrown by node-canvas
    // because our ImageData can support different BPPs.
  })

  it('should construct with Uint8ClampedArray', function () {
    let data, imageData

    data = new Uint8ClampedArray(2 * 3 * 4)
    imageData = ctx.createImageData(data, 2)
    expect(imageData.width).toBe(2)
    expect(imageData.height).toBe(3)
    expect(imageData.data instanceof Uint8ClampedArray).toBe(true)
    expect(imageData.data.length).toBe(24)

    data = new Uint8ClampedArray(3 * 4 * 4)
    imageData = ctx.createImageData(data, 3, 4)
    expect(imageData.width).toBe(3)
    expect(imageData.height).toBe(4)
    expect(imageData.data instanceof Uint8ClampedArray).toBe(true)
    expect(imageData.data.length).toBe(24)
  })

  it('should construct with Uint16Array', function () {
    let data = new Uint16Array(2 * 3 * 2)
    let imageData = ctx.createImageData(data, 2)
    expect(imageData.width).toBe(2)
    expect(imageData.height).toBe(3)
    expect(imageData.data instanceof Uint16Array).toBe(true)
    expect(imageData.data.length).toBe(12)

    data = new Uint16Array(3 * 4 * 2)
    imageData = ctx.createImageData(data, 3, 4)
    expect(imageData.width).toBe(3)
    expect(imageData.height).toBe(4)
    expect(imageData.data instanceof Uint16Array).toBe(true)
    expect(imageData.data.length).toBe(24)
  })
})
