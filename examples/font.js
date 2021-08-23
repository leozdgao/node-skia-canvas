const fs = require('fs');
const path = require('path');
const { createCanvas, registerFont } = require('../lib');

console.log(path.join(__dirname, './OswaldRegular.ttf'))

registerFont(path.join(__dirname, './OswaldRegular.ttf'), {
  family: 'Oswald'
});

const canvas = createCanvas(500, 800);
const ctx = canvas.getContext('2d');

ctx.fillStyle = '#FFF';
ctx.fillRect(0, 0, 500, 1000);

ctx.fillStyle = '#000';

ctx.font = '24px Oswald';
ctx.fillText('Abcdefghijklmnop', 40, 40);

// TextBaseline
const baselines = ['top', 'hanging', 'middle', 'alphabetic', 'ideographic', 'bottom'];

ctx.font = '36px serif';
ctx.strokeStyle = 'red';

baselines.forEach(function (baseline, index) {
  ctx.textBaseline = baseline;
  const y = 75 + index * 75;
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
