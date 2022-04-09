# General Notes On Dependencies

Encryption requires several libraries & tweaks:

1 - RadioHead library
2 - Uncomment the "#define RH_ENABLE_ENCRYPTION_MODULE" in RadioHead.h source
3 - Encryption via the Speck.h implementation by rweather:  
  https://github.com/rweather/arduinolibs  
  Install the CryptoLW directory into your Arduino/libraries/ location  
  No, this isn't obvious and it's not part of the Arduino releases/libraries system