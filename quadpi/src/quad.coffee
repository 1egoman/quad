###
 * quad
 * https://github.com/1egoman/quad
 *
 * Copyright (c) 2015 Ryan Gaus
 * Licensed under the MIT license.
###

'use strict'

express = require "express"
app = express()
chalk = require "chalk"
path = require "path"
bodyParser = require "body-parser"
uuid = require "uuid"
event = require "./call_event"

exports.main = ->

  # set ejs as view engine
  app.set "view engine", "ejs"

  # include all the required middleware
  exports.middleware app

  # some sample routes
  router = express.Router()

  router.get "/", (req, res) ->
    res.send
      name: "device.hello"
      id: uuid.v4()
      data: {}

  router.post "/device/event", event.onMiddleware



  app.use router

  
  # listen for requests
  PORT = process.argv.port or 8000
  app.listen PORT, ->
    console.log chalk.blue "-> :#{PORT}"

exports.middleware = (app) ->
  
  # json body parser
  app.use bodyParser.json()
  
  # include sass middleware to auto-compile sass stylesheets
  node_sass = require "node-sass-middleware"
  app.use node_sass
    src: path.join(__dirname, "../public"),
    dest: path.join(__dirname, "../public"),
  

  # serve static assets
  app.use require("express-static") path.join(__dirname, '../public')

exports.main()
