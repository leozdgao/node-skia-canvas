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

  it('color serialization', function () {
    const canvas = new Canvas(200, 200)
    const ctx = canvas.getContext('2d')

    ;['fillStyle', 'strokeStyle', 'shadowColor'].forEach(function (prop) {
      ctx[prop] = '#FFFFFF'
      expect(ctx[prop]).toBe('#ffffff')

      ctx[prop] = '#FFF'
      expect(ctx[prop]).toBe('#ffffff')

      ctx[prop] = 'rgba(128, 200, 128, 1)'
      expect(ctx[prop]).toBe('#80c880')

      ctx[prop] = 'rgba(128,80,0,0.5)'
      expect(ctx[prop]).toBe('rgba(128, 80, 0, 0.5)')

      ctx[prop] = 'rgba(128,80,0,0.75)'
      expect(ctx[prop]).toBe('rgba(128, 80, 0, 0.75)')

      if (prop === 'shadowColor') return

      const grad = ctx.createLinearGradient(0, 0, 0, 150)
      ctx[prop] = grad
      expect(Object.is(ctx[prop], grad)).toBe(true)
    })
  })
})
