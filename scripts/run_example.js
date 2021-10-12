const fs = require('fs')
const path = require('path')
const argv = require('yargs-parser')(process.argv.slice(2))
const logger = require('./build_tools/logger')
const { createCanvas } = require('../')

const exampleFile = argv._[0]
let exampleModule = null

if (exampleFile) {
  const potantialPath = path.join(__dirname, '../examples', `${exampleFile}.js`)

  if (fs.existsSync(potantialPath)) {
    exampleModule = require(potantialPath)
  }
}

if (typeof exampleModule != 'function') {
  logger.warn(`Could not find example \'${exampleFile}\'`)
  process.exit(1)
}

const width = exampleModule.width || 300
const height = exampleModule.height || 300

const canvas = createCanvas(width, height)
const ctx = canvas.getContext('2d')

exampleModule(ctx, canvas)

fs.writeFileSync(`temp_${exampleFile}.png`, canvas.toBuffer())
