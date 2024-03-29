// Before build this package, we need build skia first
// use `depot_tools` from google git to ensure stable source of building tools

// for debug, set SKIA_DEBUG=1 and do not set SKIA_USE_SYSTEM_LIB
// for release, do not set SKIA_DEBUG and SKIA_USE_SYSTEM_LIB=1

const path = require('path')
const { execSync } = require('child_process')
const {
  OMIT_SYNC_DEPS,
  DEP_SYS_LIBS,
  FEATURES, SYSROOT, TOOLS,
  IS_LINUX,
  IS_SKIA_DEBUG,
  SKIA_USE_SYSTEM_LIB,
  PATH_SKIA_SOURCE, PATH_OUTPUT_DIRECTORY,
  toString: envToString
} = require('./env')
const logger = require('./logger')

const yesIf = v => v ? 'true' : 'false'
const yes = () => 'true'
const no = () => 'false'

const exec = (cmd, opts) => {
  const { cwd = process.cwd(), stdio = 'inherit' } = opts || {}
  return execSync(cmd, {
    stdio,
    cwd
  })
}
const findCflagsByPkgConfig = pkgs => {
  if (!Array.isArray(pkgs)) {
    pkgs = [pkgs]
  }

  const flags = []
  const libs = []

  for (const pkg of pkgs) {
    try {
      const cflags = exec(`pkg-config ${pkg} --cflags`, {
        stdio: 'pipe'
      })
      flags.push(...cflags.toString().trim().split(/\s+/).map(str => `"${str}"`))
      const clibs = exec(`pkg-config ${pkg} --libs`, {
        stdio: 'pipe'
      })
      libs.push(...clibs.toString().trim().split(/\s+/).map(str => `"${str}"`))
    } catch (e) {
      logger.error(`Dependency '${pkg}' could not find.`)
      process.exit(1)
    }
  }

  return {
    cflags: flags,
    ldflags: libs
  }
}

logger.info(envToString())

// ========== START: generate args for gn ==========
const argsForGN = [
  ['cc', '"clang"'],
  ['cxx', '"clang++"'],
  ['is_official_build', yesIf(!IS_SKIA_DEBUG)],
  // ['is_component_build', no()], // static link is just enough
  ['is_debug', yesIf(IS_SKIA_DEBUG)],
  ['skia_enable_gpu', yesIf(FEATURES.gpu())],
  ['skia_use_gl', yesIf(FEATURES.gl)],
  ['skia_use_egl', yesIf(FEATURES.egl)],
  ['skia_use_x11', yesIf(FEATURES.x11)],
  ['skia_use_system_libpng', yesIf(SKIA_USE_SYSTEM_LIB)],
  ['skia_use_system_libjpeg_turbo', yesIf(SKIA_USE_SYSTEM_LIB)],
  ['skia_use_system_libwebp', yesIf(SKIA_USE_SYSTEM_LIB)],
  ['skia_use_system_zlib', yesIf(SKIA_USE_SYSTEM_LIB)],
  ['skia_use_xps', no()],
  ['skia_use_dng_sdk', yesIf(FEATURES.dng)]
]

if (FEATURES.vulkan) {
  argsForGN.push(['skia_use_vulkan', yes()])
  argsForGN.push(['skia_enable_spirv_validation', no()])
}

if (FEATURES.metal) {
  argsForGN.push(['skia_use_metal', yes()])
}

if (FEATURES.d3d) {
  argsForGN.push(['skia_use_direct3d', yes()])
}

if (IS_SKIA_DEBUG) {
  argsForGN.push(['skia_enable_spirv_validation', no()])
  argsForGN.push(['skia_enable_tools', no()])
  argsForGN.push(['skia_enable_vulkan_debug_layers', no()])
  argsForGN.push(['skia_use_libheif', no()])
  argsForGN.push(['skia_use_lua', no()])
}

