#define BLYNK_TEMPLATE_ID "TMPL67mKB2sRj"
#define BLYNK_TEMPLATE_NAME "ESP32"
#define BLYNK_AUTH_TOKEN "YXBMPnfwHh3la8xnIqZPBWCzEXDxHwHu"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "YXBMPnfwHh3la8xnIqZPBWCzEXDxHwHu";       // Blynkアプリから取得
char ssid[] = "59669938-2.4G";            // 2.4GHzのWi-Fi
char pass[] = "91828122";
//char ssid[] = "Robohan.net";
//char pass[] = "robohan2008";

const int pwmFreq = 5000;
const int pwmResolution = 7; // 8bit → 0〜255
int8_t vx = 0, vy = 0, omega = 0;

struct Motor {
  uint8_t in1Pin;
  uint8_t in2Pin;
  uint8_t pwmChannel1;
  uint8_t pwmChannel2;
};

Motor motors[4] = {
  {22, 23, 0, 1},  // M1
  {18, 19, 2, 3},  // M2
  {4,  5, 4, 5},   // M3
  {12, 13, 6, 7}   // M4
};


void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(motors[i].in1Pin, OUTPUT);
    pinMode(motors[i].in2Pin, OUTPUT);
  }


  Blynk.begin(auth, ssid, pass);

  for (int i = 0; i < 4; i++) {
    ledcSetup(motors[i].pwmChannel1, pwmFreq, pwmResolution);
    ledcSetup(motors[i].pwmChannel2, pwmFreq, pwmResolution);
    ledcAttachPin(motors[i].in1Pin, motors[i].pwmChannel1);
    ledcAttachPin(motors[i].in2Pin, motors[i].pwmChannel2);
  }
}

void setMotor(int index, int8_t speed) {
  if (speed == 0 || speed == -128) {
    ledcWrite(motors[index].pwmChannel1, 0);
    ledcWrite(motors[index].pwmChannel2, 0);
    return;
  }

  uint8_t pwmValue = map(abs(speed), 1, 127, 1, 127);  // 7bit解像度

  if (speed > 0) {
    ledcWrite(motors[index].pwmChannel1, pwmValue);
    ledcWrite(motors[index].pwmChannel2, 0);
  } else {
    ledcWrite(motors[index].pwmChannel1, 0);
    ledcWrite(motors[index].pwmChannel2, pwmValue);
  }
}

void updateMotors() {
  int8_t m1 = vx - vy + omega;
  int8_t m2 = -vx - vy - omega;
  int8_t m3 = -vx + vy + omega;
  int8_t m4 = vx + vy - omega;

  m1 = constrain(m1, -127, 127);
  m2 = constrain(m2, -127, 127);
  m3 = constrain(m3, -127, 127);
  m4 = constrain(m4, -127, 127);

  Serial.printf("vx:%d vy:%d omega:%d → M1:%d M2:%d M3:%d M4:%d\n", vx, vy, omega, m1, m2, m3, m4);

  setMotor(0, m1);
  setMotor(1, m2);
  setMotor(2, m3);
  setMotor(3, m4);
}



void loop() {
  Blynk.run();
}
//X軸
BLYNK_WRITE(V7) {
  vx = param.asInt();
  updateMotors();
}
//Y軸
BLYNK_WRITE(V8) {
  vy = param.asInt();
  updateMotors();
}
int8_t omegaLeft = 0;
int8_t omegaRight = 0;
//左旋回
BLYNK_WRITE(V9) {
  omegaLeft = param.asInt() ? -127 : 0;
  omega = omegaLeft + omegaRight;
  updateMotors();
}
// 右旋回
BLYNK_WRITE(V10) {
  omegaRight = param.asInt() ? 127 : 0;
  omega = omegaLeft + omegaRight;
  updateMotors();
}


