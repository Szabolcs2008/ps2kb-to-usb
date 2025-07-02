#include <Adafruit_TinyUSB.h>
#include "utils.h"


/*
By Szabolcs2008 (https://github.com/Szabolcs2008)

Plug and pray

Add #define DEBUG for Serial prints (probably wont work but you can try) 
*/

#define DAT 14
#define CLK 15

// #define DAT 22 // PS/2 data in
// #define CLK 23  // PS/2 clock

Adafruit_USBD_HID usb_hid;


bool isConnected = true;

uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_KEYBOARD() };

const uint8_t ONE_COUNT_LOOKUP_TABLE[256] = {
  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
  4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

const char USB_LOOKUP_NORMAL[256] = {
  0, 66, 0, 62, 60, 58, 59, 69, 0, 67, 65, 63, 61, 43, 53, 0, 0, 0, 0, 0, 0, 20, 30, 0, 0, 0, 29, 22, 4, 26, 31, 0, // 31
  0, 6, 27, 7, 8, 33, 32, 0, 0, 44, 25, 9, 23, 21, 34, 0, 0, 17, 5, 11, 10, 28, 35, 0, 0, 0, 16, 13, 24, 36, 37, 0, // 63
  0, 54, 14, 12, 18, 39, 38, 0, 0, 55, 56, 15, 51, 19, 45, 0, 0, 0, 52, 0, 47, 46, 0, 0, 57, 0, 40, 48, 0, 49, 0, 0, // 95
  0, 227, 0, 0, 0, 0, 42, 0, 0, 89, 0, 92, 95, 0, 0, 0, 98, 99, 90, 93, 94, 96, 41, 83, 68, 87, 91, 86, 85, 97, 71, 0, // 127
  0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };

const char USB_LOOKUP_SPECIAL[256] = {

  /*         00    01    02    03    04    05    06    07    08    09    0a    0b    0c    0d    0e    0f   */
  /* 00 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 10 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 20 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 30 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 40 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 50 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 60 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x00, 0x50, 0x4a, 0x00, 0x00, 0x00,
  /* 70 */  0x49, 0x4c, 0x51, 0x00, 0x4f, 0x52, 0x00, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x46, 0x4b, 0x00, 0x00,
  /* 80 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* 90 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* a0 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* b0 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* c0 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* d0 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* e0 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* f0 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

};

uint8_t charBuffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile bool charReady = false;


uint8_t leds = 0b00000000;
uint8_t keys[6] = {0, 0, 0, 0, 0, 0};
uint8_t modifier = 0;
uint8_t internal_pressed[128] = {};

void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) report_id;
  (void) bufsize;

  // if (!isConnected) return; // keyboard is not connected (avoid hanging)

  // LED indicator is output report with only 1 byte length
  if (report_type != HID_REPORT_TYPE_OUTPUT) return;

  // The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
  // Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  uint8_t ledIndicator = buffer[0];

  uint8_t ps2_leds = 0;

  if (ledIndicator & 1) {
    ps2_leds |= 2;
  } 
  if (ledIndicator & 2) {
    ps2_leds |= 4;
  } 
  if (ledIndicator & 4) {
    ps2_leds |= 1;
  } 
  if (ps2_leds != leds) {
    if (send_led(ps2_leds) == 0) {  // keyboard didnt respond
      isConnected = false;
      TinyUSBDevice.detach();
      return;
    };
    leds = ps2_leds;
  }
  
  #ifdef DEBUG
  Serial.printf("LED: %d\n", ledIndicator);
  #endif
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  
  delay(1000);
  
  // TinyUSB_Device_Init(0);

  if (!TinyUSBDevice.isInitialized()) {
      TinyUSBDevice.begin(0);
    }

  usb_hid.setBootProtocol(HID_ITF_PROTOCOL_KEYBOARD);
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("TinyUSB Keyboard");
  usb_hid.setReportCallback(NULL, hid_report_callback);

  if (!usb_hid.begin()) {
      #ifdef DEBUG
      Serial.println("Failed to initialize TinyUSB!");
      #endif
      while (1);
    }


  Serial.begin(115200);

  while(!TinyUSBDevice.mounted()) {
    #ifdef DEBUG
    Serial.println("Waiting for usb...");
    #endif
    delay(500);
    };
  
  
  if (TinyUSBDevice.mounted()) {
      TinyUSBDevice.detach();
      delay(10);
      TinyUSBDevice.attach();
    }
  #ifdef DEBUG
  Serial.println("USB connected.");
  #endif
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(DAT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  #ifdef DEBUG
  Serial.println("PS/2 Keyboard Initialized");
  #endif
  multicore_launch_core1(core1_entry);
  delayMicroseconds(10000);
  digitalWrite(LED_BUILTIN, LOW);
}

uint8_t getc(uint8_t (&buffer)[8]) {
  uint8_t scanCode = kb_getc();

  for (int i = 0; i < 8; i++) {
    charBuffer[i] = 0;
  }

  if (scanCode == 0xe1) {
    charBuffer[0] = 0xe1;
    for (int i = 1; i< 8; i++) {
      charBuffer[i] = kb_getc();
    }
    
  } else if (scanCode == 0xf0) {
    charBuffer[0] = scanCode;
    charBuffer[1] = kb_getc();
  } else if (scanCode == 0xe0) {
    charBuffer[0] = scanCode;
    uint8_t new_char = kb_getc();
    charBuffer[1] = new_char;

    if (new_char == 0xf0) {
      uint8_t new_char_2 = kb_getc();
      charBuffer[2] = new_char_2;

      if (new_char_2 == 0x7c) { // PrtSc | SysRq
        for (int i = 3; i < 6; i++) {
          charBuffer[i] = kb_getc();
        }
      }

    } else if (new_char == 0x12) { // PrtSc | SysRq
      for (int i = 2; i < 4; i++) {
        charBuffer[i] = kb_getc();
      }
    }
      
  } else {
    charBuffer[0] = scanCode;
  }

  return 0;

}

void core1_entry() {
  while (true) {
    if (!charReady) {
      getc(charBuffer);   // BLOCKING call
      charReady = true;   // Signal to Core 0 that data is ready
    }
    sleep_us(1);  // prevent spinning too fast
  }
}

uint8_t oneCount(uint8_t data) {
  return ONE_COUNT_LOOKUP_TABLE[data];
}

// read a PS/2 scan code (8-bit)
uint8_t kb_getc() {
  // This can hang
    while (digitalRead(CLK) == HIGH);
    while (digitalRead(CLK) == LOW);

    uint8_t data = 0;
    
    for (int i = 0; i < 8; i++) {
        while (digitalRead(CLK) == HIGH);
        data |= (digitalRead(DAT) << i);  
        while (digitalRead(CLK) == LOW);
    }

    // Parity bit
    while (digitalRead(CLK) == HIGH);
    while (digitalRead(CLK) == LOW);
    
    while (digitalRead(CLK) == HIGH);
    while (digitalRead(CLK) == LOW);
    
    // if (digitalRead(DAT) != HIGH) return 0xFF;

    return data;
}

// this hangs for some reason
uint8_t kb_send(uint8_t state) {
  pinMode(DAT, OUTPUT);
  pinMode(CLK, OUTPUT);

  // 1) Bring the Clock line low for at least 100 microseconds. 
  digitalWrite(CLK, LOW);
  delayMicroseconds(110);

  // 2) Bring the Data line low
  digitalWrite(DAT, LOW);

  // 3) Release the Clock line. 
  pinMode(CLK, INPUT_PULLUP);

  // 4) Wait for the device to bring the Clock line low. 
  // while (digitalRead(CLK) == HIGH);
  if (!waitPin(CLK, LOW, 10000)) return 1;


  // Data
  for (int i = 0; i < 8; i++) {
    // 5) Set/reset the Data line to send the first data bit.
    digitalWrite(DAT, (state >> i) & 1);

    // 6) Wait for the device to bring Clock high.
    // while (digitalRead(CLK) == LOW);
    if (!waitPin(CLK, HIGH, 10000)) return 1;

    // 7) Wait for the device to bring Clock low. 
    // while (digitalRead(CLK) == HIGH);
    if (!waitPin(CLK, LOW, 10000)) return 1;
  }

  // Parity bit
  // 5) Set/reset the Data line to send the first data bit.
  digitalWrite(DAT, oneCount(state) % 2 ? LOW : HIGH);
  // 6) Wait for the device to bring Clock high.
  // while (digitalRead(CLK) == LOW);
  if (!waitPin(CLK, HIGH, 10000)) return 1;
  // 7) Wait for the device to bring Clock low. 
  // while (digitalRead(CLK) == HIGH);
  if (!waitPin(CLK, LOW, 10000)) return 1;
  
  // 8) Repeat steps 5-7 for the other seven data bits and the parity bit 

  // 9) Release the Data line. 
  pinMode(DAT, INPUT_PULLUP);
  
  // 10) Wait for the device to bring Data low
  // while (digitalRead(DAT) == HIGH);
  if (!waitPin(DAT, LOW, 10000)) return 1;

  // 11) Wait for the device to bring Clock low. 
  // while (digitalRead(CLK) == HIGH); // this was uncommented?
  if (!waitPin(CLK, LOW, 10000)) return 1;

  // 12) Wait for the device to release Data and Clock
  unsigned long start = micros();
  while (digitalRead(CLK) == LOW && digitalRead(DAT) == LOW) {
    if ((micros() - start) > 10000) return 1;
  }

  return 0;
}

uint8_t send_led(uint8_t data) {
  if (kb_send(0xed)) return 0; // LED command
  uint8_t resp = kb_getc();
  if (resp != 0xfa) {
    return resp;
  }
  if (kb_send(data)) return 0; // LED state
  uint8_t resp2 = kb_getc();
  if (resp2 != 0xfa) {
    return resp2;
  }
  return 0xfa;
}

// print functions only if debug is on
#ifdef DEBUG
void printCharBuffer(){
  for (int i = 0; i < 8; i++) {
    Serial.printf("%02x ", charBuffer[i]);
  }
  Serial.println();
}

void printKeyBuffer(){
  Serial.printf("Pressed keys: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02x ", keys[i]);
  }
  Serial.printf("   Modifier: %d", modifier);
  Serial.println();
}

void printInternalPressedBuffer(){
  for (int i = 0; i < 128; i++) {
    Serial.printf("%02x ", internal_pressed[i]);
  }
  Serial.println();
}
#endif

bool is_pressed(uint8_t code) {
  for (int i = 0; i < 128; i++) {
    if (internal_pressed[i] == code) {
      return true;
    }
  }
  return false;
}

uint8_t put_key(uint8_t code) {
  for (int i = 0; i < 128; i++) { // Set internal pressed state
    if (internal_pressed[i] == 0) {
      internal_pressed[i] = code;
      break;
    }
  }
  for (int i = 0; i < 6; i++) {
    if (keys[i] == 0) {
      keys[i] = code;
      return 1;
    }
  }
  return 0;
}

uint8_t remove_key(uint8_t code) {
  for (int i = 0; i < 128; i++) { // Remove internal pressed state
    if (internal_pressed[i] == code) {
      internal_pressed[i] = 0;
    }
  }
  for (int i = 0; i < 6; i++) {
    if (keys[i] == code) {
      keys[i] = 0;
      return 1;
    }
  }
  return 0;
}

void loop() {
  TinyUSBDevice.task();  // USB background tasks
  // #ifdef DEBUG
  // Serial.println("loop()");
  // #endif
  digitalWrite(LED_BUILTIN, isConnected);
  if (!charReady) return;  // No new char yet
  if (!isConnected) {
    reset();
  }
  

  int64_t value = 0;  
  memcpy(&value, charBuffer, 8);

  if (value < 256 && charBuffer[0] != 0xe0 && charBuffer[0] != 0xf0) { // handle normal key press (one byte keycodes at charBuffer[0])
    uint8_t keycode = USB_LOOKUP_NORMAL[value];
    if (charBuffer[0] == 0x11) { // LALT
      modifier |= 0x04;
    } else if (charBuffer[0] == 0x12) { // LSHIFT
      modifier |= 0x02;
    } else if (charBuffer[0] == 0x59) { // RSHIFT
      modifier |= 0x20;
    } else if (charBuffer[0] == 0x14) { // LCTRL
      modifier |= 0x01;
    }
    if (!is_pressed(keycode)) {
      put_key(keycode);
    }
  } else if (charBuffer[0] == 0xf0 && value < 0x10000) { // handle normal key release (one byte keycodes at charBuffer[1])
    if (charBuffer[1] == 0x11) { // LALT
      modifier &= 0b11111011;
    } else if (charBuffer[1] == 0x12) { // LSHIFT
      modifier &= 0b11111101;
    } else if (charBuffer[1] == 0x59) { // RSHIFT
      modifier &= 0b11011111;
    } else if (charBuffer[1] == 0x14) { // LCTRL
      modifier &= 0b11111110;
    } else { // normal key
      uint8_t keycode = USB_LOOKUP_NORMAL[charBuffer[1]];
      remove_key(keycode);
    }
    
  } else if (charBuffer[0] == 0xe0 && value < 0x1000000) { // more special keys (e0 f0 .. or e0 ..)
    if (charBuffer[1] == 0xf0) { // Handle release
      uint8_t keycode = USB_LOOKUP_SPECIAL[charBuffer[2]];

      if (charBuffer[2] == 0x11) { // RALT
        modifier &= 0b10111111;
      } else if (charBuffer[2] == 0x14) {
        modifier &= 0b11101111;
      } else {  // Key is not a modifier
        remove_key(keycode);
      }

    } else { // Handle press
      uint8_t keycode = USB_LOOKUP_SPECIAL[charBuffer[1]];
      if (charBuffer[1] == 0x11) { // RALT
        modifier |= 0x40;
      } else if (charBuffer[1] == 0x14) {
        modifier |= 0x10;
      }
      else {
        if (!is_pressed(keycode)) {
          put_key(keycode);
        }  // Key is not a modifier
      }
    }
  } else if (charBuffer[0] == 0xe0 && charBuffer[1] == 0x12) {
    // Handle the weird shift + key combinations
    if (charBuffer[2] == 0xe0) {  // should be a special key
      uint8_t keycode = USB_LOOKUP_SPECIAL[charBuffer[3]];
      if (!is_pressed(keycode)) {
          put_key(keycode);
        }
    }
  } else if (value == 0x12f0e07cf0e0){  // this fuckass printscreen key
    remove_key(0x46); // printscreen
  }

  // Send report
  uint8_t report_id = 0;
  usb_hid.keyboardReport(report_id, modifier, keys);
  #ifdef DEBUG
  printCharBuffer();
  printKeyBuffer();
  Serial.printf("%llu\n", value);
  #endif
  // Reset flag so Core 1 can read next
  charReady = false;
}
