function run(ctx) {
  ctx.fillStyle = '#FFF'
  ctx.fillRect(0, 0, 256, 256)

  ctx.fillStyle = '#4285F4'
  ctx.fillRect(10, 10, 100, 160)

  ctx.fillStyle = '#0F9D58'
  ctx.beginPath()
  ctx.arc(180, 50, 25, 0, 2 * Math.PI)
  ctx.closePath()
  ctx.fill()

  ctx.fillStyle = '#DB4437'
  ctx.beginPath()
  ctx.ellipse(100, 160, 50, 80, 0, 0, 2 * Math.PI)
  ctx.closePath()
  ctx.fill()

  ctx.lineWidth = 5
  ctx.strokeStyle = '#F4B400'
  ctx.strokeRect(80, 50, 100, 160)
}

run.width = 256
run.height = 256

module.exports = run
