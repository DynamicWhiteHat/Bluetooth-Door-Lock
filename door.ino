/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <bluefruit.h>
#include <Stepper.h>

const int stepsPerRevolution = 1600;  // change this to fit the number of steps per revolution
const int rolePerMinute = 17;    

// OTA DFU service
BLEDfu bledfu;

// Uart over BLE service
BLEUart bleuart;

// Function prototypes for packetparser.cpp
uint8_t readPacket (BLEUart *ble_uart, uint16_t timeout);
float   parsefloat (uint8_t *buffer);
void    printHex   (const uint8_t * data, const uint32_t numBytes);

// Packet buffer
extern uint8_t packetbuffer[];


Stepper myStepper(stepsPerRevolution, 10, 12, 11, 13);

bool locked = false;

String receivedCode="";

void setup(void)
{
  myStepper.setSpeed(rolePerMinute);
  pinMode(LED_RED, OUTPUT);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and start the BLE Uart service
  bleuart.begin();

  // Set up and start advertising
  startAdv();


}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  
  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Wait for new data to arrive
  uint8_t len = readPacket(&bleuart, 500);
  if (len == 0) return;
if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    if (buttnum == 7) {
      digitalToggle(LED_RED);
      delay(1000);
      digitalToggle(LED_RED);
      receivedCode = "";
    }
    else {
    receivedCode += buttnum;
    if (receivedCode.length() == 8) {
    // Compare received code with the expected code
    if (receivedCode == "44112233") {
      // Unlock - Move stepper motor forward
      if (locked) {
      myStepper.step(-stepsPerRevolution);
            locked = !locked;

      }
      else {
      // Lock - Move stepper motor backward
      myStepper.step(stepsPerRevolution);
      locked = !locked;
    } 
    }
    else {
      digitalToggle(LED_RED);
      delay(200);
      digitalToggle(LED_RED);
      delay(200);
      digitalToggle(LED_RED);
      delay(200);
      digitalToggle(LED_RED);
    }
    receivedCode = "";
    buttnum=0;

    }}}
}
