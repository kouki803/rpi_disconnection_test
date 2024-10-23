#include <WiFi.h>
#include <HTTPClient.h>

//HW const
const int baud_rate_pc = 115200;
const int baud_rate_rpi = 115200;
const int TX_pin = 16;
const int RX_pin = 17;
const int relay_pin = 5;   

//settings param from PC
String ssid = "";
String password = "";
String gas_url = "";
unsigned int waittime = 0; // 待機時間（ミリ秒）

//var for test
int times = 0;
bool rpi_helth = true;  // 試験継続のフラグ

//func
String getSerialInput_for_set(String);
String getSerialInput(int);
void sendResultsToSheet(int, String);


void setup() {
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW); //リレー, デフォルトOFF 

  // PCへの接続確立
  Serial.begin(baud_rate_pc);
  delay(500);
  Serial.println("Connected");

  // RasPiへの接続確立
  Serial1.begin(baud_rate_rpi, 16, 17);
  delay(500);
  Serial1.println("Connected to RasPi");

  // SSIDとパスワードを受け取る
  Serial.println("Enter SSID and Password");
  ssid = getSerialInput_for_set("SSID: ");
  password = getSerialInput_for_set("Password: ");

  // GASのURLを受け取る
  gas_url = getSerialInput_for_set("GAS URL:");

  // 待機時間を受け取る
  waittime = getSerialInput_for_set("wait time: (seconds)").toInt() * 1000;
  if (waittime < 30 * 1000){
    waittime = 30 * 1000; //30秒以下の場合
  }

  // Wi-Fi接続の試行
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}


void loop() {
  Serial.println("test No." + times); 
  digitalWrite(relay_pin, HIGH); //リレーON 
  delay(5000);
  
  // RasPiからのシリアルデータを監視
  String input = getSerialInput(waittime);

  Serial.println("recieved: " + input); 
  
  delay(500);
  digitalWrite(relay_pin, LOW); //リレーOFF（RasPi強制OFF）

  if (input == ""){  
    rpi_helth = false;  // シリアルデータが来ない場合はフラグを変更
    input = "no response";
    }

  // 結果送信
  sendResultsToSheet(times, input);
  times++; 

  // シリアルデータが来なくなった場合にループを終了
  if (!rpi_helth) {
    Serial.println("Test terminated.");
    Serial.println("RasPi booted " + (char)times + "times");
    while (true) {
      delay(1000); // 無限ループ
    }
  
  delay(1000);
  }
}

// シリアル通信で入力を受け取り、改行までの文字列を返す
String getSerialInput_for_set(String prompt){
  String input = "";
  char c;

  Serial.print(prompt);

  while (true) {
    if (Serial.available() > 0) {
      c = Serial.read();
      if (c == '\n' || c == '\r') {
        break;
      }
      input += c;
    }
  }
  return input;

}

// シリアル通信で入力をtimeoutか改行まで受け取り、文字列を返す
String getSerialInput(int timeout) {
  unsigned long startmillis = millis(); //シリアル受信開始時刻

  String input = "";
  char c;

  while (millis() - startmillis < timeout) {
    if (Serial1.available() > 0) {
      c = Serial1.read();
      if (c == '\n' || c == '\r') {
        break;
      }
      input += c;
    }
  }
  return input;
}


// スプレッドシートに起動回数とシリアルメッセージを送信
void sendResultsToSheet(int times, String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(gas_url);

    http.addHeader("Content-Type", "application/json");
    String jsonPayload = "{\"times\":\"" + String(times) + "\",\"message\":\"" + message + "\"}";
    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } 
    else {
      Serial.println("HTTP Error: " + String(httpResponseCode));
    }
    http.end();
  } 
  else {
    Serial.println("WiFi Disconnected");
  }
}