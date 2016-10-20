var five = require("johnny-five");
var board = new five.Board({
	port: 'COM5'
});

board.on("ready", function() {
  var anode = new five.Led.RGB({
	pins: {
	  red: 5,
	  green: 6,
	  blue: 3
	},
	isAnode: true
  });

  // Add led to REPL (optional)
  this.repl.inject({
	anode: anode
  });

  // Turn it on and set the initial color
  anode.on();
  anode.color("#FF0000");

  anode.blink(1000);

});