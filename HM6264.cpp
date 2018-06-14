

#include "Arduino.h"
#include "HM6264.h"


void HM6264::init(uint8_t address_pin_array[13], uint8_t data_pin_array[8], uint8_t OE, uint8_t WE) {
  for (uint8_t i = 0; i < 13; i++) {
    address_pins[i] = address_pin_array[i];
  }

  for (uint8_t i = 0; i < 8; i++) {
    data_pins[i] = data_pin_array[i];
  }
  OE_pin = OE;
  WE_pin = WE;

  pinMode(OE_pin, OUTPUT);
  pinMode(WE_pin, OUTPUT);
}


void HM6264::write(int address, byte data) {

  bool data_bit;

  release_upload_pins();  //ensure pins released as we change states
  disable_chip_write();   //move to disable state
  set_address_pins();     //addr pins to output

  //write address
  for (int k = 0; k < 13; k++) // write output pins
  {
    data_bit = (address >> k) & 1;
    digitalWrite(address_pins[k], data_bit);
  }

  enable_chip_write();    //move to write state
  set_data_pins();        //set data pins to outputs

  //write data
  for (int k = 0; k < 8; k++) // write output pins
  {
    data_bit = (data >> k) & 1;
    digitalWrite(data_pins[k], data_bit);
  }

  delayMicroseconds(1);
  disable_chip_write();   //move to disable output state
  release_upload_pins();  //set pins as inputs
}

byte HM6264::read(int address) {

  bool data_bit;
  byte read_data = 0;

  release_upload_pins();  //ensure pins released as we change states
  disable_chip_read();   //move to disable state
  set_address_pins();     //addr pins to output

  //write address
  for (int k = 0; k < 13; k++) // write output pins
  {
    data_bit = (address >> k) & 1;
    digitalWrite(address_pins[k], data_bit);
  }

  enable_chip_read();    //move to write state
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++) {
    read_data = read_data + (digitalRead(data_pins[i]) << i);
  }
  disable_chip_read();   //move to disable output state
  release_address_pins();  //set pins as inputs

  return read_data;
}

byte HM6264::read(int address, byte (*callback_func)()) {

  bool data_bit;
  byte read_data = 0;

  release_upload_pins();  //ensure pins released as we change states
  disable_chip_read();   //move to disable state
  set_address_pins();     //addr pins to output

  //write address
  for (int k = 0; k < 13; k++) // write output pins
  {
    data_bit = (address >> k) & 1;
    digitalWrite(address_pins[k], data_bit);
  }

  enable_chip_read();    //move to write state
  delayMicroseconds(1);
  read_data = callback_func();    //use if cannot read back throguh same pins (eg if several chips chained together for writing)

  disable_chip_read();   //move to disable output state
  release_address_pins();  //set pins as inputs

  return read_data;
}



void HM6264::set_upload_pins() {
  set_data_pins();
  set_address_pins();
}

void HM6264::release_upload_pins() {
  byte k = 0;
  release_address_pins();
  for (k = 0; k < 8; k++)
    pinMode(data_pins[k], INPUT);

}

void HM6264::set_data_pins() {
  byte k = 0;
  for (k = 0; k < 8; k++) {
    pinMode(data_pins[k], OUTPUT);
    digitalWrite(data_pins[k], LOW);
  }
}

void HM6264::set_address_pins() {
  byte k = 0;
  for (k = 0; k < 13; k++) {
    pinMode(address_pins[k], OUTPUT);
    digitalWrite(address_pins[k], LOW);
  }
}

void HM6264::release_address_pins() {
  byte k = 0;
  for (k = 0; k < 13; k++)
    pinMode(address_pins[k], INPUT);
}


void HM6264::enable_chip_write() {
  digitalWrite(OE_pin, !OE_PIN_READ); //move to output disable if in read mode
  digitalWrite(WE_pin, WE_PIN_WRITE); //move to write mode (write cycle 1)
}

void HM6264::disable_chip_write() {
  digitalWrite(OE_pin, !OE_PIN_READ); //move to output disable if in read mode, write cycle 1 if in write cycle 2
  digitalWrite(WE_pin, !WE_PIN_WRITE); //move to disable output mode
}

void HM6264::enable_chip_read() {
  digitalWrite(WE_pin, !WE_PIN_WRITE); //move to disable output or read mode
  digitalWrite(OE_pin, OE_PIN_READ);  //move to read mode
}

void HM6264::disable_chip_read() {
  digitalWrite(WE_pin, !WE_PIN_WRITE); //move to disable output mode or read
  digitalWrite(OE_pin, !OE_PIN_READ);  //move to disable output mode
}

