#include <HM6264.h>

//pin assignments
byte address_pins[13] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
byte data_pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
byte OE = 11;
byte WE = 12;


int address = 1;
byte write_byte = 0;
byte read_byte = 0;

HM6264 ROM1;

void setup() {

  ROM1.init(address_pins, data_pins, OE, WE, true);   //initialise pins
  Serial.begin(115200);
  Serial.println("Write \t Read");
}


void loop() {

  ROM1.write(address, write_byte);     //write data to address
  read_byte = ROM1.read(address);      //read data back

  Serial.print(write_byte);
  Serial.print("\t\t");
  Serial.println(read_byte);

  delay(500);
  write_byte++;
}


