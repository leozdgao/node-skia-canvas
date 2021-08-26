const fs = require('fs');
const path = require('path');
const { createCanvas, registerFont } = require('../lib');

registerFont(path.join(__dirname, './OswaldRegular.ttf'), {
  family: 'Oswald'
});
registerFont(path.join(__dirname, './OswaldLight.ttf'), {
  family: 'Oswald'
});
registerFont(path.join(__dirname, './OswaldBold.ttf'), {
  family: 'Oswald'
});

const canvas = createCanvas(500, 800);
const ctx = canvas.getContext('2d');

ctx.fillStyle = '#FFF';
ctx.fillRect(0, 0, 500, 1000);

ctx.fillStyle = '#000';

// ctx.font = "normal 400 10px/12px \"PingFang SC\"";
ctx.font = "10px STFangsong";
console.log(ctx.font)
// console.log(ctx.measureText('你好世界哈哈哈'));
const info = ctx.measureText('asjdkflasdfkjlasdf');
console.log(info.width)
ctx.fillText('asjdkflasdfkjlasdf', 10, 10)

// ctx.font = 'normal 400 24px Oswald';
// ctx.fillText('Abcdefghijklmnop 0123456789', 10, 20);

// ctx.font = 'normal 700 24px Oswald';
// ctx.fillText('Abcdefghijklmnop 0123456789', 10, 50);

// ctx.font = 'normal 100 24px Oswald';
// ctx.fillText('Abcdefghijklmnop 0123456789', 10, 80);

// TextBaseline
const baselines = ['top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom'];

ctx.font = '36px serif';
ctx.strokeStyle = 'red';

baselines.forEach(function (baseline, index) {
  ctx.textBaseline = baseline;
  const y = 100 + index * 75;
  ctx.beginPath();
  ctx.moveTo(0, y + 0.5);
  ctx.lineTo(550, y + 0.5);
  ctx.stroke();

  ctx.fillText('Abcdefghijklmnop (' + baseline + ')', 0, y);
});


// TextAlign
const x = canvas.width / 2;

ctx.fillStyle = '#000';

ctx.beginPath();
ctx.moveTo(x, 500);
ctx.lineTo(x, canvas.height);
ctx.stroke();

ctx.font = '30px serif';

ctx.textAlign = 'left';
ctx.fillText('left-aligned', x, 540);

ctx.textAlign = 'center';
ctx.fillText('center-aligned', x, 585);

ctx.textAlign = 'right';
ctx.fillText('right-aligned', x, 630);

fs.writeFileSync("temp_font.png", canvas.toBuffer());
