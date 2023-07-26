#include <ESP8266WiFi.h>

const char* ssid = "ssid"; // Local Network name
const char* password = "password"; // Local Network password

WiFiServer server(80);

//MOTOR1 PINS
int ena1 = D0;
int in1 = D1;
int in2 = D2;

//MOTOR2 PINS
int ena2 = D3;
int in3 = D4;
int in4 = D5;

void setup() {
  // Setup webserver
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  WiFi.setHostname("singing-fish-1");
  server.begin();

  // Set motor 1 pin modes
  pinMode(ena1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  // Set motor 2 pin modes
  pinMode(ena2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Turn on both enable pins
  digitalWrite(ena1, HIGH);
  digitalWrite(ena2, HIGH);

  Serial.println(WiFi.localIP());
}

// Motors:
// 1 - Motor 1
// Anything else - Motor 2
// Modes:
// 0 - Off
// 1 - Forward
// 2 - Backward
void moveMotor(int motor, int mode){
  // if motor = 1, then motor 1, else motor 2
  if(motor == 1){
    if(mode == 0){
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }else if(mode == 1){
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW); 
    }else if(mode == 2){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
  }else{
    if(mode == 0){
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }else if(mode == 1){
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);      
    }else if(mode == 2){
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
  }
}

void sendHttpResponse(WiFiClient& client, const String& content) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close"); // Close the connection after sending the response
  client.println();
  client.print(content);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected!");

    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        client.flush();
        sendHttpResponse(client, "Data received successfully");
        // Force close the connection to only accept one connection per client. 
        // Will cause an error on client side, but it works for the purposes of this project. 
        client.stop();

        // Assuming the client sends the integers in the format "number1,number2"
        int commaPos = request.indexOf(',');
        int motor = request.substring(commaPos-1, commaPos).toInt();
        int mode = request.substring(commaPos + 1, commaPos+2).toInt();
        moveMotor(motor, mode);
        // For debugging
        // Serial.println(motor);
        // Serial.println(mode);
      }
    }

    client.stop();
    Serial.println("Client disconnected.");
  }
}
