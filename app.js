/**
* app.js is the file connecting elements and actions
* 11/5/2016
*/


// This function is used to turn light on/off and select special effects
function sendCommand(value)
{
   switch(value)
   {
      case(0):
         blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([0]));
         break;
      case(1):
         blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([1]));
         break;
      case(2):
         blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([2]));
         break;
      case(3):  
         blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([3]));
         break;   
   }
};

function brightnessSelect()
{
            var x = document.getElementById("brightness").value;
            blelectriclight._writeCharacteristic(blelectriclight.brightnessUUID, new Uint8Array([x]));
            console.log( 'brightness: ' + x);
}

// This code gets the color values from the point cliked on the color wheel. See voronoi project code for credits
var canvas = document.getElementById("picker");
var ctx = canvas.getContext("2d");
var img = new Image();
img.src = "./images/colorwheel.png";
img.width = 400;
img.height = 400;
img.onload = function(){ ctx.drawImage(img,0,0,400,400);}
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

// a user action is required for security reasons, here it is. . .
function bleConnect(buttonID) {
  var thisButton = document.getElementById(buttonID);
  // thisButton.setAttribute("fill-opacity",0.9);
  blelectriclight.connect()
      .then(() => console.log('connected'))
      .catch(error => { console.log('connect error!'); });

};
