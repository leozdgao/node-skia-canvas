const fs = require('fs')
const path = require('path')
const { createCanvas, registerFont, Image } = require('../lib')


// const fs = require('fs')
// const path = require('path')
const util = require('util')

const readFile = util.promisify(fs.readFile)
const loadImage = async (src) => {
  const img = new Image()
  img.src = await readFile(src)

  return img
}

// registerFont(path.join(__dirname, './OswaldRegular.ttf'), {
//   family: 'Oswald'
// })
// registerFont(path.join(__dirname, './OswaldLight.ttf'))
// registerFont(path.join(__dirname, './OswaldBold.ttf'))

// const canvas = createCanvas(500, 800)
// const ctx = canvas.getContext('2d')

// ctx.fillStyle = 'hsl(124.5, 76.1%, 47.6%)'
// console.log(ctx.fillStyle)

// ctx.fillStyle = '#FFF'
// ctx.fillRect(0, 0, 500, 800)

// ctx.fillStyle = '#000'

// ctx.font = 'normal 400 24px Oswald'
// ctx.fillText('Abcdefghijklmnop 0123456789', 10, 20)

// ctx.font = 'normal 700 24px Oswald'
// ctx.fillText('Abcdefghijklmnop 0123456789', 10, 50)

// ctx.font = 'normal 100 24px Oswald'
// ctx.fillText('Abcdefghijklmnop 0123456789', 10, 80)

// // TextBaseline
// const baselines = ['top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom']

// ctx.font = '36px serif'
// ctx.strokeStyle = 'red'

// baselines.forEach(function (baseline, index) {
//   ctx.textBaseline = baseline
//   const y = 100 + index * 75
//   ctx.beginPath()
//   ctx.moveTo(0, y + 0.5)
//   ctx.lineTo(550, y + 0.5)
//   ctx.stroke()

//   ctx.fillText('Abcdefghijklmnop (' + baseline + ')', 0, y)
// })

// // TextAlign
// const x = canvas.width / 2

// ctx.fillStyle = '#000'

// ctx.beginPath()
// ctx.moveTo(x, 500)
// ctx.lineTo(x, canvas.height)
// ctx.stroke()

// ctx.font = '30px serif'

// ctx.textAlign = 'left'
// ctx.fillText('left-aligned', x, 540)

// ctx.textAlign = 'center'
// ctx.fillText('center-aligned', x, 585)

// ctx.textAlign = 'right'
// ctx.fillText('right-aligned', x, 630)

// fs.writeFileSync('temp_font.png', canvas.toBuffer())

loadImage(path.join(__dirname, '../__tests__/fixtures/checkers.png')).then((img) => {
  const canvas = createCanvas(20, 20)
  const ctx = canvas.getContext('2d')

  ctx.scale(0.1, 0.3)
  const mat1 = ctx.getTransform()
console.log(mat1)
  const buffer = canvas.toBuffer('image/png')
  require('fs').writeFileSync('temp_cc.png', buffer)

  // const imageData = ctx.getImageData(0, 0, 20, 20)
  // console.log(imageData.data)
}).catch(e => console.log(e))
