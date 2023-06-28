
// librerias necesarias (descargarlas si no estan desde gestor de lib)
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// credenciales wifi de nuestra red
const char* ssid = "chinos8";
const char* password = "12551255";

// servidor web bot whassp a donde enviamos la peticion
const char* host = "api.callmebot.com";
const int httpsPort = 443;

// Pin conectado al sensor magnético
const int sensorPin = 5; //D1 en ESP8266 es 5

void setup() {

  // Configurar el pin del sensor como entrada y serial
  pinMode(sensorPin, INPUT_PULLUP);  
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }

  Serial.println("Conexión WiFi establecida");
}

void loop() {

  // Leer el valor del sensor
  int sensorValue = digitalRead(sensorPin);
  
  // La puerta está cerrada, todo bien
  if (sensorValue == 0) {

    // digitalWrite(ledPin, LOW);  // Apagar el LED
    Serial.println("La puerta está cerrada");
  } 
  
  // La puerta está abierta, mandamos alerta
  else {

    // digitalWrite(ledPin, HIGH);  // Encender el LED
    Serial.println("La puerta está abierta");  
    
    // Si hay conexion, armar la peticion al server WhasspBOT
    if (WiFi.status() == WL_CONNECTED) {
      
      WiFiClientSecure client;
      client.setInsecure();
   
        if (client.connect(host, httpsPort)) {

           // Armado de la peticion con el mensaje incluido 
           String url = "/whatsapp.php?phone=5492216746261&text=PUERTA+ABIERTA+CUIDADO+!+!&apikey=5382296";
           client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                      "Host: " + host + "\r\n" +
                      "Connection: close\r\n\r\n");
            Serial.println("Solicitud GET enviada");
        } 
          
        else {
            
            Serial.println("Error al conectar con el servidor Whassap");
        }

        /*while (client.connected()) {
          if (client.available()) {
            String line = client.readStringUntil('\n');
            Serial.println(line);
          }
        }*/

        client.stop();
    }
  }

  delay(1500); // Esperar X segundos antes de enviar la siguiente solicitud
}
