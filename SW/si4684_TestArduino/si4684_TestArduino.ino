#include <SPI.h>
#include "firmware.h"
#include "Si468xROM.h"

struct DABFrequencyLabel_DAB {
  uint32_t frequency;
  const char* label;
};

const DABFrequencyLabel_DAB DABfrequencyTable_DAB[] = {
  { 174928,  "5A"}, { 176640,  "5B"}, { 178352,  "5C"}, { 180064,  "5D"},
  { 181936,  "6A"}, { 183648,  "6B"}, { 185360,  "6C"}, { 187072,  "6D"},
  { 188928,  "7A"}, { 190640,  "7B"}, { 192352,  "7C"}, { 194064,  "7D"},
  { 195936,  "8A"}, { 197648,  "8B"}, { 199360,  "8C"}, { 201072,  "8D"},
  { 202928,  "9A"}, { 204640,  "9B"}, { 206352,  "9C"}, { 208064,  "9D"},
  { 209936, "10A"}, { 211648, "10B"}, { 213360, "10C"}, { 215072, "10D"},
  { 216928, "11A"}, { 218640, "11B"}, { 220352, "11C"}, { 222064, "11D"},
  { 223936, "12A"}, { 225648, "12B"}, { 227360, "12C"}, { 229072, "12D"},
  { 230784, "13A"}, { 232496, "13B"}, { 234208, "13C"}, { 235776, "13D"},
  { 237488, "13E"}, { 239200, "13F"}
};

