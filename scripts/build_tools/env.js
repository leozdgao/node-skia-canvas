// Before build this package, we need build skia first
// use `depot_tools` from google git to ensure stable source of building tools

const os = require('os')
const path = require('path')
const argv = require('yargs-parser')(process.argv.slice(2))

const PATH_WORKSPACE = path.join(__dirname, '../../')
const PATH_SKIA_SOURCE = path.join(PATH_WORKSPACE, 'skia')
const PATH_DEPOT_TOOLS = path.join(PATH_WORKSPACE, 'depot_tools')

const PLATFORM = (() => {
  const platform = os.platform()
  const arch = os.arch()

  if (platform === 'linux') {
    return `${platform}${arch.slice(1)}`
  }

  return platform
})()

const DEP_SYS_LIBS = ['harfbuzz', 'icu-uc', 'libpng', 'libwebp', 'libturbojpeg']

const IS_MAC = PLATFORM === 'darwin'
const IS_WINDOWS = PLATFORM === 'windows'
const IS_SKIA_DEBUG = !!process.env.SKIA_DEBUG
const SKIA_USE_SYSTEM_LIB = !!process.env.SKIA_USE_SYSTEM_LIB
const SYSROOT = process.env.SYSROOT

const FEATURES = (() => {
  // env first, then argv
  let featuresFromArgs = process.env.SKIA_FEATURES || argv.features || []

  if (typeof featuresFromArgs === 'string') {
    featuresFromArgs = featuresFromArgs.split(/\s*,\s*/)
  }

  return {
    gl: featuresFromArgs.includes('gl'),
    egl: featuresFromArgs.includes('egl'),
    wayland: featuresFromArgs.includes('wayland'),
    x11: featuresFromArgs.includes('x11'),
    vulkan: featuresFromArgs.includes('vulkan'),
    metal: featuresFromArgs.includes('metal'),
    d3d: featuresFromArgs.includes('d3d'),
    text_layout: featuresFromArgs.includes('textlayout'),
    lottie: featuresFromArgs.includes('lottie'),
    dng: false,
    particles: false,
    gpu () { return this.gl || this.vulkan || this.metal || this.d3d }
  }
})()

const TOOLS = {
  gn: () => path.join(PATH_DEPOT_TOOLS, 'gn'),
  ninja: () => {
    let name = 'ninja'

    switch (PLATFORM) {
      case 'darwin': name = 'ninja-mac'; break
      case 'linux32': name = 'ninja-linux32'; break
      case 'linux64': name = 'ninja-linux64'; break
      case 'windows': name = 'ninja.exe'; break
    }

    return path.join(PATH_DEPOT_TOOLS, name)
  },
  cc: () => process.env.CC || 'clang',
  cxx: () => process.env.CC || 'clang++'
}

const PATH_OUTPUT_DIRECTORY = IS_SKIA_DEBUG ? 'out/Debug' : 'out/Release'

module.exports = {
  DEP_SYS_LIBS,
  PATH_DEPOT_TOOLS,
  PATH_SKIA_SOURCE,
  PATH_OUTPUT_DIRECTORY,
  IS_MAC,
  IS_WINDOWS,
  IS_SKIA_DEBUG,
  SKIA_USE_SYSTEM_LIB,
  FEATURES,
  SYSROOT,
  TOOLS
}
