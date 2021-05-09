const { Canvas } = require('./build/Debug/node-skia.node');

// console.log(mod.hello('5yasudjfas'))

// console.log(mod)

// console.log(mod)

const c = new Canvas();
console.log(c);

c.toBuffer();
c.getContext('2d');


// setTimeout(() => {
//     console.log('another loop')
// }, 1000);
