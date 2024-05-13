#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "Nome do seu wifi";
const char* password = "senha do seu wifi";

WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

String page = "<!DOCTYPE html>\
<html>\
<head>\
<meta charset=\"UTF-8\">\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
<title>ESP32 Chat</title>\
<style>\
body{\
    background-color: rgb(19, 25, 24);\
    color: gray;\
}\
#chat{\
    border: 1px solid rgb(196, 169, 169);\
    border-radius: 10px;\
    overflow: auto;\
}\
.recebido {\
background-color: yellowgreen;\
text-align: left;\
margin-right: auto;\
}\
\
.enviado {\
background-color: blue;\
text-align: right;\
margin-left: auto;\
}\
.enviado,.recebido{\
border-radius: 10px;\
margin-bottom: 3px;\
padding-left: 5px;\
padding-right: 5px;\
color: rgb(60, 60, 60);\
max-width:100%;\
}\
.enviado > *,.recebido > * {\
  max-width: 100%;\
}\
.horario{\
font-size: small;\
}\
#message{\
width: 73%;\
}\
#snd{\
width: 25%;\
border: none;\
background-color: blue;\
outline: gray;\
}\
#message,#snd{\
border-radius: 10px;\
height: 35px;\
}\
#msgbx{\
margin-top: 10px;\
}\
</style>\
</head>\
<body>\
<h1>ESP32 Chat</h1>\
<div id=\"chat\" style=\"width: 99vw;height: 60vh;display: flex;align-content: center;flex-direction: column;\"></div>\
<div id=\"msgbx\">\
<input type=\"text\" id=\"message\" placeholder=\"Digite sua mensagem\">\
<button id=\"snd\">Enviar</button>\
</div>\
<script>\
function iniciarChat() {\
    var nome = '';\
    var wsUri = 'ws://' + window.location.hostname + ':81/';\
    var websocket = new WebSocket(wsUri);\
    function enviarMensagem() {\
        var message = '<' + nome + '> ' + document.getElementById('message').value;\
        websocket.send(message);\
        document.getElementById('message').value = '';\
    }\
    document.getElementById('message').addEventListener('keypress', function(event) {\
        if (event.key === 'Enter') {\
            enviarMensagem();\
        }\
    });\
    document.getElementById('snd').addEventListener('click', enviarMensagem);\
    websocket.onopen = function(evt) {\
        console.log('Conectado ao servidor');\
    };\
    websocket.onmessage = function(evt) {\
        var data = evt.data;\
        var regex = /<([^>]*)>/;\
        var matches = data.match(regex);\
        let lname = matches[1];\
        var messageClass = 'recebido';\
        if (data.indexOf('<' + nome + '>') !== -1) {\
            messageClass = 'enviado';\
        }\
        let mensag = data.replace(matches[0], \" \");\
        document.getElementById('chat').innerHTML += '<div class=\"' + messageClass + '\"><div><strong>' + lname +'</strong></div>' + mensag.trim() + '<div class=\"horario\">'+new Date().toLocaleString('pt-BR', {day: '2-digit',month: '2-digit',year: 'numeric',hour: '2-digit',minute: '2-digit',second: '2-digit'})+'</div></div>';\
    };  \
    nome = prompt('Qual é o seu nome?');\
}\
window.onload = iniciarChat;\
</script>\
</body>\
</html>";

void setup() {
  Serial.begin(115200);
  connectWiFi(); // Conecta à rede Wi-Fi existente
  setupServer();
  setupWebSocket();
}

void loop() {
  loopServer();
  loopWebSocket();
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando à rede Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Conectado à rede Wi-Fi com sucesso");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  server.send(200, "text/html", page);
}

void setupServer() {
  server.on("/", handleRoot);
  server.begin();
}

void loopServer() {
  server.handleClient();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_CONNECTED:
      Serial.printf("[%u] Conectado!\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Recebido: %s\n", num, payload);
      webSocket.broadcastTXT(payload, length);
      break;
  }
}

void setupWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loopWebSocket() {
  webSocket.loop();
}
