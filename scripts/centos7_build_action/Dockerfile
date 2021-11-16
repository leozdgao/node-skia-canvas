FROM leozdgao/centos7_node12_llvm

WORKDIR /workspace
COPY build.js build.js

RUN git clone --depth=1 https://github.com/leozdgao/node-skia-canvas.git

WORKDIR /workspace/node-skia-canvas
RUN git submodule init && git submodule update
RUN npm install

WORKDIR /workspace/node-skia-canvas/skia
RUN python2 tools/git-sync-deps

ENV OMIT_SYNC_DEPS=true

ENTRYPOINT [ "node", "/workspace/build.js" ]
