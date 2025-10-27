/*#include <WiFi.h>

// WiFiの設定
const char* ssid = "59669938-2.4G"; // 各自のルータのSSIDを記入
const char* password = "91828122"; // 各自のルータのパスワードを記入

// サーバーの設定
WiFiServer server(80);
String header; // HTTPリクエストのヘッダー

// LEDピンの設定
const int ledPin = 23;
bool ledState = LOW;

void setup() {
  // シリアルモニタの初期化
  Serial.begin(115200);
  
  // LEDピンを出力モードに設定
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  // WiFi接続
  Serial.println("WiFiに接続中...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFiに接続完了!");
  Serial.println("IPアドレス: ");
  Serial.println(WiFi.localIP());

  // サーバーを開始
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // クライアント接続の確認
  
  if (client) { // クライアントが接続した場合
    Serial.println("クライアントが接続しました");
    String currentLine = ""; // 現在の受信行を保存

    while (client.connected()) {
      if (client.available()) { // データが利用可能か確認
        char c = client.read();
        header += c;
        Serial.write(c);

        // 行が終わった場合
        if (c == '\n') {
          // HTTPリクエストの終了を確認
          if (currentLine.length() == 0) {
            // リクエストに応じてLEDを制御
            if (header.indexOf("GET /LED_ON") >= 0) {
              Serial.println("LED ON");
              ledState = HIGH;
              digitalWrite(ledPin, ledState);
            } else if (header.indexOf("GET /LED_OFF") >= 0) {
              Serial.println("LED OFF");
              ledState = LOW;
              digitalWrite(ledPin, ledState);
            }

            // HTTPレスポンスを送信
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // HTMLを生成
            client.println("<!DOCTYPE html><html>");
            client.println("<head><title>ESP32 LED Control</title></head>");
            client.println("<body>");
            client.println("<h1>ESP32 LED Control</h1>");
            client.println("<p>LED is currently " + String(ledState ? "ON" : "OFF") + "</p>");
            client.println("<p><a href=\"/LED_ON\">Turn ON</a></p>");
            client.println("<p><a href=\"/LED_OFF\">Turn OFF</a></p>");
            client.println("</body></html>");

            // HTTPリクエストの処理終了
            break;
          } else { 
            currentLine = ""; // 次の行に進む
          }
        } else if (c != '\r') {
          currentLine += c; // 受信した行を追加
        }
      }
    }

    // クライアントを切断
    client.stop();
    Serial.println("クライアントを切断しました");
    
    // ヘッダーのリセット
    header = "";
  }
}*/
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

const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmFreq = 5000;
const int pwmResolution = 7; // 8bit → 0〜255
const int pwmPin1 = 22;
const int pwmPin2 = 23;



void setup() {
  Serial.begin(115200);
  pinMode(pwmPin1, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  ledcSetup(pwmChannel1, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin1, pwmChannel1);

  ledcSetup(pwmChannel2, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin2, pwmChannel2);
}

void setMotor(int8_t speed) {
  if (speed >= 0) {
    ledcWrite(pwmChannel1, speed);
    ledcWrite(pwmChannel2, 0);
  } else {
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, -speed);
  }
}



void loop() {
  Blynk.run();
}

BLYNK_WRITE(V7) {
  int value = param.asInt(); 
  Serial.printf("x:%d\n",value);
  setMotor(value);
}

/*BLYNK_WRITE(V8) {
  int value = param.asInt();
  Serial.printf("y:%d\n",value);
  setMotor(value); 
  ledcWrite(pwmChannelY, value);
}*/