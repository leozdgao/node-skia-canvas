const fs = require('fs');
const { Canvas, CanvasGradient, CanvasPattern, Image } = require('../build/Debug/node-skia.node');

const imgData = fs.readFileSync('./examples/leize.jpeg');
const image = new Image();
image.src = imgData;

const c = new Canvas(300, 300);
const ctx = c.getContext('2d');

const ccp = new Canvas(300, 300);
const cctx = c.getContext('2d');

const gradient = new CanvasGradient('linear', 20, 0, 220, 0);
gradient.addColorStop(0, 'green');
gradient.addColorStop(.5, 'cyan');
gradient.addColorStop(1, 'green');


const rgradient = new CanvasGradient('radial', 110, 90, 100, 100, 30, 70);
// Add three color stops
rgradient.addColorStop(0, 'pink');
rgradient.addColorStop(.9, 'white');
rgradient.addColorStop(1, 'green');

// ctx.fillStyle = '#FFF';
// ctx.fillRect(0, 0, 500, 500);

cctx.fillStyle = gradient;
cctx.fillRect(20, 20, 200, 100);

cctx.fillStyle = rgradient;
cctx.fillRect(20, 20, 160, 160);


// const pattern = new CanvasPattern(image, "repeat");
const pattern = new CanvasPattern(ccp, "repeat");
// ctx.fillStyle = "#FFF";
ctx.fillStyle = pattern;
ctx.fillRect(0, 0, 300, 300)


fs.writeFileSync("temp_pattern.png", c.toBuffer());