unsigned char SPIbuffer[4096];
uint8_t slaveSelectPin = 3;
uint8_t RESET_PIN = 1;
uint8_t SCK_DAB = 7; 
uint8_t MISO_DAB = 8;
uint8_t MOSI_DAB = 9;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("SETUP STARTING ....");

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  delay(500);
  digitalWrite(RESET_PIN, HIGH);

  /* leds on
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    pinMode(5, OUTPUT);
    digitalWrite(5, HIGH);*/

  Serial.println("Configure SPI");
  pinMode(slaveSelectPin, OUTPUT);                                        // Configure SPI
  digitalWrite(slaveSelectPin, HIGH);
  
  //SPI.begin(14, 16, 13, slaveSelectPin);
  SPI.begin(SCK_DAB, MISO_DAB, MOSI_DAB, slaveSelectPin);

  delay(3);
  SPIbuffer[0] = 0x09;
  SPIbuffer[1] = 0x00;
  SPIwrite(SPIbuffer, 2);
  cts();
  SPIread(6);

  bool result;
  if (SPIbuffer[1] != 0x80) {
    result = false;
  } else {
    result = true;
  }

  if (SPIbuffer[1] != 2) {
    Serial.println("POWER_UP");

    SPIbuffer[0] = 0x01;                                                    // POWER_UP
    SPIbuffer[1] = 0x00;
    SPIbuffer[2] = 0x17;
    SPIbuffer[3] = 0x48;
    SPIbuffer[4] = 0x00;
    SPIbuffer[5] = 0xf8;
    SPIbuffer[6] = 0x24;
    SPIbuffer[7] = 0x01;
    SPIbuffer[8] = 0x1F;
    SPIbuffer[9] = 0x10;
    SPIbuffer[10] = 0x00;
    SPIbuffer[11] = 0x00;
    SPIbuffer[12] = 0x00;
    SPIbuffer[13] = 0x18;
    SPIbuffer[14] = 0x00;
    SPIbuffer[15] = 0x00;
    SPIwrite(SPIbuffer, 16);

    /*
    SPIbuffer[0] = 0x80; // ARG1
    SPIbuffer[1] = 0x00; // ARG2 CLK_MODE=0x1 TR_SIZE=0x7
    SPIbuffer[2] = 0x48; // ARG3 IBIAS=0x48
    SPIbuffer[3] = 0x00; // ARG4 XTAL
    SPIbuffer[4] = 0xF9; // ARG5 XTAL // F8
    SPIbuffer[5] = 0x24; // ARG6 XTAL
    SPIbuffer[6] = 0x01; // ARG7 XTAL 19.2MHz
    SPIbuffer[7] = 0x1F; // ARG8 CTUN
    SPIbuffer[8] = 0x10; // ARG9
    SPIbuffer[9] = 0x00; // ARG10
    SPIbuffer[10] = 0x00; // ARG11
    SPIbuffer[11] = 0x00; // ARG12
    SPIbuffer[12] = 0x00; // ARG13 IBIAS_RUN
    SPIbuffer[13] = 0x00; // ARG14
    SPIbuffer[14] = 0x00; // ARG15
    SPIwrite(SPIbuffer, 15);
    */
    cts();

    delayMicroseconds(20);
    Serial.println("LOAD_INIT");
    SPIbuffer[0] = 0x06;                                                    // LOAD_INIT
    SPIbuffer[1] = 0x00;
    SPIwrite(SPIbuffer, 2);
    cts();

    uint32_t index = 0;
    Serial.print("Write bootloader Size:");                                     // Write bootloader
    Serial.println(sizeof(rom_patch_016));
    for (uint16_t i = 0; index < sizeof(rom_patch_016); i++) {
      //Serial.print("Index:");
      //Serial.println(index);
      SPIbuffer[0] = 0x04;
      SPIbuffer[1] = 0x00;
      SPIbuffer[2] = 0x00;
      SPIbuffer[3] = 0x00;
      for (uint16_t j = 4; j < 128 && index < sizeof(rom_patch_016); j++, index++) SPIbuffer[j] = pgm_read_byte_near(rom_patch_016 + index);
      SPIwrite(SPIbuffer, 128);
      cts();
    }

    Serial.println("LOAD_INIT");
    delay(4);
    SPIbuffer[0] = 0x06;                                                    // LOAD_INIT
    SPIbuffer[1] = 0x00;
    SPIwrite(SPIbuffer, 2);
    cts();

    index = 0;
    uint16_t i = 0;
    //Serial.print("Firmware:");
    //Serial.println(sizeof(firmware));
    while (index < sizeof(firmware)) {
      //Serial.print("Index:");
      //Serial.println(index);
      SPIbuffer[0] = 0x04;
      SPIbuffer[1] = 0x00;
      SPIbuffer[2] = 0x00;
      SPIbuffer[3] = 0x00;
      for (i = 4; (i < 2048) && (index < sizeof(firmware)); i++) {
        SPIbuffer[i] = pgm_read_byte_near(firmware + index);
        index++;
      }
      SPIwrite(SPIbuffer, i);
      cts();
    }

    Serial.println("BOOT");
    SPIbuffer[0] = 0x07;                                                    // BOOT
    SPIbuffer[1] = 0x00;
    SPIwrite(SPIbuffer, 2);
    cts();

    Serial.println("Write DAB frequencyplan");
    SPIbuffer[0] = 0xB8;                                                    // Write DAB frequencyplan
    SPIbuffer[1] = 0x26;
    SPIbuffer[2] = 0x00;
    SPIbuffer[3] = 0x00;
    for (i = 0; i < sizeof(DABfrequencyTable_DAB) / sizeof(DABFrequencyLabel_DAB); i++) {
      SPIbuffer[4 + (i * 4)] = DABfrequencyTable_DAB[i].frequency & 0xFF;
      SPIbuffer[5 + (i * 4)] = (DABfrequencyTable_DAB[i].frequency >> 8) & 0xFF;
      SPIbuffer[6 + (i * 4)] = (DABfrequencyTable_DAB[i].frequency >> 16) & 0xFF;
      SPIbuffer[7 + (i * 4)] = (DABfrequencyTable_DAB[i].frequency >> 24) & 0xFF;
    }
    SPIwrite(SPIbuffer, 4 + (38 * 4));
    cts();

    Serial.println("Set properties");
    Set_Property(0x0200, 0x8000);                                            // Set properties
    Set_Property(0x0202, 0x1600);
    Set_Property(0x0800, 0x0003);
    Set_Property(0x1710, 0xF7A0);
    Set_Property(0x1711, 0x01E0);
    Set_Property(0x8100, 0x0001);
    Set_Property(0x8101, 0x0064);
    Set_Property(0xB200, 0x0000);
    Set_Property(0xB201, 0x0080);
    Set_Property(0xB301, 0x0000);
    Set_Property(0xB302, 0x0000);
    Set_Property(0xB303, 0x0000);
    Set_Property(0xB400, 0x0097);
    Set_Property(0xB401, 0x0002);
    Set_Property(0xB500, 0x0000);
    Set_Property(0x8001, 0x0003); //enable I²S
  }

  // ChipID
  delay(500);
  char ChipType[7];
  SPIbuffer[0] = 0x08;
  SPIbuffer[1] = 0x00;
  SPIwrite(SPIbuffer, 2);
  cts();
  SPIread(23);
  itoa((SPIbuffer[10] << 8) + SPIbuffer[9], ChipType + 2, 10);
  ChipType[0] = 'S';
  ChipType[1] = 'I';
  ChipType[6] = '\0';
  Serial.print("ChipID:");
  Serial.println(ChipType);


  // FirmwareVersion
  delay(500);
  char FirmwVersion[6];
  SPIbuffer[0] = 0x12;
  SPIbuffer[1] = 0x00;
  SPIwrite(SPIbuffer, 2);
  cts();
  SPIread(12);
  char buffer[5];
  itoa(SPIbuffer[5], buffer, 10);
  FirmwVersion[0] = buffer[0];
  FirmwVersion[1] = '.';
  itoa(SPIbuffer[6], buffer, 10);
  FirmwVersion[2] = buffer[0];
  FirmwVersion[3] = '.';
  itoa(SPIbuffer[7], buffer, 10);
  FirmwVersion[4] = buffer[0];
  FirmwVersion[5] = '\0';
  Serial.print("FirmwareVersion:");
  Serial.println(FirmwVersion);


  delay(500);
  SPIbuffer[0] = 0x09;
  SPIbuffer[1] = 0x00;
  SPIwrite(SPIbuffer, 2);
  cts();
  SPIread(10);

  for (int i = 0; i < 10; i++) {
    Serial.printf("SPIbuffer[%d]:", i);
    Serial.println(SPIbuffer[i], HEX);
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}

