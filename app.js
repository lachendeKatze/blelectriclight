/**
* app.js is the file connecting elements and actions
* 11/5/2016
*/



function sendCommand(value)
{
   switch(value)
   {
      case(0):
         blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([0]));
         break;
      case(1):
         break;
      case(2):
         break;
      case(3):  
         break;   
   }
};
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

function rainbow()
{
    blelectriclight._writeCharacteristic(blelectriclight.switchUUID, new Uint8Array([3]));
}

/**
hold off here unitl I can figure out how to put this on the UI nicely
function brightnessSelect()
{
            var x = document.getElementById("brightness").value;
            blelectriclight._writeCharacteristic(blelectriclight.brightnessUUID, new Uint8Array([x]));
            console.log( 'brightness: ' + x);
}
**/
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


function bleConnect(buttonID) {
  var thisButton = document.getElementById(buttonID);
  // thisButton.setAttribute("fill-opacity",0.9);
  blelectriclight.connect()
      .then(() => console.log('connected'))
      .catch(error => { console.log('connect error!'); });

};
