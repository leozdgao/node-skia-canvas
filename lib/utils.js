const noop = () => {}

exports.createProxyToNative = (tgt, getTarget = noop) => {
  return new Proxy(tgt, {
    construct (target, args) {
      const Ctor = target
      const obj = new Ctor(...args)

      return new Proxy(obj, {
        get (target, prop, receiver) {
          if (target[prop] != null) {
            return target[prop]
          }

          const t = getTarget(target)
          if (t) {
            const func = t[prop]

            if (typeof func === 'function') {
              return func.bind(t)
            }

            return func
          }
        },
        set (target, prop, value) {
          const descriptor = Object.getOwnPropertyDescriptor(
            Reflect.getPrototypeOf(target),
            prop
          )

          if (descriptor && typeof descriptor.set === 'function') {
            target[prop] = value
          } else {
            const t = getTarget(target)
            if (t) {
              t[prop] = value
            }
          }
        }
      })
    }
  })
}

exports.colorVectorToHex = (vector) => {
  if (!Array.isArray(vector)) {
    return null
  }

  const r = Math.round(vector[0] * 255)
  const g = Math.round(vector[1] * 255)
  const b = Math.round(vector[2] * 255)
  const a = Math.round(vector[3] * 100) / 100

  // no opacity, convert to #RRGGBB
  if (a === 1) {
    return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`
  }
  // or convert to rgba(r, g, b, a)
  return `rgba(${r}, ${g}, ${b}, ${a})`
}
