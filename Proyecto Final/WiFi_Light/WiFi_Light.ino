#include <ESP8266WiFi.h>

const char* ssid = "XXXX";
const char* password = "XXXX";

// GPIO13:
int ledPin = 13;
WiFiServer server(80);

void setup() {
  // Port in COM:
  Serial.begin(115200);
  delay(10);

  // Set output pin and initialize it:
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network:
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");

  // Start the server:
  server.begin();
  Serial.println("Server started.");

  // Print the IP address:
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // Check if a client has connected:
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data:
  Serial.println("New client!");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request:
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Map request to output in serial:
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

  // Return the response:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<head><style>");
  client.println("body { font-size: 3em; color: #4286f4; font-family: \"Verdana\", Sans-serif; }");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 1.1em; margin: 4px 2px; cursor: pointer; }");
  client.println("</style></head>");
  client.println("<html>");
  client.println("<body>");
  client.println("<center>");


  client.print("<h2 style=\"padding: 30px;\">El pin LED est&aacute;: <br>");

  if(value == HIGH) {
    client.print("<b style=\"color: #f44141\">ENCENDIDO</b>");
  } else {
    client.print("<b style=\"color: #f44141\">APAGADO</b>");
  }
  client.println("</h2>");
  client.println("<a href=\"/LED=ON\"\"><button class=\"button\">Encender </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button class=\"button\">Apagar </button></a><br>");
  client.println("</body>");
  client.println("</center>");
  client.println("</html>");


  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
