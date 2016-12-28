#include <CurieBLE.h>
#include <Adafruit_NeoPixel.h>

/**
* This project demonstrates how to control an adafruit industries NeoPixel Ring over BLE using an arduino/genuino101.
* This projct is derived from the Cproject in Chapter 7 of Make: Bluetooth: Bluetooth LE Projects with Arduino, Raspberry Pi, 
* and Smartphones and the project by the Ruiz brothers from Adafruit entitled,(http://www.thingiverse.com/thing:1343925) 
*/

/**
* Setup our BLE service and characteristics here
*/
BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService lampService("917649A0-D98E-11E5-9EEC-0002A5D5C51B"); // Custom UUID
BLEUnsignedCharCharacteristic switchCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLEWrite);
BLEUnsignedCharCharacteristic brightnessCharacteristic("917649A2-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLEWrite);
BLECharacteristic colorCharacteristic("917649A3-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLEWrite,3);
BLEDescriptor lampDescriptor("2902", "lamp");

/**
 * establish which pin the NeoPixel ring is attachd to on the Arduino/Genuino101, and the
 * number of NeoPixels on the ring.
 * 
*/
#define PIN            7
#define NUMPIXELS      12
/* 
 *  When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
 *  Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
 *  example for more information on possible values.
*/

Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);
// Adafruit_NeoPixel pixels;
int delayTime = 150; // delay used for NeoPixel light effects.

// Lamp state structure

struct LampStateStructure
{
  boolean lampOn;
  int brightness;
  int green;
  int red;
  int blue;
};

struct LampStateStructure lampState;

void setup() {

  /** Serial 
  * Serial communication is not necessary for this project to work, but
  * is very useful for debugging. All serial references could be 
  * commented out if desried.
  */
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
 
  Serial.println("Arduino101/IntelCurie/NeoPixel-Voroni-Lamp/Evothings Example Started");
  Serial.println("Serial rate set to 9600");

  // initilize the ble service, characterisitcs and advertising
  blePeripheral.setLocalName("voronoi");
  blePeripheral.setAdvertisedServiceUuid(lampService.uuid());
  blePeripheral.addAttribute(lampService);
  blePeripheral.addAttribute(switchCharacteristic);
  blePeripheral.addAttribute(brightnessCharacteristic);
  blePeripheral.addAttribute(colorCharacteristic);
  blePeripheral.addAttribute(lampDescriptor);

  // add event handlers here
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  brightnessCharacteristic.setEventHandler(BLEWritten, brightnessCharacteristicWritten);
  colorCharacteristic.setEventHandler(BLEWritten, colorCharacteristicWritten);

  // set an initial values for the characteristics
  switchCharacteristic.setValue(-1);
  brightnessCharacteristic.setValue(0);
  
  /** 
   * the color characterisitc consists of the 3 GREEN,RED and BLUE values sent
   * from the mobile device as a single characterisitic of 3 bytes, each a 
   * value from 0..255
  */
  
  const unsigned char initialColor[3] = {0,0,0};
  colorCharacteristic.setValue(initialColor,3);
  
  // advertise the service
  blePeripheral.begin();
  Serial.println(("Bluetooth device active, waiting for connections..."));

  // setup the lamp state variable strucutre and set to intial values
  
  lampState.lampOn = false;
  lampState.brightness = 200;
  lampState.green = 250;
  lampState.red = 250;
  lampState.blue = 250;


  // This initializes the NeoPixel, read the code in the NeoPixellibrary to understand what it does.
  ring.begin(); 
}

void loop() {


  // continual poll the ble device for activity, connection, disconnect, characterisitc cahgnes, etc.
  blePeripheral.poll();
  
} // end loop

/**
 * BLE Event Handlers are defined in the fucntions below
 */
 
void blePeripheralConnectHandler(BLECentral& central) 
{
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) 
{
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{
  /**
   * We will use a switch/case statement here so that we can expand the functionality of the app 
   * in the future; This will allow us to accept values up to 255 on the switch characteristic
   * and not permanently tie the characteritis to just turning the lamp on/off;
   */

   int switchValue = switchCharacteristic.value();
   Serial.print("Switch Value: "); Serial.println(switchValue);

   switch(switchValue)
   {
      case 0:
        // Turn lamp on/off
        lamp();
        break;
      case 1:
        // for debugging/expanded functionality later
        break;
      case 2:    
        // for debugging/expanded functionality later
        break;
      case 3:
        // for debugging/expanded functionality later
        break;  
   }   
}
void brightnessCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{

  lampState.brightness = brightnessCharacteristic.value();
  ring.setBrightness(lampState.brightness);
  ring.show();
  
}
void colorCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{
  // The colors on this neopixel are speciifed GRB not RGB!
  const unsigned char* colors = colorCharacteristic.value();
  lampState.green = colors[0];
  lampState.red = colors[1];
  lampState.blue = colors[2];
  for (int i=0;i<NUMPIXELS;i++)
  {
    ring.setPixelColor( i, ring.Color(lampState.green,lampState.red,lampState.blue));
  }
  ring.show();
}

void lamp()
{
  if (lampState.lampOn)
  {
    /** do not set the lamp state variable lampState.brightness to 0 
     *  as the expectation of turning the lamp back on will be 
     *  for the lamp to turn back on to the same brightness
     *  as before turnng off. 
     *        Consider adding a dimming effect here.
     */
    ring.setBrightness(0);
    ring.show();
    lampState.lampOn = false;
  } 
  else
  {
    ring.setBrightness( lampState.brightness );
    for (int i=0;i<NUMPIXELS;i++)
    {
      ring.setPixelColor( i, ring.Color(lampState.green,lampState.red,lampState.blue));
    }
    ring.show();
    lampState.lampOn = true;
  }
}

