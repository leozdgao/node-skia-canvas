const fs = require('fs');
const { createCanvas } = require('../lib');

const canvas = createCanvas(300, 300);
const ctx = canvas.getContext('2d');

ctx.fillStyle = '#FFF';
ctx.fillRect(0, 0, 300, 300);


ctx.fillStyle = 'blue';
ctx.font = "normal 700 32px \"PingFang SC\"";
ctx.fillText("你好世界 哈哈 13114", 0, 0);


fs.writeFileSync("temp_font.png", canvas.toBuffer());
