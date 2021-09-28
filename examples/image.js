const fs = require('fs')
const { Image } = require('../build/Debug/node-skia.node')

const imgData = fs.readFileSync('./examples/leize.jpeg')
const image = new Image()
image.src = imgData

// console.log(image.src);
console.log('=== images', image.width, image.height)

fs.writeFileSync('temp_tt.jpg', image.src)
