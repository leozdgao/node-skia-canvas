const { Canvas } = require('./build/Debug/node-skia.node');

// console.log(mod.hello('5yasudjfas'))

// console.log(mod)

// console.log(mod)

const c = new Canvas();
console.log(c);

c.toBuffer();
const ctx = c.getContext('2d');

console.log(ctx);
ctx.fillText();

const ctx0 = c.getContext('2d');
const ctx1 = c.getContext('2d');
const ctx2 = c.getContext('2d');


// setTimeout(() => {
//     console.log('another loop')
// }, 1000);
