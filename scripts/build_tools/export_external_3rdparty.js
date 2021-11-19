const fs = require('fs')
const { execSync } = require('child_process')
const join = require('path').join
const { PATH_SKIA_SOURCE } = require('./env')
const logger = require('./logger')

const PATH_SKIA_EXTERNAL_3RD_PARTY_SOURCE = join(PATH_SKIA_SOURCE, 'third_party/externals')
const PATH_DOCKER_ACTION = join(__dirname, '../centos7_build_action')

const isExternalExit = fs.existsSync(PATH_SKIA_EXTERNAL_3RD_PARTY_SOURCE)

if (isExternalExit) {
  execSync(`tar --exclude='.git' -czvf ${PATH_DOCKER_ACTION}/externals.tar.gz externals`, {
    stdio: 'inherit',
    cwd: join(PATH_SKIA_SOURCE, 'third_party')
  })
} else {
  logger.error('Local skia third_party/externals is not exist!')
}
