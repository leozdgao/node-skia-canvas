const fs = require('fs');
const { createCanvas } = require('../lib');

const canvas = createCanvas(500, 500);
const ctx = canvas.getContext('2d');

ctx.fillStyle = '#FFF';
ctx.fillRect(0, 0, 500, 500);

// ctx.textBaseline = 'top';
// ctx.fillStyle = 'blue';
// ctx.font = "normal 700 32px/1.2 \"PingFang SC\"";
// ctx.fillText("你好世界 哈哈 13114", 0, 0);

const baselines = ['alphabetic'];
ctx.font = '36px serif';
ctx.fillStyle = '#000';
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

fs.writeFileSync("temp_font.png", canvas.toBuffer());
