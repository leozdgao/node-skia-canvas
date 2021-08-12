const fs = require('fs');
const { Canvas } = require('./build/Debug/node-skia.node');

const c = new Canvas(500, 500);
const ctx = c.getContext('2d');

ctx.fillStyle = "#FFF";
ctx.fillRect(0, 0, 500, 500)


// Draw the ellipse
// ctx.beginPath();
// ctx.ellipse(100, 100, 50, 75, Math.PI / 4, 0, 2 * Math.PI, false);
// ctx.stroke();

// // Draw the ellipse's line of reflection
// ctx.beginPath();
// ctx.setLineDash([5, 5]);
// ctx.moveTo(0, 200);
// ctx.lineTo(200, 0);
// ctx.stroke();

ctx.save();

ctx.fillStyle = 'blue';
// Create circular clipping region
ctx.beginPath();
ctx.arc(100, 75, 50, 1, 360);
// ctx.fill();
// ctx.rect(100, 75, 80, 40)
ctx.clip();

ctx.restore();

// Draw stuff that gets clipped
ctx.fillStyle = 'blue';
ctx.fillRect(0, 0, c.width, c.height);
ctx.fillStyle = 'orange';
ctx.fillRect(0, 0, 100, 100);



fs.writeFileSync("temp_path.png", c.toBuffer());
