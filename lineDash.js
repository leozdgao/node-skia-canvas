const fs = require('fs');
const { Canvas } = require('./build/Debug/node-skia.node');

const c = new Canvas(300, 300);
const ctx = c.getContext('2d');

ctx.fillStyle = "#FFF";
ctx.fillRect(0, 0, 300, 300);

// console.log(ctx.lineCap, ctx.lineJoin, ctx.lineDashOffset, ctx.miterLimit);

// ctx.beginPath();
// ctx.setLineDash([5, 15]);
// ctx.moveTo(0, 50);
// ctx.lineTo(300, 50);
// ctx.stroke();

// console.log(ctx.getLineDash());

// console.log(ctx.lineCap, ctx.lineJoin, ctx.lineDashOffset, ctx.miterLimit);

// // Solid line
// ctx.beginPath();
// ctx.setLineDash([]);
// ctx.moveTo(0, 100);
// ctx.lineTo(300, 100);
// ctx.stroke();

// console.log(ctx.lineCap, ctx.lineJoin, ctx.lineDashOffset, ctx.miterLimit);

// function drawDashedLine(pattern) {
//     ctx.beginPath();
//     ctx.setLineDash(pattern);
//     ctx.moveTo(0, y);
//     ctx.lineTo(300, y);
//     ctx.stroke();
//     y += 20;
//   }

//   let y = 15;

// drawDashedLine([]);
// drawDashedLine([1, 1]);
// drawDashedLine([10, 10]);
// drawDashedLine([20, 5]);
// drawDashedLine([15, 3, 3, 3]);
// drawDashedLine([20, 3, 3, 3, 3, 3, 3, 3]);
// drawDashedLine([12, 3, 3]);  // Equals [12, 3, 3, 12, 3, 3]
// ctx.lineWidth = 20;
// ctx.lineJoin = 'round';
// ctx.beginPath();
// ctx.moveTo(20, 20);
// ctx.lineTo(190, 100);
// ctx.lineTo(280, 20);
// ctx.lineTo(280, 150);
// ctx.stroke();


// Draw guides
// ctx.strokeStyle = '#09f';
// ctx.lineWidth   = 2;
// ctx.strokeRect(-5, 50, 160, 50);

// Set line styles
// ctx.strokeStyle = '#000';
// ctx.lineWidth = 10;

// ctx.miterLimit = 6;

// Draw lines
// ctx.beginPath();
// ctx.moveTo(0, 100);
// for (i = 0; i < 24 ; i++) {
//   var dy = i % 2 == 0 ? 25 : -25;
//   ctx.lineTo(Math.pow(i, 1.5) * 2, 75 + dy);
// }
// ctx.stroke();

const buffer = c.toBuffer();
fs.writeFileSync('temp_line_dash.png', Buffer.from(buffer));

console.log('over~')

