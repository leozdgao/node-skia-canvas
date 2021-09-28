const chalk = require('chalk')

module.exports = {
  info (...args) {
    console.log(chalk.bgGreen.black('INFO'), chalk.green(...args))
  },
  warn (...args) {
    console.log(chalk.bgYellow.black('WARN'), chalk.yellow(...args))
  },
  error (...args) {
    console.log(chalk.bgRed.black('ERROR'), chalk.red(...args))
  }
}