static void SPIwrite(unsigned char *data, uint32_t length) {
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  digitalWrite (slaveSelectPin, LOW);
  SPI.transfer(data, length);
  digitalWrite (slaveSelectPin, HIGH);
  SPI.endTransaction();
}

static void SPIread(uint16_t length) {
  for (uint16_t i = 0; i < length + 1; i++) SPIbuffer[i] = 0;
  SPIwrite(SPIbuffer, length + 1);
}

static void cts(void) {
  bool timeout = false;
  uint16_t countdown = 1000;
  while (!(SPIbuffer[1] & 0x80)) {
    delay(4);
    for (byte i = 0; i < 5; i++) SPIbuffer[i] = 0;
    SPIwrite(SPIbuffer, 5);
    countdown--;
    if (countdown == 0) {
      timeout = true;
      break;
    }
  }

  if (SPIbuffer[1] & 0x40) {
    for (byte i = 0; i < 5; i++) SPIbuffer[i] = 0;
    if (timeout) SPIbuffer[5] |= (1 << 7);
  }
}

static void Set_Property(uint16_t property, uint16_t value) {
  SPIbuffer[0] = 0x13;
  SPIbuffer[1] = 0x00;
  SPIbuffer[2] = property & 0xFF;
  SPIbuffer[3] = (property >> 8) & 0xFF;
  SPIbuffer[4] = value & 0xFF;
  SPIbuffer[5] = (value >> 8) & 0xFF;
  SPIwrite(SPIbuffer, 6);
  cts();
}