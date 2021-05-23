const fs = require('fs');
const { Canvas } = require('./build/Debug/node-skia.node');

const c = new Canvas(300, 300);


console.log(c.width, c.height);
const ctx = c.getContext('2d');

console.log('context', ctx);
console.log(ctx.fillStyle);
ctx.fillStyle = "yellowgreen";
console.log(ctx.fillStyle);

ctx.fillText('It`s OK');

ctx.fillStyle = "rgba(0x46,0x82,0xb4)";
ctx.fillStyle = "#4682b4"
ctx.fillStyle = "hsl(120, 1, 0.5)";

console.log(ctx.fillStyle.map(i => (Math.floor(i * 255)).toString(16)));

ctx.fillText('aaaa');

const buffer = c.toBuffer();
fs.writeFileSync('temp_a.png', Buffer.from(buffer));
