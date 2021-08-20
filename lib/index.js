/**
 * fillStyle, strokeStyle getter 格式化
 * lineWidth 默认 1
 * drawImage 多态 API 封装
 * arc 处理 angle 单位问题，以及顺时针逆时针参数
 * createImageData 完全在 JS 侧实现
 */

const Canvas = require('./Canvas');

function createCanvas(width, height) { return new Canvas(width, height); }

module.exports = {
  createCanvas,
  Canvas
};
