
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

exec('git clone --depth=1 https://github.com/leozdgao/node-skia-canvas.git')

const workDirectory = path.join(process.cwd(), 'node-skia-canvas')

exec('git submodule init && git submodule update', {
    cwd: workDirectory
})
exec('npm install', {
  cwd: workDirectory
})
exec('npm run ci && npm test', {
    cwd: workDirectory
})
