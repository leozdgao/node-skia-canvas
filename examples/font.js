const fs = require('fs');
const { Canvas } = require('../build/Debug/node-skia.node');

const canvas = new Canvas(300, 300);
const ctx = canvas.getContext('2d');

ctx.fillStyle = '#FFF';
ctx.fillRect(0, 0, 300, 300);

ctx.font = "PingFang SC";


fs.writeFileSync("temp_font.png", c.toBuffer());
