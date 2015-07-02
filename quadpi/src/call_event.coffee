
# decode a value from another metric unit into the base metric unit
exports.decodeValue = (value) ->
  if typeof value is 'number' then return value

  pref = value.unit[0]
  unit = switch pref
    when 'k' then 3
    when 'h' then 2
    when 'D' then 1
    when 'd' then -1
    when 'c' then -2
    when 'm' then -3
    when 'u' then -6
    when 'n' then -9
    when 'p' then -12
    else 0
  quant = value.quantity or value.value
  quant * Math.pow 10, unit






###########################
# Quad controlling events #
###########################

do_movement_event = (res, data) ->
  res.send "yo!"

exports.onMiddleware = (req, res) ->

  switch req.body.name

    when "device.movement.latrot", "device.movement.lat", "device.movement.rot"
      do_movement_event res, req.body.data

    when "device.identify"
      do_device_identify res

    else
      res.send
        name: "error.no.such.event"

  # res.send
  #   name: req.body.name
  #
  #   value: exports.decodeValue req.body.data.x
