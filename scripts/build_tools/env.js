// Before build this package, we need build skia first
// use `depot_tools` from google git to ensure stable source of building tools

const os = require('os')
const path = require('path')

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
const IS_MAC = PLATFORM === 'darwin'

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
  }
}

module.exports = {
  IS_MAC,
  TOOLS
}
