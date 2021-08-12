const { CanvasGradient } = require('./build/Debug/node-skia.node');

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

    createLinearGradient(x0, y0, x1, y1) {
        return new CanvasGradient('linear', x0, y0, x1, y1);
    }

    createRadialGradient(x0, y0, r0, x1, y1, r1) {
        return new CanvasGradient('radial', x0, y0, x1, y1, r0, r1);
    }

    ellipse(x, y, radiusX, radiusY, rotation, startAngle, endAngle, counterclockwise = false) {
        const tau = 2 * Math.PI;
        let newStartAngle = startAngle % tau;

        if (newStartAngle < 0) {
            newStartAngle += tau;
        }

        const delta = newStartAngle - startAngle;
        let newEndAngle = endAngle + delta;

        if (!counterclockwise && (newEndAngle - newStartAngle) >= tau) {
            newEndAngle = newStartAngle + tau;
        } else if (counterclockwise && (newStartAngle - newEndAngle) >= tau) {
            newEndAngle = newStartAngle - tau;
        } else if (!counterclockwise && newStartAngle > newEndAngle) {
            newEndAngle = newStartAngle + (tau - (newStartAngle - newEndAngle) % tau);
        } else if (counterclockwise && newStartAngle < newEndAngle) {
            newEndAngle = newStartAngle - (tau - (newEndAngle - newStartAngle) % tau);
        }

        return this.nativeContext.ellipse(x, y, radiusX, radiusY, rotation, newStartAngle, newEndAngle, counterclockwise);
    }

    getTransform() {
        
    }

    setLineDash(pattern) {
        if (!Array.isArray(pattern)) {
            return;
        }

        if (pattern.some(v => typeof v !== 'number')) {
            return;
        }

        if (pattern.length % 2 === 1) {
            pattern = [
                ...pattern,
                ...pattern,
            ];
        }

        return this.nativeContext.setLineDash(pattern);
    }

    rotate(angle) {
        return this.nativeContext.rotate(angle / Math.PI * 180);
    }
}

module.exports = CanvasRenderingContext2D;
