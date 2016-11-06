/**
* app.js is the file connecting elements and actions
* 11/5/2016
*/
function lightSwitch()
{
   blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([0]));
}

function blink()
{
   blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([1]));
}

function colorWipe()
{
    blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([2]));
}


function brightnessSelect()
{
            var x = document.getElementById("brightness").value;
            blelectriclight._writeCharacteristic(blelectriclight.brightnessUUID, new Uint8Array([x]));
            console.log( 'brightness: ' + x);
}

var canvas = document.getElementById("picker");
var ctx = canvas.getContext("2d");
var img = new Image();
img.src = "./images/color_wheel.png";
img.onload = function(){ ctx.drawImage(img,0,0);}
canvas.addEventListener("mousedown", getPosition, false);

function getPosition(evt)
{
   var x = evt.pageX - this.offsetLeft;
   var y = evt.pageY - this.offsetTop;
   
   var imgData = ctx.getImageData(x,y,1,1).data;
   var R = imgData[0];
   var G = imgData[1];
   var B = imgData[2];
   
   console.log(x + " , " + y +"(R,G,B): " + R + "," + G + "," + B);
   blelectriclight._writeCharacteristic(blelectriclight.colorUUID, new Uint8Array([G,R,B]));
}




// var img = new Image();
// img.src = 'blelectriclight/images/color_wheel_730.png';
        // img.src = 'lizard.png'
/** $(img).load(function(){ canvas.drawImage(img,100,5,295,295);});
$('#picker').click(function(event)
{
            var x = event.pageX - this.offsetLeft;
            var y = event.pageY - this.offsetTop;
            var imgData = canvas.getImageData(x,y,1,1).data;
            var R = imgData[0];
            var G = imgData[1];
            var B = imgData[2];
            blelectriclight._writeCharacteristic(blelectriclight.colorUUID, new Uint8Array([G,R,B]));
});


*/


/**
function lightIt(circleID)
{
  var circle = document.getElementById(circleID);
  var currentOpacity = circle.getAttribute("fill-opacity");
  console.log("current 0 opacity = " + currentOpacity);
  var curOp = parseFloat(currentOpacity);
  console.log("curOp: " + curOp);
 if ( currentOpacity < 0.8 ) {
  // currentOpacity = currentOpacity + 0.2;
   curOp = curOp + 0.2;
  circle.setAttribute("fill-opacity", curOp);
 }
 else {
  circle.setAttribute("fill-opacity", 0.2);
 }
 toggleLight(circleID);
};

function getBrightness()
{
    lightReading = exp3._readCharacteristic(exp3.cds);
    console.log('light: ' + lightReading);
}

/**
  * takes the chosen "circle" and writes the appropriate data
  * to the ledservice characteristics.

var toggleLight = function( circleID )
{
  if (circleID == "circle1" ) {
   exp3._writeCharacteristic(exp3.ledUUID, new Uint8Array([4]))
   .then(() => console.log('wrote 4'))
   .catch(error => {console.log('write error');
   });
  }
  else if (circleID == "circle2" ){
    exp3._writeCharacteristic(exp3.ledUUID, new Uint8Array([3]))
   .then(() => console.log('wrote 3'))
   .catch(error => {console.log('write error');
   });
  }
  else if (circleID == "circle3") {
    exp3._writeCharacteristic(exp3.ledUUID, new Uint8Array([2]))
   .then(() => console.log('wrote 2'))
   .catch(error => {console.log('write error');
   })
  }
  else{
    exp3._writeCharacteristic(exp3.ledUUID, new Uint8Array([1]))
   .then(() => console.log('wrote 1'))
   .catch(error => {console.log('write error');
   });
  }
};
*/
/**
  * The Web Bluetooth API specifies that a user action like a click is needed to
  * initiate a connection.
  */
function bleConnect(buttonID) {
  var thisButton = document.getElementById(buttonID);
  // thisButton.setAttribute("fill-opacity",0.9);
  blelectriclight.connect()
      .then(() => console.log('connected'))
      .catch(error => { console.log('connect error!'); });

};
