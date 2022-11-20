/*
Este código fonte tem por objetivo integrar o componente IOT transmissor com o broker.
Ele possibilitará:
1 - Recuperar a leitura dos parâmetros de luminisidade através do sensor LDR presente no esquema;
2 - Ler estes parâmetros para tomada de decisão;
3 - Enviar dados para o Broker de acordo com leitura e decisão dos parâmetros;
4 - Monitorar os parâmetros em real time;
Obs: para que fosse possível desenvolver este projeto, eu utilizei como exemplo o código fonte localizado em
Arquivo>Exemplos>PubSubClient>mqtt_esp8266, da biblioteca PubSubClient”, feita por Nick O’Leary, que mostra
como manter uma conexão com o broker MQTT.
*/

#include <ESP8266WiFi.h> 
#include <PubSubClient.h>

// Definição da porta do sensor LDR e da variável de leitura
int ldr = A0; // Atribui A0 a variável LDR (sensor de luminosidade)
int valorldr = 0; // Variável que receberá o valor do sensor LDR
bool status =  false;// true - ACESO // false - APAGADO 
bool statusAnterior = false; // true - ACESO // false - APAGADO 

//WiFi
const char* SSID = "VIVOFIBRA-122-2G"; // NOME DA REDE WIFI
const char* PASSWORD = "Abacaxi000"; //SENHA DA REDE WIFI
WiFiClient wifiClient;                        
 
//MQTT Server
const char* BROKER_MQTT = "broker.mqtt-dashboard.com"; // "mqtt.eclipseprojects.io"; //URL do broker MQTT
int BROKER_PORT = 1883; // Porta do Broker MQTT

// Identificadores dos meus sensores no Broker MQTT
#define ID_MQTT  "SENSOR" //ID do sensor LDR
#define TOPIC_PUBLISH "IOTMACKENZIE" //ID do tópico
PubSubClient MQTT(wifiClient); // Instancia o Cliente MQTT passando o objeto espClient

//Declaração das Funções
void mantemConexoes(); //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi(); //Faz conexão com WiFi
void conectaMQTT(); //Faz conexão com Broker MQTT
void enviaPacote();

void setup() {
  pinMode(ldr, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);   
}

void loop() {
  mantemConexoes();
  enviaValores();
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
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void enviaValores() {
  
  valorldr = analogRead(ldr);

  if((valorldr) > 500){ //Luz ambiente baixa

    if(status == false){ // Abajour apagado -> Envia instrução para acender    
      MQTT.publish("IOTMACKENZIE", "1");
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Luz ambiente baixa..instrucao para acender abajour enviada.");
      Serial.println(valorldr);
      delay(1000);
      statusAnterior = status;
      status = true;
    }

  }
  
  else if((valorldr) < 500){ //Luz ambiente alta
    
    if(status == true){  // Abajour ligado -> Envia instrução para apagar
      MQTT.publish("IOTMACKENZIE", "0");
      Serial.println("Luz ambiente alta..instrucao para apagar abajour enviada.");
      Serial.println(valorldr);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      statusAnterior = status;
      status = false;
    }

  }    

}