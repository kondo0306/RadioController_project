#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 5
#define CAN_INT 4

MCP_CAN CAN(CAN_CS);  // CSピン指定
unsigned long lastSend = 0;  // 送信タイミング管理用

void setup() {
  delay(1000);
  SPI.begin(18, 19, 23, CAN_CS);  // SCK, MISO, MOSI, SS
  Serial.begin(115200);
  SPI.begin();
  while (CAN_OK != CAN.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ)) {
    Serial.println("CAN init failed");
    delay(100);
  }
  Serial.println("CAN init success");
}

void loop(){
  if (millis() - lastSend > 500) {  // 500msごとに送信
    byte data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    byte result = CAN.sendMsgBuf(0x100, 0, 8, data);
    if (result == CAN_OK) {
      Serial.println("CAN message sent");
    } else {
      Serial.println("CAN send failed");
    }
    lastSend = millis();
    delay(500);
    for (int i = 0; i < 8; i++) {
      data[i] = 0x00;
    }
    result = CAN.sendMsgBuf(0x100, 0, 8, data);
    delay(500);
  }
  

}
