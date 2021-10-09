const assert = require('assert')
const { Canvas } = require('../lib')

it('color serialization', function () {
  const canvas = new Canvas(200, 200)
  const ctx = canvas.getContext('2d')

  ;['fillStyle', 'strokeStyle', 'shadowColor'].forEach(function (prop) {
    ctx[prop] = '#FFFFFF'
    assert.equal(ctx[prop], '#ffffff')

    ctx[prop] = '#FFF'
    assert.equal(ctx[prop], '#ffffff')

    ctx[prop] = 'rgba(128, 200, 128, 1)'
    assert.equal(ctx[prop], '#80c880')

    ctx[prop] = 'rgba(128,80,0,0.5)'
    assert.equal(ctx[prop], 'rgba(128, 80, 0, 0.5)')

    ctx[prop] = 'rgba(128,80,0,0.75)'
    assert.equal(ctx[prop], 'rgba(128, 80, 0, 0.75)')

    if (prop === 'shadowColor') return

    const grad = ctx.createLinearGradient(0, 0, 0, 150)
    ctx[prop] = grad
    assert.equal(Object.is(ctx[prop], grad), true)
  })
})

it('color parser', function () {
  const canvas = new Canvas(200, 200)
  const ctx = canvas.getContext('2d')

  ctx.fillStyle = '#ffccaa'
  assert.equal(ctx.fillStyle, '#ffccaa')

  ctx.fillStyle = '#FFCCAA'
  assert.equal(ctx.fillStyle, '#ffccaa')

  ctx.fillStyle = '#FCA'
  assert.equal(ctx.fillStyle, '#ffccaa')

  ctx.fillStyle = '#fff'
  ctx.fillStyle = '#FGG' // only accept a-f
  assert.equal(ctx.fillStyle, '#ffffff')

  ctx.fillStyle = '#fff'
  ctx.fillStyle = 'afasdfasdf'
  assert.equal(ctx.fillStyle, '#ffffff')

  // #rgba and #rrggbbaa
  ctx.fillStyle = '#ffccaa80'
  assert.equal(ctx.fillStyle, 'rgba(255, 204, 170, 0.5)')

  ctx.fillStyle = '#acf8'
  assert.equal(ctx.fillStyle, 'rgba(170, 204, 255, 0.53)')

  ctx.fillStyle = '#BEAD'
  assert.equal(ctx.fillStyle, 'rgba(187, 238, 170, 0.87)')

  ctx.fillStyle = 'rgb(255,255,255)'
  assert.equal(ctx.fillStyle, '#ffffff')

  ctx.fillStyle = 'rgb(0,0,0)'
  assert.equal(ctx.fillStyle, '#000000')

  ctx.fillStyle = 'rgb( 0  ,   0  ,  0)'
  assert.equal(ctx.fillStyle, '#000000')

  ctx.fillStyle = 'rgba( 0  ,   0  ,  0, 1)'
  assert.equal(ctx.fillStyle, '#000000')

  ctx.fillStyle = 'rgba( 255, 200, 90, 0.5)'
  assert.equal(ctx.fillStyle, 'rgba(255, 200, 90, 0.5)')

  ctx.fillStyle = 'rgba( 255, 200, 90, 0.75)'
  assert.equal(ctx.fillStyle, 'rgba(255, 200, 90, 0.75)')

  ctx.fillStyle = 'rgba( 255, 200, 90, 0.7555)'
  assert.equal(ctx.fillStyle, 'rgba(255, 200, 90, 0.75)')

  ctx.fillStyle = 'rgba( 255, 200, 90, .7555)'
  assert.equal(ctx.fillStyle, 'rgba(255, 200, 90, 0.75)')

  ctx.fillStyle = 'rgb(0, 0, 9000)'
  assert.equal(ctx.fillStyle, '#0000ff')

  ctx.fillStyle = 'rgba(0, 0, 0, 42.42)'
  assert.equal(ctx.fillStyle, '#000000')

  // hsl / hsla tests

  ctx.fillStyle = 'hsl(0, 0%, 0%)'
  assert.equal(ctx.fillStyle, '#000000')

  ctx.fillStyle = 'hsl(3600, -10%, -10%)'
  assert.equal(ctx.fillStyle, '#000000')

  ctx.fillStyle = 'hsl(10, 100%, 42%)'
  assert.equal(ctx.fillStyle, '#d62400')

  ctx.fillStyle = 'hsl(370, 120%, 42%)'
  assert.equal(ctx.fillStyle, '#d62400')

  ctx.fillStyle = 'hsl(0, 100%, 100%)'
  assert.equal(ctx.fillStyle, '#ffffff')

  ctx.fillStyle = 'hsl(0, 150%, 150%)'
  assert.equal(ctx.fillStyle, '#ffffff')

  ctx.fillStyle = 'hsl(237, 76%, 25%)'
  assert.equal(ctx.fillStyle, '#0f1470')

  ctx.fillStyle = 'hsl(240, 73%, 25%)'
  assert.equal(ctx.fillStyle, '#11116e')

  ctx.fillStyle = 'hsl(262, 32%, 42%)'
  assert.equal(ctx.fillStyle, '#62498d')

  ctx.fillStyle = 'hsla(0, 0%, 0%, 1)'
  assert.equal(ctx.fillStyle, '#000000')

  ctx.fillStyle = 'hsla(0, 100%, 100%, 1)'
  assert.equal(ctx.fillStyle, '#ffffff')

  ctx.fillStyle = 'hsla(120, 25%, 75%, 0.5)'
  assert.equal(ctx.fillStyle, 'rgba(175, 207, 175, 0.5)')

  ctx.fillStyle = 'hsla(240, 75%, 25%, 0.75)'
  assert.equal(ctx.fillStyle, 'rgba(16, 16, 112, 0.75)')

  ctx.fillStyle = 'hsl(124.5, 76.1%, 47.6%)'
  assert.equal(ctx.fillStyle, '#1dd62b')

  // ctx.fillStyle = 'hsla(172.0, 33.00000e0%, 42%, 1)'
  // assert.equal(ctx.fillStyle, '#488e85')

  // ctx.fillStyle = 'hsl(1.24e2, 760e-1%, 4.7e1%)'
  // assert.equal(ctx.fillStyle, '#1dd329')

  // case-insensitive (#235)
  ctx.fillStyle = 'sILveR'
  assert.equal(ctx.fillStyle, '#c0c0c0')
})
