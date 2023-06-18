avr-objcopy.exe -j .text -j .data -O ihex %1 %1.hex
avr-objcopy -j .eeprom --set-section-flags .eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex %1 %1.eep
avr-size.exe -C --mcu=attiny13a %1
avrdude.exe -p t13 -c usbasp -P usb -U flash:w:%1.hex:i -U eeprom:w:%1.eep:i