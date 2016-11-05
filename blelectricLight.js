(function() {
  'use strict';

  class BLElectricLight{

    /**
      * The LED class is a object used to represent the BLE service. Note for this project
      * the read code is not used, but I kept it there for you to see.
      *
      * customize your project here to reflect the uuid of your service and characteristics.
      */
    constructor() {
        this.deviceName = 'lamp'; // BLElectricLight Device Name
        this.serviceUUID =  '917649a0-d98e-11e5-9eec-0002a5d5c51b'; // BLElectricLight service uuid
        this.switchUUID = '917649a1-d98e-11e5-9eec-0002a5d5c51b'; // On/Off switch uuid
        this.brightnessUUID = '917649a2-d98e-11e5-9eec-0002a5d5c51b'; // Brightness uuid
        this.colorUUID = '917649a3-d98e-11e5-9eec-0002a5d5c51b'; // color uuid, holds 3 values G(reen)R(ed)B(lue)
        // this.patternUUID = '917649A4-D98E-11E5-9EEC-0002A5D5C51B'; // pattern uuid
        this.device = null;
        this.server = null;
        // The cache allows us to hold on to characeristics for access in response to user commands
        this._characteristics = new Map();
    }

    connect(){

      return navigator.bluetooth.requestDevice({
        filters:[{
          services:[this.serviceUUID],
        }]
      }).then(device => device.gatt.connect())
      .then(server => server.getPrimaryService(this.serviceUUID))
      .then(service => {
        return Promise.all([
          this._cacheCharacteristic(service, this.colorUUID),
          this._cacheCharacteristic(service, this.brightnessUUID),
          this._cacheCharacteristic(service, this.switchUUID),
        ]);
      })
    }

  _cacheCharacteristic(service, characteristicUuid){
    return service.getCharacteristic(characteristicUuid)
    .then(characteristic => {
      this._characteristics.set(characteristicUuid, characteristic).then(console.log('cache: '  + characteristicUuid));
    });
  }

 _readCharacteristic(characteristicUuid) {
   let characteristic = this._characteristics.get(characteristicUuid);
   console.log("read this char: " + characteristicUuid);
   console.log("this is thechar: " + characteristic);
   return characteristic.readValue()
   .then(value => {console.log('value: ' + value);
     return value;
   });

 }
 _writeCharacteristic(characteristicUuid, value){
   let characteristic = this._characteristics.get(characteristicUuid);
   console.log("read this char: " + characteristicUuid);
   console.log("the write char: " + characteristic);
   return characteristic.writeValue(value);
 }
}


// makes the led object available to therest of the program, remeber to change this to reflect your project
window.blelectriclight = new BLElectricLight();

})();
