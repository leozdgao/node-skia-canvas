const fs = require('fs')
const { Canvas } = require('../')

const c = new Canvas(300, 300)
const ctx = c.getContext('2d')

ctx.fillStyle = '#FFF'
ctx.fillRect(0, 0, 300, 300)

// Moved square
// ctx.translate(110, 30);
// ctx.fillStyle = 'red';
// ctx.fillRect(0, 0, 80, 80);

// // Reset current transformation matrix to the identity matrix
// ctx.setTransform(1, 0, 0, 1, 0, 0);

// // Unmoved square
// ctx.fillStyle = 'gray';
// ctx.fillRect(0, 0, 80, 80);

// ctx.restore();

// Point of transform origin
// ctx.arc(0, 0, 5, 0, 2 * Math.PI)
// ctx.fillStyle = 'blue'
// ctx.fill()

// Non-rotated rectangle
ctx.fillStyle = 'gray'
ctx.fillRect(100, 0, 80, 20)

// Rotated rectangle
ctx.rotate(NaN)
// ctx.translate(-100, 50);
ctx.fillStyle = 'red'
ctx.fillRect(100, 0, 80, 20)

console.log(ctx.getTransform())

// Reset transformation matrix to the identity matrix
// ctx.setTransform(1, 0, 0, 1, 0, 0);

fs.writeFileSync('temp_transform.png', c.toBuffer())
