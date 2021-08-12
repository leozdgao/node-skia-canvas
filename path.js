const fs = require('fs');
const { Canvas } = require('./build/Debug/node-skia.node');

const c = new Canvas(500, 500);
const ctx = c.getContext('2d');

ctx.fillStyle = "#FFF";
ctx.fillRect(0, 0, 500, 500)


// Draw the ellipse
ctx.beginPath();
ctx.ellipse(100, 100, 50, 75, Math.PI / 4, 0, 2 * Math.PI, false);
ctx.stroke();

// Draw the ellipse's line of reflection
ctx.beginPath();
ctx.setLineDash([5, 5]);
ctx.moveTo(0, 200);
ctx.lineTo(200, 0);
ctx.stroke();


fs.writeFileSync("temp_path.png", c.toBuffer());
