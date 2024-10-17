#include <WiFi.h>
#include <WebSocketsServer.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "IPMLabo"; // Reemplaza con tu SSID
const char* password = "j2LK98!we"; // Reemplaza con tu contraseña

WebSocketsServer webSocket = WebSocketsServer(81); // Puerto 81

void setup() {
    Serial.begin(115200);

    // Conectar a WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }
    Serial.println("Conectado a WiFi");

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    webSocket.loop(); // Mantener el bucle de WebSocket
}

// Función para manejar eventos de WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch (type) {
        case WStype_TEXT:
            Serial.printf("Mensaje recibido: %s\n", payload); // Imprimir mensaje en la consola
            Serial.printf("El color recibido es: %s\n", payload); // Mensaje adicional para mostrar el color
            break;
        case WStype_CONNECTED:
            Serial.printf("Cliente conectado desde: %u\n", num);
            break;
        case WStype_DISCONNECTED:
            Serial.printf("Cliente desconectado: %u\n", num);
            break;
    }
}
