#include <SPI.h>

#pragma pack(push, 1)
struct GamepadState {
  uint8_t cross, circle, square, triangle;
  uint8_t l1, l2, r1, r2;
  uint8_t up, down, left, right;
  int32_t lx, ly, rx, ry;
};
#pragma pack(pop)

volatile GamepadState myGamepad;
volatile uint8_t index = 0;
const size_t totalBytes = sizeof(GamepadState);

void setup() {
  pinMode(MISO, OUTPUT);      // スレーブはMISOを出力
  SPCR |= _BV(SPE);           // SPI有効化（スレーブモード）
  SPI.attachInterrupt();      // SPI割り込み有効
  Serial.begin(9600);
}

ISR(SPI_STC_vect) {
  ((uint8_t*)&myGamepad)[index++] = SPDR;
  if (index >= totalBytes) {
    index = 0; // 次の構造体受信に備える
  }
}

void loop() {
  Serial.print("×: "); //×
  Serial.print(myGamepad.cross);
  Serial.print("○: "); //○
  Serial.print(myGamepad.circle);
  Serial.print("□: "); //□
  Serial.print(myGamepad.square);
  Serial.print("△: "); //△
  Serial.print(myGamepad.triangle);
  Serial.print("l1: "); 
  Serial.print(myGamepad.l1);
  Serial.print("l2: "); 
  Serial.print(myGamepad.l2);
  Serial.print("r1: ");
  Serial.print(myGamepad.r1);
  Serial.print("r2: ");
  Serial.print(myGamepad.r2);
  Serial.print("↑: ");
  Serial.print(myGamepad.up);  // 上
  Serial.print("↓: ");
  Serial.print(myGamepad.down);  // 下
  Serial.print("→: ");
  Serial.print(myGamepad.left);  // 左
  Serial.print("←: ");
  Serial.print(myGamepad.right);  // 右
  //Lスティック-511～512  ｘ：右が＋ｙ：下が＋
  Serial.print("LX: "); 
  Serial.print(myGamepad.lx); 
  Serial.print(" LY: "); 
  Serial.print(myGamepad.ly); 
  //Rスティック-511～512  ｘ：右が＋ｙ：下が＋
  Serial.print("RX: "); 
  Serial.print(myGamepad.rx); 
  Serial.print(" RY: "); 
  Serial.print(myGamepad.ry);
  Serial.println();
  delay(50);
}