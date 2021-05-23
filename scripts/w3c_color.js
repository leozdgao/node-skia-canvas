/**
 * Generate const skia colors for cpp
 */

const fs = require('fs');
const path = require('path');

const content = fs.readFileSync(path.join(__dirname, './w3c_extended_color'), 'utf-8');

const lines = content.split('\n');

const temp_w3c_extedend_color_cpp = lines.reduce((result, line) => {
    const [ name, rgbStr ] = line.split('\t');
    const r = rgbStr.slice(1, 3);
    const g = rgbStr.slice(3, 5);
    const b = rgbStr.slice(5, 7);

    result += `{ "${name}", SkColorSetARGB(0xFF, 0x${r}, 0x${g}, 0x${b}) },\n`;

    return result;
}, '');

fs.writeFileSync('temp_w3c_extedend_color_cpp', temp_w3c_extedend_color_cpp);

