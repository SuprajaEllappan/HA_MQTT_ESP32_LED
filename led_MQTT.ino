#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "Praveen act"; // Enter your WiFi name
const char *password = "slact7204813084";  // Enter WiFi password
const int led = 2;
// MQTT Broker
const char *mqtt_broker = "192.168.0.100";
const char *topic = "Homeassistant/value";
const char *mqtt_username = "mosquitto";
const char *mqtt_password = "password_not_changed**__";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 // Set software serial baud to 115200;
 Serial.begin(115200);
 pinMode(led,OUTPUT);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-test-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("HA mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 
 client.subscribe(topic);
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);

    // Convert the payload (byte array) to a String
    String Button = "";
    for (unsigned int i = 0; i < length; i++) {
        Button += (char)payload[i];
    }

    // Check if the message is "On"
    if (Button == "ON") {
        digitalWrite(led, HIGH);   // Turn the LED on
               
           }
    if(Button == "OFF") {
        digitalWrite(led, LOW);   // Turn the LED off
                
           }

    // Print the received message
    Serial.print("Received message: ");
    Serial.println(Button);
    Serial.println("-----------------------");
}


void loop() {
 client.loop();
}
