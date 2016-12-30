// Function to send a message to the Pebble using AppMessage API
function sendMessage() {
  //testRequest();
	Pebble.sendAppMessage({"status": 0});
	
	// PRO TIP: If you are sending more than one message, or a complex set of messages, 
	// it is important that you setup an ackHandler and a nackHandler and call 
	// Pebble.sendAppMessage({ /* Message here */ }, ackHandler, nackHandler), which 
	// will designate the ackHandler and nackHandler that will be called upon the Pebble 
	// ack-ing or nack-ing the message you just sent. The specified nackHandler will 
	// also be called if your message send attempt times out.
}

function makeRequest(method, url, callback) {
	var req = new XMLHttpRequest();
	req.open(method, url, true);
	req.onload = function(e) {
		if(req.readyState == 4) { callback(req); }
	};
	req.send(null);
}


//function handler() {
//  if(this.readyState == this.DONE) {
//   if(this.status == 200 &&
//       this.responseXML != null &&
//       this.responseXML.getElementById('test').textContent) {
//      // success!
//      processData(this.responseXML.getElementById('test').textContent);
//      return;
//    }
//    // something went wrong
//    processData(null);
//  }
//}

function testRequest() {
  var req = new XMLHttpRequest();
  req.open('GET', 'http://api.openweathermap.org/data/2.1/find/city?lat=37.830310&lon=-122.270831&cnt=1', true);
  req.onload = function(e) {
  if (req.readyState == 4 && req.status == 200) {
    //readystate 4 is DONE
    //status 200 is OK
    if(req.status == 200) {
      var response = JSON.parse(req.responseText);
      var temperature = response.list[0].main.temp;
      var icon = response.list[0].main.icon;
      Pebble.sendAppMessage({ 'icon':icon, 'temperature':temperature + '\u00B0C', 'status': 0, 'message':'test completed'});
    } else { 
      console.log('Error'); 
    }
  }
req.send(null);
};
}

// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) {
							});
												
// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
							function(e) {
								console.log("Received Status: " + e.payload.status);
								sendMessage();
							});