if (FEATURES.text_layout) {
  argsForGN.push(['skia_enable_skshaper', yes()])
  argsForGN.push(['skia_use_icu', yes()])
  argsForGN.push(['skia_use_system_icu', yesIf(SKIA_USE_SYSTEM_LIB)])
  argsForGN.push(['skia_use_harfbuzz', yes()])
  argsForGN.push(['skia_pdf_subset_harfbuzz', yes()])
  argsForGN.push(['skia_use_system_harfbuzz', yesIf(SKIA_USE_SYSTEM_LIB)])
  argsForGN.push(['skia_use_sfntly', no()])
  argsForGN.push(['skia_enable_skparagraph', yes()])
} else {
  argsForGN.push(['skia_use_icu', no()])
}

// OMIT: asmflags
// OMIT: android

const cflags = []
const ldflags = []

if (SYSROOT) {
  cflags.push(`--sysroot=${SYSROOT}`)
}

// ========= START: DEPS PREPARE  =========
//
// need `pkg-config` first
try {
  exec('which pkg-config', {
    stdio: 'pipe'
  })
} catch (e) {
  logger.error('Could not find `pkg-config`')
  process.exit(1)
}

// need fontconfig on linux whether debug or release
if (IS_LINUX) {
  const { cflags: clagsFound, ldflags: ldflagsFound } = findCflagsByPkgConfig(['fontconfig'])

  // conflict for sys libpng
  // if () {
  //   // clagsFound.unshift('"-I/usr/local/include/libpng16"')
  // }

  const cflagsForReal = clagsFound.filter(flag => !/libpng15/.test(flag))

  cflags.push(...cflagsForReal)
  ldflags.push(...ldflagsFound)
}

if (SKIA_USE_SYSTEM_LIB) {
  const { cflags: clagsFound, ldflags: ldflagsFound } = findCflagsByPkgConfig(DEP_SYS_LIBS)
  cflags.push(...clagsFound)
  ldflags.push(...ldflagsFound)
}

// ========= END: DEPS PREPARE =========

// TODO: OPT_LEVEL just hard code for production
// if (!IS_SKIA_DEBUG) {

// }

if (cflags.length > 0) {
  argsForGN.push(['extra_cflags', `[${cflags.join(', ')}]`])
}
if (ldflags.length > 0) {
  argsForGN.push(['extra_ldflags', `[${ldflags.join(', ')}]`])
}

logger.info(`Get gn args: ${JSON.stringify(argsForGN.reduce((ret, i) => {
  const [k, v] = i
  ret[k] = v
  return ret
}, {}), null, 2)}`)
// ========== END: generate args for gn ==========

// ========== START: building ==========
if (!SKIA_USE_SYSTEM_LIB && !OMIT_SYNC_DEPS) {
  logger.info('Synchronizing Skia dependencies: python2 tools/git-sync-deps')

  exec('python2 tools/git-sync-deps', {
    cwd: PATH_SKIA_SOURCE
  })

  logger.info('After synchronize Skia dependencies')
}

// gn config
const GN_CONFIG_COMMAND = `${TOOLS.gn()} gen ${PATH_OUTPUT_DIRECTORY} --args='${argsForGN.map(i => i.join('=')).join(' ')}'`
logger.info(`Config gn: ${GN_CONFIG_COMMAND}`)

exec(GN_CONFIG_COMMAND, {
  cwd: PATH_SKIA_SOURCE
})

// output current build args
exec(`${TOOLS.gn()} args ${PATH_OUTPUT_DIRECTORY} --list --short`, {
  stdio: 'inherit',
  cwd: PATH_SKIA_SOURCE
})

const NINJA_BUILD_COMMAND = `${TOOLS.ninja()} -C ${PATH_OUTPUT_DIRECTORY}`
logger.info(`Start ninja build: ${NINJA_BUILD_COMMAND}`)

exec(NINJA_BUILD_COMMAND, {
  cwd: PATH_SKIA_SOURCE
})

// ========== END: building ==========
logger.info(`Build success to ${PATH_OUTPUT_DIRECTORY}`)
