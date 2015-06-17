
# decode a value from another metric unit into the base metric unit
exports.decodeValue = (value) ->
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

exports.onMiddleware = (req, res) ->

  res.send
    name: req.body.name

    value: exports.decodeValue req.body.data.x
