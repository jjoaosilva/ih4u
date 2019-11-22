// sudo chmod a+rw /dev/ttyUSB0
//Programa: NodeMCU e MQTT - Controle e Monitoramento IoT
//Autor: Pedro Bertoleti
 
#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
 
//defines:
//defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE "ih4u/sensores/sub/sound"     //tópico MQTT de escuta
#define TOPICO_PUBLISH "ih4u/sensores/pub/sound"    //tópico MQTT de envio de informações para Broker
                                                   
#define ID_MQTT  "SoundHomeAut"     //id mqtt (para identificação de sessão)
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
 
 
// WIFI
//const char* SSID = "ARRIS-RV"; // SSID / nome da rede WI-FI que deseja se conectar
//const char* PASSWORD = "J606208221"; // Senha da rede WI-FI que deseja se conectar

// 3G
const char* SSID = "AndroidAP8C20"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "fonteles"; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
//const char* BROKER_MQTT = "3.15.205.236"; //URL do broker MQTT que se deseja utilizar
//int BROKER_PORT = 1883; // Porta do Broker MQTT

const char* BROKER_MQTT = "broker.mqttdashboard.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
//const char* BROKER_MQTT = "soldier.cloudmqtt.com"; //URL do broker MQTT que se deseja utilizar
//int BROKER_PORT = 10290; // Porta do Broker MQTT
//const char* USER_MQTT = "slhhlhba";
//const char* PASSWORD_MQTT = "cX2o65sDKOK_";

 
 
//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
//char estadoSaidaGreen = '1';
//char estadoSaidaRed = '1';//variável que armazena o estado atual da saída
  
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
//int Led = D6 ;    // define LED Interface
//int Sound = D7;    // define Sound Interface
//int val = 0;      // define numeric variables val

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
    Serial.begin(9600);
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
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
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
    if( digitalRead(D7) == HIGH){

      digitalWrite(D6,HIGH);
      //MQTT.publish(TOPICO_PUBLISH, "ligado"); 
      delay(1000);
      digitalWrite(D6,LOW);
      }   
    else{
      digitalWrite(D6, LOW);
      }
    Serial.println("- Estado da saida D0 enviado ao broker!");
    delay(1000);
}
 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum  
//Retorno: nenhum
void InitOutput(void)
{
    pinMode (D6, OUTPUT) ;       // define LED as output interface
    pinMode (D7, INPUT) ;
          
}

 
 
//programa principal

void loop() 
{
      
    VerificaConexoesWiFIEMQTT();
    
      if (digitalRead(D7) == HIGH) //
      {
        MQTT.publish(TOPICO_PUBLISH, "ligado");
        digitalWrite (D6, HIGH);
        delay(1000);
        }
      else
      {
        digitalWrite (D6, LOW);
      }
    
    //Enva dados para o broker
    //EnviaDadosParaBroker();
    
    //garante funcionamento das conexões WiFi e ao broker MQTT
 
    //envia o status de todos os outputs para o Broker no protocolo esperado
    //EnviaEstadoOutputMQTT();
 
    //keep-alive da comunicação com broker MQTT
   MQTT.loop();

    
}
