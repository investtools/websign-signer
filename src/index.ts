import { init } from 'nbind'
import { join } from 'path'

import * as LibTypes from './lib-types'

const binding = init<typeof LibTypes>(join(__dirname, '..'))

export default binding.lib.Signer
