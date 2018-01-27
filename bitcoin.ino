#include <ESP8266WiFi.h>

WiFiClient client;

int pins[] = {12, 13, 15, 0, 16, 2, 5, 4};
int price;

void setup() {
    Serial.begin(115200);

    for (int i = 0; i < 8; i++){
        pinMode(pins[i], OUTPUT);
    }

    digitalWrite(pins[7], LOW);
    digitalWrite(pins[0], HIGH);
    
    WiFi.begin("SSID", "pass");

    Serial.println("connecting\n");
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.print("IP:");
    Serial.println(WiFi.localIP());

    String site = "api.coindesk.com";
    int port = 80;
    Serial.println("connecting to " + site + " on port " + String(port));
    boolean success = client.connect(site, port);
    Serial.println(success ? "success" : "failure");


    delay(100);
    String path = "/v1/bpi/currentprice.json";
    String request = "GET "+path+" HTTP/1.1\r\nHost: "+site+"\r\nuConnection: close\r\n\r\n";
    Serial.println("requesting search");
    client.print(request);
    
    delay(500);

    Serial.println("reading response");
    String res = client.readString();
    Serial.println("response read into memory");
    int ss = res.indexOf("rate_float\":", res.indexOf("GBP"));
    price = res.substring(ss + 12, res.indexOf(".", ss)).toInt();
    Serial.println(price);

    for (int i = 0; i < 14; i++){
        Serial.print(bitRead(price, i));
    }
    
}

void loop() {
    displayNum(price);
}

void displayNum(int n){
    digitalWrite(pins[7], LOW);
    for (int i = 0; i < 7; i++){
        digitalWrite(pins[i], bitRead(n, i));
    }
    delay(2);
    digitalWrite(pins[7], HIGH);
    for (int i = 0; i < 7; i++){
        digitalWrite(pins[i], !bitRead(n, i+7));
    }
    delay(2);
}

