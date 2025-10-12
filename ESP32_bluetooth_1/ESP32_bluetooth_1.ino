#include <Bluepad32.h>
#include <SPI.h>

#define SS_PIN 5

ControllerPtr myGamepad;
struct GamepadState {
  // ボタン類
  uint8_t cross;     // ×
  uint8_t circle;     // ○
  uint8_t square;     // □
  uint8_t triangle;     // △
  uint8_t l1;
  uint8_t l2;
  uint8_t r1;
  uint8_t r2;

  // D-Pad（方向キー）
  uint8_t up;
  uint8_t down;
  uint8_t left;
  uint8_t right;

  // スティック（-511～512）
  int32_t lx;  // 左スティックX
  int32_t ly;  // 左スティックY
  int32_t rx;  // 右スティックX
  int32_t ry;  // 右スティックY
};

void setup(){
    Serial.begin(115200); 
    BP32.setup(&onConnected, &onDisconnected); 
    BP32.forgetBluetoothKeys(); // 初回ペアリング時に推奨 
    SPI.begin(); // GPIO18=SCK, GPIO23=MOSI, GPIO19=MISO
    pinMode(SS_PIN, OUTPUT);
}
void loop(){
    GamepadState state;
    BP32.update();
    if (myGamepad && myGamepad->isConnected()){
        Serial.print("×: "); //×
        Serial.print(myGamepad->a());
        Serial.print("○: "); //○
        Serial.print(myGamepad->b());
        Serial.print("□: "); //□
        Serial.print(myGamepad->x());
        Serial.print("△: "); //△
        Serial.print(myGamepad->y());
        Serial.print("l1: "); 
        Serial.print(myGamepad->l1());
        Serial.print("l2: "); 
        Serial.print(myGamepad->l2());
        Serial.print("r1: ");
        Serial.print(myGamepad->r1());
        Serial.print("r2: ");
        Serial.print(myGamepad->r2());
        Serial.print("↑: ");
        Serial.print(myGamepad->dpad() & 0b0001 ? 1 : 0);  // 上
        Serial.print("↓: ");
        Serial.print(myGamepad->dpad() & 0b0010 ? 1 : 0);  // 下
        Serial.print("→: ");
        Serial.print(myGamepad->dpad() & 0b0100 ? 1 : 0);  // 左
        Serial.print("←: ");
        Serial.print(myGamepad->dpad() & 0b1000 ? 1 : 0);  // 右
        //Lスティック-511～512  ｘ：右が＋ｙ：下が＋
        Serial.print("LX: "); 
        Serial.print(myGamepad->axisX()); 
        Serial.print(" LY: "); 
        Serial.print(myGamepad->axisY()); 
        //Rスティック-511～512  ｘ：右が＋ｙ：下が＋
        Serial.print("RX: "); 
        Serial.print(myGamepad->axisRX()); 
        Serial.print(" RY: "); 
        Serial.print(myGamepad->axisRY());
        Serial.println();

        if(myGamepad->a() == 1)myGamepad->setColorLED(0, 0, 255);
        else if(myGamepad->b() == 1)myGamepad->setColorLED(255, 0, 0);
        else if(myGamepad->x() == 1)myGamepad->setColorLED(255, 128, 128);
        else if(myGamepad->y() == 1)myGamepad->setColorLED(0, 255, 0);
        else myGamepad->setColorLED(255, 255, 255);

        state.cross = myGamepad->a();
        state.circle = myGamepad->b();
        state.square = myGamepad->x();
        state.triangle = myGamepad->y();
        state.l1 = myGamepad->l1();
        state.l2 = myGamepad->l2();
        state.r1 = myGamepad->r1();
        state.r2 = myGamepad->r2();

        state.up    = (myGamepad->dpad() & 0b0001) ? 1 : 0;
        state.down  = (myGamepad->dpad() & 0b0010) ? 1 : 0;
        state.left  = (myGamepad->dpad() & 0b0100) ? 1 : 0;
        state.right = (myGamepad->dpad() & 0b1000) ? 1 : 0;

        state.lx = myGamepad->axisX();
        state.ly = myGamepad->axisY();
        state.rx = myGamepad->axisRX();
        state.ry = myGamepad->axisRY();

        sendGamepadState(state);
    }
    delay(50); 

}

void onConnected(ControllerPtr ctl){ 
    Serial.println("DS4 connected!"); 
    myGamepad = ctl; 
    myGamepad->setColorLED(255, 255, 255);
}
void onDisconnected(ControllerPtr ctl){
    Serial.println("DS4 disconnected.");
    myGamepad = nullptr; 
}

void sendGamepadState(const GamepadState& state) {
  const uint8_t* data = reinterpret_cast<const uint8_t*>(&state);
  const size_t len = sizeof(GamepadState);

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(SS_PIN, LOW);
  for (size_t i = 0; i < len; ++i) {
    SPI.transfer(data[i]);
  }
  digitalWrite(SS_PIN, HIGH);
  SPI.endTransaction();
}
