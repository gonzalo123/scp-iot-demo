#include <ESP8266WiFi.h>

const int potentiometerPin = 0;

// Wifi configuration
const char* ssid = "my-wifi-ssid";
const char* password = "my-wifi-password";

// SAP HCP specific configuration
const char* host = "mytenant.hanatrial.ondemand.com";
String device_id = "my-device-ide";
String message_type_id = "my-device-type-id";
String oauth_token = "my-oauth-token";

String url = "https://[mytenant].hanatrial.ondemand.com/com.sap.iotservices.mms/v1/api/http/data/" + device_id;

const int httpsPort = 443;

WiFiClientSecure clientTLS;

void wifiConnect() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendMessage(int value) {
  String payload = "{\"mode\":\"async\", \"messageType\":\"" + message_type_id + "\", \"messages\":[{\"value\": " + (String) value + "}]}";
  Serial.print("connecting to ");
  Serial.println(host);
  if (!clientTLS.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("requesting payload: ");
  Serial.println(url);

  // using HTTP/1.0 enforces a non-chunked response
  clientTLS.print(String("POST ") + url + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json;charset=utf-8\r\n" +
               "Authorization: Bearer " + oauth_token + "\r\n" +
               "Content-Length: " + payload.length() + "\r\n\r\n" +
               payload + "\r\n\r\n");

  Serial.println("request sent");

  Serial.println("reply was:");
  Serial.println("==========");
  while (clientTLS.connected()) {
    String line = clientTLS.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("==========");
}

void setup() {
  Serial.begin(9600);
  wifiConnect();

  ///sendMessage(20);

  delay(10);
}

int mem;
void loop() {

  int value = ((analogRead(potentiometerPin) * 100) / 1010);
  if (value < (mem - 1) or value > (mem + 1)) {
    sendMessage(value);
    Serial.println(value);
    mem = value;
  }

  delay(200);
}