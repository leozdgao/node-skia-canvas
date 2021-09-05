exports.colorVectorToHex = (vector) => {
  if (!Array.isArray(vector)) {
    return null
  }

  const r = Math.round(vector[0] * 255)
  const g = Math.round(vector[1] * 255)
  const b = Math.round(vector[2] * 255)
  const a = Math.round(vector[3] * 100) / 100

  // no opacity, convert to #RRGGBB
  if (a === 1) {
    return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`
  }
  // or convert to rgba(r, g, b, a)
  return `rgba(${r}, ${g}, ${b}, ${a})`
}

exports.isValidColorStr = (color) => {
  if (typeof color !== 'string') {
    return false
  }

  if (color.startsWith('#')) {
    return /^#[a-fA-F0-9]{3,8}$/.test(color)
  }

  if (color.startsWith('rgba(') || color.startsWith('rgb(') || color.startsWith('hsla') || color.startsWith('hsl')) {
    return true
  }

  return false
}
