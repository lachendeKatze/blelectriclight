#include <CurieBLE.h>
#include <CurieTimerOne.h>
#include <Adafruit_NeoPixel.h>

/**
* This project demonstrates how to control an adafruit industries NeoPixel Ring over BLE using an arduino/genuino101.
* This projct is derived from the project in Chapter 7 of Make: Bluetooth: Bluetooth LE Projects with Arduino, Raspberry Pi,
* and Smartphones and the project by the Ruiz brothers from Adafruit entitled,(http://www.thingiverse.com/thing:1343925)
* and is now modified yo include special effects.
*/

/**
* Setup our BLE service and characteristics here
*/
BLEPeripheral blePeripheral;       // BLE Peripheral Device (the board you're programming)
BLEService lampService("917649A0-D98E-11E5-9EEC-0002A5D5C51B"); // Custom UUID
BLEUnsignedCharCharacteristic switchCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLEWrite);
BLEUnsignedCharCharacteristic brightnessCharacteristic("917649A2-D98E-11E5-9EEC-0002A5D5C51B", BLEWrite);
BLECharacteristic colorCharacteristic("917649A3-D98E-11E5-9EEC-0002A5D5C51B", BLEWrite,3);
BLEDescriptor lampDescriptor("2902", "ble");
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

// light will represent the 12 neopixel ring
Adafruit_NeoPixel light = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);
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
  boolean flip;
  int currentPixel;
  uint16_t colorIncrement;
};

struct LampStateStructure lampState;

void setup() {

  /** Serial
  * Serial communication is not necessary for this project to work, but
  * is very useful for debugging. All serial references could be
  * commented out if desried.
  */
  // Serial.begin(9600); // initialize Serial communication
  // while (!Serial);    // wait for the serial port to open

  // Serial.println("Arduino101/IntelCurie/NeoPixel-Voroni-Lamp/Evothings Example Started");
  // Serial.println("Serial rate set to 9600");

  // initilize the ble service, characterisitcs and advertising
  blePeripheral.setLocalName("lamp");
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
  // Serial.println(("Bluetooth device active, waiting for connections..."));

  // setup the lamp state variable strucutre and set to intial values

  lampState.lampOn = false;
  lampState.brightness = 200;
  lampState.green = 250;
  lampState.red = 250;
  lampState.blue = 250;
  lampState.flip = false;
  lampState.currentPixel = 0;
  lampState.colorIncrement = 0;


  // This initializes the NeoPixel, read the code in the NeoPixellibrary to understand what it does.
  light.begin();
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
  // Serial.print("Connected event, central: ");
  // Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central)
{
  // central disconnected event handler
  // Serial.print("Disconnected event, central: ");
  // Serial.println(central.address());
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
        CurieTimerOne.stop();
        break;
      case 1:
        // for debugging/expanded functionality later
         CurieTimerOne.stop();
         CurieTimerOne.start(100000, &blinkISR);
        break;
      case 2:
        // for debugging/expanded functionality later
        CurieTimerOne.stop();
        CurieTimerOne.start(50000, &colorWipeISR);
        break;
      case 3:
        CurieTimerOne.stop();
        CurieTimerOne.start(10000, &rainbowISR);
        // for debugging/expanded functionality later
        break;
   }
}
void brightnessCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic)
{

  lampState.brightness = brightnessCharacteristic.value();
  light.setBrightness(lampState.brightness);
  light.show();

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
    light.setPixelColor( i, light.Color(lampState.green,lampState.red,lampState.blue));
  }
  light.show();
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
    light.setBrightness(0);
    light.show();
    lampState.lampOn = false;
  }
  else
  {
    light.setBrightness( lampState.brightness );
    for (int i=0;i<NUMPIXELS;i++)
    {
      light.setPixelColor( i, light.Color(lampState.green,lampState.red,lampState.blue));
    }
    light.show();
    lampState.lampOn = true;
  }
}
// ble electric light patterns & util functions
void blank()
{
 for (int i = 0; i < light.numPixels(); i++)
 {
  light.setPixelColor(i, light.Color(0,0,0));
 }
 light.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// taken from the strandtest example
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return light.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return light.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return light.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/*
* Each of these ISR changes the state of the each of neopxiels every time it is called by "one" by CurieTimerOne
* and then displays the entire ring. No delay() used, the unit as a whole remains reponsive to BLE characterisitic
* changes. The for loops only set the state of each neopixel.
*/

void rainbowISR()
{
 if ( lampState.colorIncrement == 256 ) { lampState.colorIncrement = 0; }
 else lampState.colorIncrement++;

 for (uint16_t i = 0; i<light.numPixels(); i++)
 {
  light.setPixelColor(i,Wheel((i+lampState.colorIncrement) & 255));
 }
 light.show();   
}

void blinkISR()
{
    lampState.flip = !lampState.flip;
  
     if (lampState.flip) 
     {
        for (int i = 0; i<light.numPixels(); i++)
        {
          light.setPixelColor(i, light.Color(lampState.green,lampState.red,lampState.blue));
        }
     }
    else 
    {
      blank();
    }
  
    light.show(); 
}

void colorWipeISR()
{
  light.setPixelColor( lampState.currentPixel, light.Color(lampState.green,lampState.red,lampState.blue));
  lampState.currentPixel++;
  if (lampState.currentPixel > light.numPixels()) { 
    lampState.currentPixel = 0; 
    blank();
  }
  light.show();
}




