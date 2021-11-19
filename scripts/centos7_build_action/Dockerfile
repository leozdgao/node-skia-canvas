FROM leozdgao/centos7_node12_llvm_build_skia

WORKDIR /work
COPY build.js build.js

RUN git clone --depth=1 https://github.com/leozdgao/node-skia-canvas.git

WORKDIR /work/node-skia-canvas
RUN git submodule init && git submodule update
RUN npm install

WORKDIR /work/node-skia-canvas/skia
RUN tar -zxf /work/externals.tar.gz -C /work/node-skia-canvas/skia/third_party

ENV OMIT_SYNC_DEPS=true

ENTRYPOINT [ "node", "/work/build.js" ]
