
#ifndef HM6264_H
#define HM6264_H

#include "Arduino.h"

#define OE_PIN_READ LOW
#define WE_PIN_WRITE LOW


class HM6264 {
  public:
    HM6264() {}
    HM6264(uint8_t address_pin_array[13], uint8_t data_pin_array[8], uint8_t OE, uint8_t WE)
    {
      init(address_pin_array, data_pin_array, OE, WE);
    }
    

    void write(int address, byte data);
    byte read(int address);
    byte read(int address, byte (*callback_func)());
    void init(uint8_t address_pin_array[13], uint8_t data_pin_array[8], uint8_t OE, uint8_t WE);

  private:

    byte OE_pin = 0;
    byte WE_pin = 0;
    byte data_pins [8] = {0};
    byte address_pins[13] = {0};

    void set_upload_pins();
    void release_upload_pins();
    void enable_chip_write();
    void disable_chip_write();
    void disable_chip_read();
    void enable_chip_read();
    void set_data_pins();
    void set_address_pins();
    void release_address_pins();

};

#endif HM6264_H
