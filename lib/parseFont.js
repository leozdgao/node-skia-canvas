const styleRE = /^(normal|italic|oblique)$/
const smallcapsRE = /^(normal|small-caps)$/
const stretchRE = /^(normal|(semi-|extra-|ultra-)?(condensed|expanded))$/
const namedSizeRE = /(?:xx?-)?small|smaller|medium|larger|(?:xx?-)?large|normal/
const numSizeRE = /^([\d.]+)(px|pt|pc|in|cm|mm|%|em|ex|ch|rem|q)/
const namedWeightRE = /^(normal|bold(er)?|lighter)$/
const numWeightRE = /^(1000|\d{1,3})$/
const unquote = s => s.replace(/^(['"])(.*?)\1$/, '$2')
const isSize = s => namedSizeRE.test(s) || numSizeRE.test(s)
const isWeight = s => namedWeightRE.test(s) || numWeightRE.test(s)

const weightMap = {
  lighter: 300,
  normal: 400,
  bold: 700,
  bolder: 800
}

const sizeMap = {
  'xx-small': 3 / 5,
  'x-small': 3 / 4,
  small: 8 / 9,
  smaller: 8 / 9,
  large: 6 / 5,
  larger: 6 / 5,
  'x-large': 3 / 2,
  'xx-large': 2 / 1,
  normal: 1.2 // special case for lineHeight
}

function parseFont (str) {
  if (typeof str !== 'string' || !str) {
    throw new TypeError('Font specification must be a non empty string')
  }

  const font = {
    style: 'normal',
    variant: 'normal',
    weight: 400,
    stretch: 'normal'
  }
  const tokens = str.replace(/\s*\/\s*/, '/').split(/\s+/)
  let token

  // eslint-disable-next-line no-cond-assign
  while (token = tokens.shift()) {
    const match = styleRE.test(token)
      ? 'style'
      : smallcapsRE.test(token)
        ? 'variant'
        : stretchRE.test(token)
          ? 'stretch'
          : isWeight(token)
            ? 'weight'
            : isSize(token)
              ? 'size'
              : null

    switch (match) {
      case 'style':
      case 'variant':
      case 'stretch':
      case 'weight':
        font[match] = token
        break
      case 'size': {
        // size is the pivot point between the style fields and the family name stack,
        // so start processing what's been collected
        const [emSize, leading] = token.split('/')
        const size = parseSize(emSize)
        const lineHeight = parseSize((leading || '1.2').replace(/(\d)$/, '$1em'), size)
        const weight = parseWeight(font.weight)
        const family = tokens.join(' ').split(/\s*,\s*/).map(unquote)
        const features = font.variant === 'small-caps' ? { on: ['smcp', 'onum'] } : {}
        const { style, stretch, variant } = font

        // make sure all the numeric fields have legitimate values
        const invalid = !isFinite(size)
          ? `font size "${emSize}"`
          : !isFinite(lineHeight)
              ? `line height "${leading}"`
              : !isFinite(weight)
                  ? `font weight "${font.weight}"`
                  : family.length === 0
                    ? `font family "${tokens.join(', ')}"`
                    : false

        if (!invalid) {
          // include a re-stringified version of the decoded/absified values
          return Object.assign(font, {
            size,
            lineHeight,
            leading: lineHeight,
            weight,
            family,
            features,
            canonical: [
              style,
              (variant !== style) && variant,
              ([variant, style].indexOf(weight) === -1) && weight,
              ([variant, style, weight].indexOf(stretch) === -1) && stretch,
              `${size}px/${lineHeight}px`,
              family.map(nm => nm.match(/\s/) ? `"${nm}"` : nm).join(', ')
            ].filter(Boolean).join(' ')
          })
        } else {
          return null
        }
      }
    }
  }

  return null
};

function parseSize (str, emSize = 16) {
  let m = numSizeRE.exec(str)

  if (m) {
    const [size, unit] = [parseFloat(m[1]), m[2]]
    return size * (unit === 'px'
      ? 1
      : unit === 'pt'
        ? 1 / 0.75
        : unit === '%'
          ? emSize / 100
          : unit === 'pc'
            ? 16
            : unit === 'in'
              ? 96
              : unit === 'cm'
                ? 96.0 / 2.54
                : unit === 'mm'
                  ? 96.0 / 25.4
                  : unit === 'q'
                    ? 96 / 25.4 / 4
                    : unit.match('r?em')
                      ? emSize
                      : NaN)
  }

  m = namedSizeRE.exec(str)

  if (m) {
    return emSize * (sizeMap[m[0]] || 1.0)
  }

  return NaN
}

function parseWeight (str) {
  let m = numWeightRE.exec(str)

  if (m) {
    return parseInt(m[0]) || NaN
  }

  m = namedWeightRE.exec(str)

  return weightMap[m[0]] || NaN
}

module.exports = parseFont
