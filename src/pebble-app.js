Pebble.addEventListener("showConfiguration", function(e) {
  //Pebble.openURL("http://castalbums.org/pebble/stripes_digital_1.0.php?");
  Pebble.openURL("http://castalbums.org/pebble/stripes_digital_1.0.html?");
});

Pebble.addEventListener("webviewclosed", function(e){
  var values;
  console.log("Configuration closed");
  console.log("Response = " + e.response.length + "   " + e.response);
  if (e.response !== undefined && e.response !== '' && e.response !== 'CANCELLED') {
    console.log("User hit save");
    values = JSON.parse(decodeURIComponent(e.response));
    console.log("stringified options: " + JSON.stringify((values)));
    
    //var json = JSON.parse(e.response);

		Pebble.sendAppMessage(values,
      function(e) {
        console.log("Config sent to Pebble successfully!");
      },
      function(e) {
        console.log("Error sending config data to Pebble!");
      }
      );
  }
});

//start to listen for the watchface to be opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");
  }
);

//listen for when a message is received from the app
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    console.log('Received appmessage: ' + JSON.stringify(e.payload));
  }
);
