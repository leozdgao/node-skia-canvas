const { Canvas } = require('../')

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
})
