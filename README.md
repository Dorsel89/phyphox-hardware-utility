# phyphox-hardware-utility

## Example DS18B20
```
   Onewire myOneWire(P0_4);
   DS18B20 myDS18B20(&myOneWire);

   myDS18B20.init();
   myDS18B20.startTempConversion();
   ThisThread::sleep_for(1s);
   float currentTemperature =  myDS18B20.getTemp();
```
