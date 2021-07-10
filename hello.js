const fs = require('fs');
const { Canvas, Image } = require('./build/Debug/node-skia.node');

const c = new Canvas(500, 500);


const imgData = fs.readFileSync('./examples/leize.jpeg');
const image = new Image();
image.src = imgData;
console.log(image.src)
console.log('=== images', image.width, image.height);

// fs.writeFileSync('lzlz.jpg', image.src);

console.log(c.width, c.height);
const ctx = c.getContext('2d');

// ctx.fillStyle = '#FFF';
// ctx.fillRect(0, 0, 500, 500);

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

ctx.fillStyle = "red";
ctx.fillRect(100, 100, 50, 50);

console.log('strokeStyle', ctx.strokeStyle);

ctx.strokeStyle = "blue";
ctx.strokeRect(100, 200, 50, 50);
console.log('strokeStyle', ctx.strokeStyle);

console.log('lineWidth', ctx.lineWidth);
ctx.lineWidth = 5;

ctx.strokeRect(200, 200, 50, 50);

ctx.clearRect(120, 120, 20, 20);


ctx.fillStyle = '#000'
console.log(ctx.measureText('123xfg'));

console.log(ctx.textAlign);
ctx.textAlign = 'center';

console.log(ctx.textAlign);
console.log(ctx.textBaseline);
ctx.textBaseline = 'top';
console.log(ctx.measureText('123'));



const img = fs.readFileSync('./examples/leize.jpeg');
ctx.drawImage(img, 0, 0, 1044, 1045, 200, 200, 300, 300);



ctx.fillStyle = 'red';
ctx.beginPath();
ctx.arc(300, 50, 5, 180, 360);
ctx.closePath();
ctx.fill();


/* ctx.fillStyle = 'blue' */

/* ctx.fill() */
// ctx.beginPath()
// ctx.moveTo(10, 10)
// ctx.lineTo(100, 10)
// ctx.lineTo(100, 100)
// ctx.rect(10, 10, 40, 40)
// ctx.lineWidth = 2
// ctx.closePath()

// ctx.beginPath()
// ctx.lineTo(100, 50)
// ctx.rect(10, 70, 80, 40)
// ctx.fillStyle = 'red'
// ctx.arc(200, 100, 5, 180, 360, false)
// ctx.lineTo(100, 200)

// ctx.fill()

// ctx.stroke()
// ctx.strokeStyle = 'blue'
// ctx.lineWidth = 1
// ctx.strokeRect(0, 100, 400, 1)

// ctx.globalAlpha = 0.2;

// ctx.fillStyle = '#FD0';
// ctx.fillRect(0, 0, 75, 75);
// ctx.fillStyle = '#6C0';
// ctx.fillRect(75, 0, 75, 75);
// ctx.fillStyle = '#09F';
// ctx.fillRect(0, 75, 75, 75);
// ctx.fillStyle = '#F30';
// ctx.fillRect(75, 75, 75, 75);
// ctx.fillStyle = '#FFF';

// Set transparency value
// ctx.globalAlpha = 0.2;

// Draw transparent circles
// for (let i = 0; i < 7; i++) {
//     // FIXME: not work
//   ctx.beginPath();
//   ctx.arc(75, 75, 10 + 10 * i, 0, Math.PI * 2, true);
//   ctx.fill();
// }

ctx.fillStyle = 'yellow';
ctx.fillRect(0, 300, 60, 80);

ctx.save();

ctx.fillStyle = 'red';
ctx.fillRect(100, 300, 60, 80);

ctx.restore();

ctx.fillRect(200, 300, 60, 80);

const data = ctx.getImageData(200, 200, 300, 300);

for (let i = 0, l = data.byteLength; i < l; i++) {
    console.log(data)
    // console.log(data[i])
}
// console.log(data.byteLength)
// for ()
// ctx.putImageData(data, 300, 300, 0, 0)

const buffer = c.toBuffer();
fs.writeFileSync('temp_a.png', Buffer.from(buffer));

console.log('over~')
