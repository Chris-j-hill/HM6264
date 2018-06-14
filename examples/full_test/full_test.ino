
#include <HM6264.h>

#define MAX_ADDR (2^13) -1
//pin assignments
byte address_pins[13] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
byte data_pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
byte OE = 11;
byte WE = 12;


int address = 0;
byte write_byte = 1;
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

  if (write_byte != read_byte) {      //check for error
    Serial.print("Error, address:");
    Serial.println(address);
    Serial.print("Value written:");
    Serial.println(write_byte);
    Serial.print("Value read:");
    Serial.println(read_byte);
  }

  if (address = MAX_ADDR) {     //stop once all addresses checked
    Serial.println("Done");
    while (1) {}
  }

  write_byte = write_byte << 1; //check each bit writes in byte, not all addresses
  
  if (write_byte == 0) {  //once shifted 8 times, increment address
    address++;
    write_byte = 1;
  }
}




