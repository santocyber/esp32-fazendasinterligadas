#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <EEPROM.h>

//defines de id mqtt e tópicos para publicação e subscribe
#define PUBLISHtemperature "MaravilhaMirako/temperature"     
#define PUBLISHhumidade    "MaravilhaMirako/Humidade"    
#define PUBLISHnivel       "MaravilhaMirako/Nivelreservatorio"
#define PUBLISHluz         "MaravilhaMirako/Nivelluz"
#define PUBLISHph          "MaravilhaMirako/pH"
#define PUBLISHppm         "MaravilhaMirako/TDS"
#define ID_MQTT            "Maravilha01"   
#define TOPICO_SUBSCRIBE   "MaravilhaMirako"

// WIFI
const char* SSID = "InternetSA_plus"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "semsenha"; // Senha da rede WI-FI que deseja se conectar
  
//DATA

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "b.ntp.br", -3*3600, 60000);

//PH
const int analogInPin = 35; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;


//temperature
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 

//TDS
#define TdsSensorPin 34
#define VREF 1.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 5 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;




// MQTT
const char* BROKER_MQTT = "iot.mirako.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
#define mqtt_user "mqtt"
#define mqtt_password "mqtt"

 
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída

  
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);



void setup() {
 Serial.begin(9600);
    dht.begin();
    pinMode(TdsSensorPin,INPUT);
    pinMode(analogInPin,INPUT);
    digitalWrite(TdsSensorPin, LOW);
    digitalWrite(analogInPin, LOW); 
    initWiFi();
    initMQTT();
    timeClient.begin();
 

 
}

void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  String msg;
     //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }}
    
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

void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
void loop() {
     //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaConexoesWiFIEMQTT();
     //keep-alive da comunicação com broker MQTT
    MQTT.loop();


//PH



for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol = (float)avgValue*5/1024/6;
 float phValue = -5.70 * pHVol + 91.34;
 Serial.print("sensor = ");
 Serial.println(phValue);
  Serial.print("voltage = ");
  Serial.println(pHVol);
  MQTT.publish(PUBLISHph,String(phValue).c_str(), true);

 
 delay(200);



static unsigned long analogSampleTimepoint = millis();
if(millis()-analogSampleTimepoint > 40U) //every 40 milliseconds,read the analog value from the ADC
{
analogSampleTimepoint = millis();
analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); //read the analog value and store into the buffer
analogBufferIndex++;
if(analogBufferIndex == SCOUNT)
analogBufferIndex = 0;
}
static unsigned long printTimepoint = millis();
if(millis()-printTimepoint > 800U)
{
printTimepoint = millis();
for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF/ 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
float compensationCoefficient=1.0+0.02*(temperature-25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
float compensationVolatge=averageVoltage/compensationCoefficient; //temperature compensation
tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
Serial.print("voltage:");
Serial.print(averageVoltage,2);
Serial.println("V ");
Serial.print("TDS Value:");
Serial.print(tdsValue,0);
Serial.println("ppm");
MQTT.publish(PUBLISHppm,String(tdsValue,0).c_str(), true);
}

//temperature UMIDADE
  float h = dht.readHumidity();
  float t = dht.readTemperature();

     Serial.println("temp e humid");
    Serial.println(h);
    Serial.println(t);
   MQTT.publish(PUBLISHtemperature,String(t).c_str(), true);
   MQTT.publish(PUBLISHhumidade,String(h).c_str(), true);

delay(1000);


}
int getMedianNum(int bArray[], int iFilterLen)
{
int bTab[iFilterLen];
for (byte i = 0; i<iFilterLen; i++)
bTab[i] = bArray[i];
int i, j, bTemp;
for (j = 0; j < iFilterLen - 1; j++)
{
for (i = 0; i < iFilterLen - j - 1; i++)
{
if (bTab[i] > bTab[i + 1])
{
bTemp = bTab[i];
bTab[i] = bTab[i + 1];
bTab[i + 1] = bTemp;
}
}
}
if ((iFilterLen & 1) > 0)
bTemp = bTab[(iFilterLen - 1) / 2];
else
bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
return bTemp;
delay(5000);
}
