const fs = require('fs');
const { Canvas } = require('./build/Debug/node-skia.node');

// console.log(mod.hello('5yasudjfas'))

// console.log(mod)

// console.log(mod)

const c = new Canvas(300, 300);
console.log(c);

// c.toBuffer();
const ctx = c.getContext('2d');

console.log('context', ctx);
console.log(ctx.fillStyle);
ctx.fillStyle = "red";
console.log(ctx.fillStyle);

ctx.fillText('It`s OK');

ctx.fillStyle = "yellowgreen";

ctx.fillText('aaaa');

const buffer = c.toBuffer();
fs.writeFileSync('temp_a.png', Buffer.from(buffer));
