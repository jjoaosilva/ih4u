#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient

//defines:
//defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE_INFO "ih4u/sensores/sub/smoke"     //tópico MQTT de escuta
#define TOPICO_PUBLISH_INFO   "ih4u/sensores/pub/smoke"    //tópico MQTT de envio de informações para Broker

#define ID_MQTT  "amooairtin"     //id mqtt (para identificação de sessão)
                               //IMPORTANTE: este deve ser único no broker (ou seja, 
                               //            se um client MQTT tentar entrar com o mesmo 
                               //            id de outro já conectado ao broker, o broker 
                               //            irá fechar a conexão de um deles).
                               
// WIFI
const char* SSID = "AndroidAP8C20"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "fonteles"; // Senha da rede WI-FI que deseja se conectar

const char* BROKER_MQTT = "broker.mqttdashboard.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT

//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);

int Pinbuzzer = D0; //PINO UTILIZADO PELO BUZZER
int PinA0 = A0;//PINO UTILIZADO PELO SENSOR DE GÁS MQ-2

int leitura_sensor = 600;//DEFININDO UM VALOR LIMITE (NÍVEL DE GÁS NORMAL)

void setup(){
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
            MQTT.subscribe(TOPICO_SUBSCRIBE_INFO); 
            String information =  WiFi.localIP().toString();
            information = "nodeMCU;" + information + ";";
            char sendInfo [24];
            information.toCharArray(sendInfo, 24);
            MQTT.publish(TOPICO_PUBLISH_INFO, sendInfo);
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
    int valor_analogico = analogRead(PinA0); //VARIÁVEL RECEBE O VALOR LIDO NO PINO ANALÓGICO
    Serial.print("Leitura: "); //EXIBE O TEXTO NO MONITOR SERIAL
    Serial.println(valor_analogico);// MOSTRA NO MONITOR SERIAL O VALOR LIDO DO PINO ANALÓGICO
    if (valor_analogico > leitura_sensor){//SE VALOR LIDO NO PINO ANALÓGICO FOR MAIOR QUE O VALOR LIMITE, FAZ 
        digitalWrite(Pinbuzzer, HIGH); //ATIVA O BUZZER E O MESMO EMITE O SINAL SONORO
        MQTT.publish(TOPICO_PUBLISH_INFO, "FIRE ON THE HOLE;");
        delay(3000);
        digitalWrite(Pinbuzzer, LOW);
        delay(30000);
    }else{ //SENÃO, FAZ
        digitalWrite(Pinbuzzer, LOW);//BUZZER DESLIGADO
    }
    delay(100); //INTERVALO DE 100 MILISSEGUNDOS
    Serial.println("- Estado da saida A0 enviado ao broker!");
    delay(1000);
}
 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum  
//Retorno: nenhum
void InitOutput(void)
{
pinMode(PinA0, INPUT); //DEFINE O PINO COMO ENTRADA
pinMode(Pinbuzzer, OUTPUT); //DEFINE O PINO COMO SAÍDA         
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
}
