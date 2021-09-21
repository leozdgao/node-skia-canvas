
const { Canvas } = require('../')

describe('ImageData', function () {
  const canvas = new Canvas(20, 20)
  const ctx = canvas.getContext('2d')

  it('should throw with invalid numeric arguments', function () {
    expect(() => { ctx.createImageData(0, 0) }).toThrow(/width for/)
    expect(() => { ctx.createImageData(1, 0) }).toThrow(/height for/)
    expect(() => { ctx.createImageData(0) }).toThrow(Error)
  })

  it('should construct with width and height', function () {
    const imageData = ctx.createImageData(2, 3)

    expect(imageData.width).toBe(2)
    expect(imageData.height).toBe(3)

    expect(imageData.data instanceof Uint8ClampedArray).toBe(true)
    expect(imageData.data.length).toBe(24)
  })

  it('should construct with width and height', function () {
    const anotherImageData = ctx.createImageData(4, 5)
    const imageData = ctx.createImageData(anotherImageData)

    expect(imageData.width).toBe(4)
    expect(imageData.height).toBe(5)

    expect(imageData.data instanceof Uint8ClampedArray).toBe(true)
    expect(imageData.data.length).toBe(80)
  })
})
