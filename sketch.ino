#include <WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"

#define DHTPIN 4           // Chân DATA nối vào GPIO 4 trên Wokwi
#define DHTTYPE DHT22      // Đã cập nhật DHT22 theo Wokwi

// --- ĐIỀN THÔNG TIN WIFI CỦA WOKWI (Hoặc WiFi thực tế) ---
const char* ssid = "Wokwi-GUEST"; // Trên Wokwi dùng tên này, không cần mật khẩu
const char* password = "";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

// --- THÔNG SỐ THINGSPEAK CỦA BẠN ---
unsigned long myChannelNumber = 3515940;             // Channel ID
const char * myWriteAPIKey = "0CHH7MRPW4H9GKSO";    // Write API Key

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.print("Dang ket noi WiFi Wokwi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi da ket noi thanh cong!");
  
  ThingSpeak.begin(client);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Loi doc cam bien DHT22!");
    return;
  }

  Serial.print("Nhiet do: "); Serial.print(temp);
  Serial.print(" *C | Do am: "); Serial.print(hum); Serial.println(" %");

  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);

  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (httpCode == 200) {
    Serial.println("Gui len ThingSpeak thanh cong (HTTP 200)!");
  } else {
    Serial.println("Loi gui ThingSpeak, ma HTTP: " + String(httpCode));
  }

  delay(15000); // ThingSpeak yêu cầu giãn cách tối thiểu 15 giây
}