#ifndef CONSTANTS_H
#define CONSTANTS_H

/*#define ROTARY_PIN_A    11//27
#define ROTARY_PIN_B    12//34
#define ROTARY_PIN_2A   13//33
#define ROTARY_PIN_2B   11//32
#define ROTARY_BUTTON   12//25
#define ROTARY_BUTTON2  13//35*/
#define STANDBYBUTTON   11//36
//#define SLBUTTON        12//26
//#define MODEBUTTON      21//39
#define CONTRASTPIN     2//2



// TCA9554 I2C address (default is 0x20, can be 0x20-0x27 depending on A0-A2 pins)
#define TCA9554_ADDRESS 0x20

// TCA9554 Register addresses
#define TCA9554_INPUT_REG     0x00  // Input port register
#define TCA9554_OUTPUT_REG    0x01  // Output port register  
#define TCA9554_POLARITY_REG  0x02  // Polarity inversion register
#define TCA9554_CONFIG_REG    0x03  // Configuration register

// Pin assignments on TCA9554 (P0-P7)
#define ENCODER_A_PIN    7  // P0 - Encoder A signal
#define ENCODER_B_PIN    5  // P1 - Encoder B signal
#define ENCODER_BTN_PIN  6  // P2 - Encoder button

#define ENCODER2_A_PIN    4  // P0 - Encoder A signal
#define ENCODER2_B_PIN    1  // P1 - Encoder B signal
#define ENCODER2_BTN_PIN  0  // P2 - Encoder button

#define ENCODER_BTN3_PIN  2  // P2 - Encoder button
#define ENCODER_BTN4_PIN  3  // P2 - Encoder button

#define INT_PIN          43  // Arduino interrupt pin connected to TCA9554 INT




#define ITEM_GAP        20
#define ITEM1           3
#define ITEM2           23
#define ITEM3           43
#define ITEM4           63
#define ITEM5           83
#define ITEM6           103
#define ITEM7           123
#define ITEM8           143
#define ITEM9           163
#define ITEM10          183

// EEPROM index defines
#define EE_PRESETS_CNT              99
#define EE_PRESETS_FREQUENCY        255
#define EE_CHECKBYTE_VALUE          2 // 0 ~ 255,add new entry, change for new value

#define EE_TOTAL_CNT                2614
#define EE_BYTE_CHECKBYTE           0
#define EE_BYTE_LANGUAGE            1
#define EE_BYTE_CONTRASTSET         2
#define EE_BYTE_DISPLAYFLIP         3
#define EE_BYTE_ROTARYMODE          4
#define EE_BYTE_TUNEMODE            5
#define EE_BYTE_BUFFERSLIDESHOW     6
#define EE_BYTE_UNIT                7
#define EE_BYTE_DABFREQ             8
#define EE_BYTE_VOLUME              9
#define EE_BYTE_MEMORYPOS           10
#define EE_BYTE_THEME               11
#define EE_BYTE_AUTOSLIDESHOW       12
#define EE_BYTE_TOT                 13
#define EE_UINT32_SERVICEID         14
#define EE_CHAR17_SERVICENAME       22 // 17 bytes!
#define EE_PRESETS_FREQ_START       39
#define EE_PRESETS_SERVICEID_START  138
#define EE_PRESETS_NAME_START       930
// End of EEPROM index defines

static const char* const unitString[] = {"dBμV", "dBf", "dBm"};
static const char* const Theme[] = {"Elegant", "GoldenDusk", "Vibrant", "Serenity", "Luminous", "Radiant", "Sunset"};

enum RADIO_TUNE_MODE {
  TUNE_MAN, TUNE_AUTO, TUNE_MEM
};

enum RADIO_MEM_POS_STATUS {
  MEM_DARK, MEM_NORMAL, MEM_EXIST
};

#endif