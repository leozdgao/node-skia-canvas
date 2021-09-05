const { registerFont } = require('../')
const parseFont = require('../lib/parseFont')

test('.parseFont()', function () {
  const tests = [
    '20px sans-serif',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['sans-serif'], canonical: 'normal 400 20px/24px sans-serif' },
    '20.5pt Arial',
    { size: 27.333333333333332, lineHeight: 32.8, leading: 32.8, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 400 27.333333333333332px/32.8px Arial' },
    '20mm Arial',
    { size: 75.59055118110237, lineHeight: 90.70866141732284, leading: 90.70866141732284, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 400 75.59055118110237px/90.70866141732284px Arial' },
    '50px Helvetica ,  Arial, sans-serif',
    { size: 50, lineHeight: 60, leading: 60, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Helvetica', 'Arial', 'sans-serif'], canonical: 'normal 400 50px/60px Helvetica, Arial, sans-serif' },
    '50px "Helvetica Neue", sans-serif',
    { size: 50, lineHeight: 60, leading: 60, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Helvetica Neue', 'sans-serif'], canonical: 'normal 400 50px/60px "Helvetica Neue", sans-serif' },
    'bold italic 50px Arial, sans-serif',
    { size: 50, lineHeight: 60, leading: 60, features: {}, weight: 700, style: 'italic', variant: 'normal', stretch: 'normal', family: ['Arial', 'sans-serif'], canonical: 'italic normal 700 50px/60px Arial, sans-serif' },
    'italic 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 400, style: 'italic', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'italic normal 400 20px/24px Arial' },
    'oblique 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 400, style: 'oblique', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'oblique normal 400 20px/24px Arial' },
    'normal 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 400 20px/24px Arial' },
    '300 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 300, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 300 20px/24px Arial' },
    '800 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 800, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 800 20px/24px Arial' },
    'bolder 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 800, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 800 20px/24px Arial' },
    'lighter 20px Arial',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 300, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial'], canonical: 'normal 300 20px/24px Arial' },
    'normal normal normal 16px Impact',
    { size: 16, lineHeight: 19.2, leading: 19.2, features: {}, weight: 400, family: ['Impact'], style: 'normal', variant: 'normal', canonical: 'normal 400 16px/19.2px Impact' },
    'italic small-caps bolder 16px cursive',
    { size: 16, lineHeight: 19.2, leading: 19.2, features: { on: ['smcp', 'onum'] }, weight: 800, family: ['cursive'], style: 'italic', variant: 'small-caps', canonical: 'italic small-caps 800 normal 16px/19.2px cursive' },
    '20px "new century schoolbook", serif',
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 400, family: ['new century schoolbook', 'serif'], style: 'normal', variant: 'normal', canonical: 'normal 400 20px/24px "new century schoolbook", serif' },
    '20px "Arial bold 300"', // synthetic case with weight keyword inside family
    { size: 20, lineHeight: 24, leading: 24, features: {}, weight: 400, style: 'normal', variant: 'normal', stretch: 'normal', family: ['Arial bold 300'], canonical: 'normal 400 20px/24px "Arial bold 300"' }
  ]

  for (let i = 0, len = tests.length; i < len; ++i) {
    const str = tests[i++]
    const expected = tests[i]
    const actual = parseFont(str)

    if (!expected.style) expected.style = 'normal'
    if (!expected.weight) expected.weight = 'normal'
    if (!expected.stretch) expected.stretch = 'normal'
    if (!expected.variant) expected.variant = 'normal'

    expect(actual).toEqual(expected)
  }

  // not match
  expect(parseFont('Helvetica, sans')).toBeNull()
})

test('registerFont', function () {
  // Minimal test to make sure nothing is thrown
  registerFont('./examples/OswaldBold.ttf')
  registerFont('./examples/OswaldRegular.ttf', { family: 'Oswald' })
  registerFont('./examples/OswaldLight.ttf', { family: 'Oswald' })
})
