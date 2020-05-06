//Programa: NodeMCU e MQTT - Controle e Monitoramento IoT
//Autor: Pedro Bertoleti

#include <SPI.h>
#include <Wire.h> 
#include <WiFi.h>
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

 
//defines:
//defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE "MQTTMirakoEnvia"     //tópico MQTT de escuta
#define TOPICO_PUBLISH   "MQTTMirakoRecebe"    //tópico MQTT de envio de informações para Broker
#define humidity_topic "sensor/humidity"
#define temperature_celsius_topic "sensor/temperature_celsius"
#define topicpub "MirakoEnvia"
#define ID_MQTT  "Maravilha01"     //id mqtt (para identificação de sessão)
                               //IMPORTANTE: este deve ser único no broker (ou seja, 
                               //            se um client MQTT tentar entrar com o mesmo 
                               //            id de outro já conectado ao broker, o broker 
                               //            irá fechar a conexão de um deles).
                                
 
//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1


//Ultrasound sensor SNS-US020
unsigned int EchoPin = D2;           // connect Pin 2(Arduino digital io) to Echo at US-015
unsigned int TrigPin = D3;           // connect Pin 3(Arduino digital io) to Trig at US-015
unsigned long Time_Echo_us = 0;
int len_cm  = 0;
long lastMsg = 0;
char msg[50];
int value = 0;


 
// WIFI
const char* SSID = "InternetSA_tube"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "cadebabaca"; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
const char* BROKER_MQTT = "iot.mirako.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
#define mqtt_user "mqtt"
#define mqtt_password "mqtt"

 
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída
  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);
 
/* 
 *  Implementações das funções
 */



/*void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}*/

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

 
void setup() 
{
    //inicializações:
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();

}
  
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
  
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
   
    
}
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentativa de conexao em 5s");
            delay(5000);
        }
    }
}
  
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum


void EnviaEstadoOutputMQTT(void)
{

}
 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum
//Retorno: nenhum
void InitOutput(void)
{
  
}
 
 
//programa principal
void loop() 
{   
    //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaConexoesWiFIEMQTT();
 
    //envia o status de todos os outputs para o Broker no protocolo esperado
    EnviaEstadoOutputMQTT();
 
    //keep-alive da comunicação com broker MQTT
    MQTT.loop();


      // Wait a few seconds between measurements.
  delay(2000);
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();

   MQTT.publish(temperature_celsius_topic, String(t).c_str(), true);
   MQTT.publish(humidity_topic, String(h).c_str(), true);
    Serial.print(String(h));
    Serial.print(String(t));


//Sensor Distancia
const int trigpin = 27;
const int echopin = 26;
pinMode(trigpin, OUTPUT);
pinMode(echopin, INPUT);
digitalWrite(trigpin,LOW);
delayMicroseconds(2);
digitalWrite(trigpin, HIGH);
delayMicroseconds(5);
 // Read US sensor data
    digitalWrite(trigpin, HIGH);              //begin to send a high pulse, then US-015 begin to measure the distance
    delayMicroseconds(50);                    //set this high pulse width as 50us (>10us)
    digitalWrite(trigpin, LOW);               //end this high pulse
    
    Time_Echo_us = pulseIn(echopin, HIGH);               //calculate the pulse width at EchoPin, 
    if((Time_Echo_us < 60000) && (Time_Echo_us > 1))     //a valid pulse width should be between (1, 60000).
    {
      len_cm = (Time_Echo_us*34/100)/20;      //calculate the distance by pulse width, Len_mm = (Time_Echo_us * 0.34mm/us) / 20 (cm)
      Serial.print("Present Distance is: ");  //output result to Serial monitor
      Serial.print(len_cm, DEC);            //output result to Serial monitor
      Serial.println("cm");                 //output result to Serial monitor
      snprintf (msg, 50, "%ld", len_cm);
      MQTT.publish("Nivel", msg);      // publish to MQTT server
    }


long duration;
int distance;
  duration = pulseIn(echopin,HIGH);
  distance = duration *0.034/2;


   MQTT.publish("Distancia", String(distance).c_str(), true);
   Serial.print(String(distance));


//Sensor LUZ

 int volsound;
  volsound=analogRead(13);
  Serial.println("Som");
  Serial.println(volsound,DEC);
  MQTT.publish("LUZ", String(volsound).c_str(), true);

}
