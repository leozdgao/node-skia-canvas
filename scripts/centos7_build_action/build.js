
// RUN git clone --depth=1 https://github.com/leozdgao/node-skia-canvas.git 

// WORKDIR /workspace/node-skia-canvas
// RUN git submodule init && git submodule update

const path = require('path')
const { execSync } = require('child_process')

const exec = (cmd, opts) => {
  const { cwd = process.cwd(), stdio = 'inherit' } = opts || {}
  return execSync(cmd, {
    stdio,
    cwd
  })
}

const PREBUILD_UPLOAD_SECRET = process.argv[2] || process.env.PREBUILD_UPLOAD_SECRET || '';

exec(`echo upload=${PREBUILD_UPLOAD_SECRET} >> ~/.prebuildrc`)

exec('OMIT_SYNC_DEPS=true npm run ci && npm test', {
    cwd: '/work/node-skia-canvas'
})
