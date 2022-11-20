/*
Este código fonte permite que o esquema RECEPTOR IOT se conecte com o broker MQTT e receba mensagens do esquema TRANSMISSOR.
Ele possibilitará:
1 - Receber mensagens com instruções para apagar ou acender o abajur;
2 - Quem controlará se o abajur está aceso ou apagado é o esquema TRANSMISSOR, por este motivo não teremos testes neste programa;
3 - Ao receber uma instrução do TRANSMISSOR, o programa verifica se é 0 ou 1, se for 0 então apaga luzes, se for 1, então acende;
Obs: para que fosse possível desenvolver este projeto, eu utilizei como exemplo o código fonte localizado em
Arquivo>Exemplos>PubSubClient>mqtt_esp8266, da biblioteca PubSubClient”, feita por Nick O’Leary, que mostra
como manter uma conexão com o broker MQTT.
*/

#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#define RELE 0 // Modulo RELE

//WIFI
const char* SSID = "VIVOFIBRA-122-2G"; // NOME DA REDE WIFI
const char* PASSWORD = "Abacaxi000"; //SENHA DA REDE WIFI
WiFiClient wifiClient;                        
 
//MQTT Server
const char* BROKER_MQTT = "broker.mqtt-dashboard.com"; // "mqtt.eclipseprojects.io"; //URL do broker MQTT
int BROKER_PORT = 1883; // Porta do Broker MQTT

#define ID_MQTT  "ATUADOR" //ID do Atuador (ESP01)
#define TOPIC_SUBSCRIBE "IOTMACKENZIE" //ID do tópico
PubSubClient MQTT(wifiClient); // Instancia o Cliente MQTT passando o objeto espClient

//Declaração das Funções
void mantemConexoes(); //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi(); //Faz conexão com WiFi
void conectaMQTT(); //Faz conexão com Broker MQTT
void recebePacote(char* topic, byte* payload, unsigned int length);

void setup() {

  pinMode(RELE, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  MQTT.setCallback(recebePacote); 
}

void loop() {
  mantemConexoes();
  MQTT.loop();
}

void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
        
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WIFI  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);  
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP()); 
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    //obtem a string do pacote recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    // Se instrução recebida for 0, apaga o abajur
    if (msg == "0") {

      Serial.println("Luz ambiente alta..instrucao para apagar abajour recebida.");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);

      digitalWrite(RELE, HIGH);
    }
    // Se instrção recebida for 1, acende o abajur
    if (msg == "1") {
    
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Luz ambiente baixa..instrucao para acender abajour re.");
      delay(1000);

      digitalWrite(RELE, LOW);
    }
}