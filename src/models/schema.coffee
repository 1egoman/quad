###
 * quad
 * https://github.com/1egoman/quad
 *
 * Copyright (c) 2015 Ryan Gaus
 * Licensed under the MIT license.
###

mongoose = require 'mongoose'

schema = mongoose.Schema
  name: String

module.exports = mongoose.model 'Schema', schema
