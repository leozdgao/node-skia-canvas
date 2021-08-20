const styleRE = /^(normal|italic|oblique)$/,
      smallcapsRE = /^(normal|small-caps)$/,
      stretchRE = /^(normal|(semi-|extra-|ultra-)?(condensed|expanded))$/,
      namedSizeRE = /(?:xx?-)?small|smaller|medium|larger|(?:xx?-)?large|normal/,
      numSizeRE = /^([\d\.]+)(px|pt|pc|in|cm|mm|%|em|ex|ch|rem|q)/,
      namedWeightRE = /^(normal|bold(er)?|lighter)$/,
      numWeightRE = /^(1000|\d{1,3})$/,
      parameterizedRE = /([\w\-]+)\((.*?)\)/,
      unquote = s => s.replace(/^(['"])(.*?)\1$/, "$2"),
      isSize = s => namedSizeRE.test(s) || numSizeRE.test(s),
      isWeight = s => namedWeightRE.test(s) || numWeightRE.test(s);


const weightMap = {
  "lighter": 300,
  "normal": 400,
  "bold": 700,
  "bolder": 800
}

const sizeMap = {
  "xx-small": 3/5,
  "x-small": 3/4,
  "small": 8/9,
  "smaller": 8/9,
  "large": 6/5,
  "larger": 6/5,
  "x-large": 3/2,
  "xx-large": 2/1,
  "normal": 1.2 // special case for lineHeight
}
      

function parseFont(str) {
  if (typeof str !== 'string' || !str) {
    throw new TypeError('Font specification must be a non empty string');
  }

  const font = {
    style: 'normal',
    variant: 'normal',
    weight: 400,
    stretch: 'normal'
  };
  const tokens = str.replace(/\s*\/\s*/, '/').split(/\s+/);
  let token;

  while (token = tokens.shift()) {
    let match = styleRE.test(token) ? 'style'
              : smallcapsRE.test(token) ? 'variant'
              : stretchRE.test(token) ? 'stretch'
              : isWeight(token) ? 'weight'
              : isSize(token) ? 'size'
              : null;

    switch (match) {
      case "style":
      case "variant":
      case "stretch":
      case "weight":
        font[match] = token
        break;
      case "size":
        // size is the pivot point between the style fields and the family name stack,
        // so start processing what's been collected
        let [emSize, leading] = token.split('/'),
            size = parseSize(emSize),
            lineHeight = parseSize((leading || '1.2').replace(/(\d)$/, '$1em'), size),
            weight = parseWeight(font.weight),
            family = tokens.join(' ').split(/\s*,\s*/).map(unquote),
            features = font.variant=='small-caps' ? {on:['smcp', 'onum']} : {},
            {style, stretch, variant} = font;

        // make sure all the numeric fields have legitimate values
        let invalid = !isFinite(size) ? `font size "${emSize}"`
                    : !isFinite(lineHeight) ? `line height "${leading}"`
                    : !isFinite(weight) ? `font weight "${font.weight}"`
                    : family.length==0 ? `font family "${tokens.join(', ')}"`
                    : false;

        if (!invalid) {
          // include a re-stringified version of the decoded/absified values
          return Object.assign(font, {
            size, lineHeight, leading: lineHeight, weight, family, features,
            canonical:[
              style,
              (variant !== style) && variant,
              ([variant, style].indexOf(weight) == -1) && weight,
              ([variant, style, weight].indexOf(stretch) == -1) && stretch,
              `${size}px/${lineHeight}px`,
              family.map(nm => nm.match(/\s/) ? `"${nm}"` : nm).join(", ")
            ].filter(Boolean).join(' ')
          });
        } else {
          return null;
        }
    }
  }

  return null;
};

function parseSize(str, emSize=16){
  if (m = numSizeRE.exec(str)){
    let [size, unit] = [parseFloat(m[1]), m[2]]
    return size * (unit == 'px' ? 1
                :  unit == 'pt' ? 1 / 0.75
                :  unit == '%' ? emSize / 100
                :  unit == 'pc' ? 16
                :  unit == 'in' ? 96
                :  unit == 'cm' ? 96.0 / 2.54
                :  unit == 'mm' ? 96.0 / 25.4
                :  unit == 'q' ? 96 / 25.4 / 4
                :  unit.match('r?em') ? emSize
                :  NaN )
  }

  if (m = namedSizeRE.exec(str)){
    return emSize * (sizeMap[m[0]] || 1.0)
  }

  return NaN
}

function parseWeight(str){
  return (m = numWeightRE.exec(str)) ? parseInt(m[0]) || NaN
       : (m = namedWeightRE.exec(str)) ? weightMap[m[0]]
       : NaN
}

module.exports = parseFont;
