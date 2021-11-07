function run(ctx) {
  ctx.fillStyle = '#FFF'
  ctx.fillRect(0, 0, 256, 256)

  ctx.fillStyle = '#000'
  ctx.textBaseline = 'top'
  ctx.font = '24px/1.2 "PingFang SC"'
  // ctx.textDecoration = 'line-through'
  ctx.fillText('Hello World, skia', 0, 0, 200)
  ctx.fillText('你好，世界！项目 with skia', 0, 100, 200)

  console.log(
    ctx.measureText('你好，世界！项目 with skia', 200)
  )
}

run.width = 256
run.height = 256

module.exports = run
