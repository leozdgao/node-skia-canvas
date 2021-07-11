class CanvasRenderingContext2D {
    static getFromCanvas(nativeCanvas) {
        const nativeContext = nativeCanvas.getContext('2d')
        return new CanvasRenderingContext2D(nativeContext);
    }

    constructor(nativeContext) {
        this.nativeContext = nativeContext;
    }

    createImageData(width, height) {
        if (typeof width === 'object') {
            width = width.width;
            height = width.height;
        }

        if (width <= 0) {
            throw new Error('width for createImageData should greater then zero');
        }

        if (height <= 0) {
            throw new Error('height for createImageData should greater then zero');
        }

        return this.nativeContext.createImageData(width, height);
    }

    getImageData(sx, sy, sw, sh) {
        const data = this.nativeContext.getImageData(sx, sy, sw, sh);
        
    }
}

module.exports = CanvasRenderingContext2D;
