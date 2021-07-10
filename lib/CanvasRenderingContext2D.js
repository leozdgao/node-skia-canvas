class CanvasRenderingContext2D {
    static getFromCanvas(nativeCanvas) {
        const nativeContext = nativeCanvas.getContext('2d')
        return new CanvasRenderingContext2D(nativeContext);
    }

    constructor(nativeContext) {
        this.nativeContext = nativeContext;
    }

    createImageData(width, height) {
        return {
            width, height, /* data, */
        }
    }

    getImageData(sx, sy, sw, sh) {
        const data = this.nativeContext.getImageData(sx, sy, sw, sh);
        
    }
}

module.exports = CanvasRenderingContext2D;
