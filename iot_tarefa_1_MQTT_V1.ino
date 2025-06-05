#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid  = "ILUMINET_AE5A";//"Galaxy Z Flip4";//"Redmi";//
const char* password = "86777162";//"laralinda";//"pet112358";//

//infos MQTT broker
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

#define LED_PIN 2

WiFiClient espClient; 
PubSubClient client(espClient);

const char* client_name = "iot_ufpr_equipe_9_client";

String comando_rx;

//-----------Room 1-----------
const char* topic_set_lights_room_1 = "iot_ufpr/equipe9/floor9/room1/light/set";
const char* topic_set_temp_room_1 = "iot_ufpr/equipe9/floor9/room1/temp/set";

const char* topic_get_lights_room_1 = "iot_ufpr/equipe9/floor9/room1/light";
const char* topic_get_temp_room_1 = "iot_ufpr/equipe9/floor9/room1/temp";

int cur_temp_1 = 20;
String led_state_1 = "off";


//-----------Room 2-----------
const char* topic_set_lights_room_2 = "iot_ufpr/equipe9/floor9/room2/light/set";
const char* topic_set_temp_room_2 = "iot_ufpr/equipe9/floor9/room2/temp/set";

const char* topic_get_lights_room_2 = "iot_ufpr/equipe9/floor9/room2/light";
const char* topic_get_temp_room_2 = "iot_ufpr/equipe9/floor9/room2/temp";

int cur_temp_2 = 20;
String led_state_2 = "off";

//-----------Room 3-----------
const char* topic_set_lights_room_3 = "iot_ufpr/equipe9/floor9/room3/light/set";
const char* topic_set_temp_room_3 = "iot_ufpr/equipe9/floor9/room3/temp/set";

const char* topic_get_lights_room_3 = "iot_ufpr/equipe9/floor9/room3/light";
const char* topic_get_temp_room_3 = "iot_ufpr/equipe9/floor9/room3/temp";

int cur_temp_3 = 20;
String led_state_3 = "off";

//----------Alarm------------
const char* topic_alarm = "iot_ufpr/equipe9/floor9/alarm";
volatile bool alarm_active = false;
TaskHandle_t alarmHandle = NULL;

//---------Reset------------
const char* topic_reset = "iot_ufpr/equipe9/floor9/reset";

const int temp_min = 18;
const int temp_max = 24;
const String temp_min_cmd = "min";
const String temp_max_cmd = "max";
const String temp_range[] = {"18", "19", "20", "21", "22" , "23", "24"};

String get_comand(byte* comando_tx, unsigned int length) {
  String comando = "";
  for (unsigned int i = 0; i < length; i++) {
    comando += (char)comando_tx[i];
  }
  return comando;
}

bool is_valid_temp(const String& cmd){
  for(const String& val : temp_range){
    if(cmd == val) return true; 
  }
  return false;
}

void control(char* topic, byte* comando_tx, unsigned int length){
  if(alarm_active && strcmp(topic, topic_alarm) != 0 && strcmp(topic, topic_reset) != 0){
    Serial.println("-----Alarme Ativo-----");
    return;
  }
  
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  comando_rx = get_comand(comando_tx, length);

  Serial.print("Comando: ");
  Serial.println(comando_rx);

//-------------Subscribed topics------------------
  if(strcmp(topic,topic_set_lights_room_1) == 0){ //topic == topic_set_lights
    if(comando_rx == "on"){
      digitalWrite(LED_PIN, HIGH); //Feedback visual
      led_state_1 = "on";
    }
    else if (comando_rx == "off"){
      digitalWrite(LED_PIN, LOW); //Feedback visual
      led_state_1 = "off";
    }
  }
  else if(strcmp(topic,topic_set_temp_room_1) == 0){  //topic == topic_set_temp
    if(comando_rx == "max"){
      cur_temp_1 = temp_max;
      Serial.println(cur_temp_1);
    }
    else if(comando_rx == "min"){
      cur_temp_1 = temp_min;
      Serial.println(cur_temp_1);
    }
    else if(is_valid_temp(comando_rx)){
      cur_temp_1 = comando_rx.toInt();
      Serial.println(cur_temp_1);
    }
    else {
      Serial.println("Comando de temperatura inválido"); //apenas para debug
    }
  }
  else if(strcmp(topic,topic_set_lights_room_2) == 0){ //topic == topic_set_lights
    if(comando_rx == "on"){
      digitalWrite(LED_PIN, HIGH); //Feedback visual
      led_state_2 = "on";
    }
    else if (comando_rx == "off"){
      digitalWrite(LED_PIN, LOW); //Feedback visual
      led_state_2 = "off";
    }
  }
  else if(strcmp(topic,topic_set_temp_room_2) == 0){  //topic == topic_set_temp
    if(comando_rx == "max"){
      cur_temp_2 = temp_max;
      Serial.println(cur_temp_2);
    }
    else if(comando_rx == "min"){
      cur_temp_2 = temp_min;
      Serial.println(cur_temp_2);
    }
    else if(is_valid_temp(comando_rx)){
      cur_temp_2 = comando_rx.toInt();
      Serial.println(cur_temp_2);
    }
    else {
      Serial.println("Comando de temperatura inválido"); //apenas para debug
    }
  }
  else if(strcmp(topic,topic_set_lights_room_3) == 0){ //topic == topic_set_lights
    if(comando_rx == "on"){
      digitalWrite(LED_PIN, HIGH); //Feedback visual
      led_state_3 = "on";
    }
    else if (comando_rx == "off"){
      digitalWrite(LED_PIN, LOW); //Feedback visual
      led_state_3 = "off";
    }
  }
  else if(strcmp(topic,topic_set_temp_room_3) == 0){  //topic == topic_set_temp
    if(comando_rx == "max"){
      cur_temp_3 = temp_max;
      Serial.println(cur_temp_3);
    }
    else if(comando_rx == "min"){
      cur_temp_3 = temp_min;
      Serial.println(cur_temp_3);
    }
    else if(is_valid_temp(comando_rx)){
      cur_temp_3 = comando_rx.toInt();
      Serial.println(cur_temp_3);
    }
    else {
      Serial.println("Comando de temperatura inválido"); //apenas para debug
    }
  }
  else if (strcmp(topic, topic_alarm) == 0){ //Alarm
    if(comando_rx == "on"){
      alarm_active = true;
      vTaskResume(alarmHandle);
      Serial.println("Alarme armado!");
    }
    else if (comando_rx == "off"){
      alarm_active = false;
      vTaskSuspend(alarmHandle);
      digitalWrite(LED_PIN, LOW);
      led_state_1 = "off";
      led_state_2 = "off";
      led_state_3 = "off";
      Serial.println("Alarme desarmado!");

    }
  }
  else if (strcmp(topic, topic_reset) == 0){ //Reset
    if(comando_rx == "reset"){
      alarm_active = false;
      vTaskSuspend(alarmHandle);
      digitalWrite(LED_PIN, LOW);

      cur_temp_1 = 20;
      cur_temp_2 = 20;
      cur_temp_3 = 20;

      led_state_1 = "off";
      led_state_2 = "off";
      led_state_3 = "off";

    Serial.println("Sistema resetado!");
    }
    else{
      Serial.println("Comando de reset inválido! Para Reiniciar sistema envie 'reset'");
    }
  }
}

void connect_broker(){
  while(!client.connected()){
    Serial.print("Conectando-se ao Broker...");

    if(client.connect(client_name)){
      Serial.println("Conectado!");
      //Room 1
      client.subscribe(topic_set_lights_room_1);
      client.subscribe(topic_set_temp_room_1);
      client.subscribe(topic_get_lights_room_1);
      client.subscribe(topic_get_temp_room_1);

      //Room 2
      client.subscribe(topic_set_lights_room_2);
      client.subscribe(topic_set_temp_room_2);
      client.subscribe(topic_get_lights_room_2);
      client.subscribe(topic_get_temp_room_2);

      //Room 3
      client.subscribe(topic_set_lights_room_3);
      client.subscribe(topic_set_temp_room_3);
      client.subscribe(topic_get_lights_room_3);
      client.subscribe(topic_get_temp_room_3);

      //Alarm
      client.subscribe(topic_alarm);

      //Reset
      client.subscribe(topic_reset);

    }
    else{
      Serial.println("Falha, tentando novamente...");
      delay(2000);
    }
  }
}

void connect_wifi(){
  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao Wi-fi...");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-fi conectado!");
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());
}

void mqttLoopTask(void* parameter){
  for(;;){
    if(!client.connected()){
      connect_broker();
    }
    client.loop();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void statusPublisherTask(void* parameter){
  for(;;){
    if(!alarm_active){
      //Room 1
      client.publish(topic_get_temp_room_1, String(cur_temp_1).c_str());
      client.publish(topic_get_lights_room_1, led_state_1.c_str());

      //Room 2
      client.publish(topic_get_temp_room_2, String(cur_temp_2).c_str());
      client.publish(topic_get_lights_room_2, led_state_2.c_str());

      //Room 3
      client.publish(topic_get_temp_room_3, String(cur_temp_3).c_str());
      client.publish(topic_get_lights_room_3, led_state_3.c_str());

      Serial.println("Status publicado.");
    }
    else{
      Serial.println("Alarme Ativo, publicações suspensas!");
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void alarmTask(void* parameter){
  while(true){
    digitalWrite(LED_PIN, HIGH);
    led_state_1 = "on";
    led_state_2 = "on";
    led_state_3 = "on";
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    led_state_1 = "off";
    led_state_2 = "off";
    led_state_3 = "off";
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  connect_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(control);

  xTaskCreate(
    mqttLoopTask, //Nome da função da task
    "MQTT Loop Task", //Nome simbólico da tarefa
    4096, //tamanho da pilha
    NULL, //Parametros 
    1, // Prioridade
    NULL //Handle
  );

  xTaskCreate(
    statusPublisherTask,
    "Status Publisher Task",
    4096,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    alarmTask,
    "Alarm task",
    2048,
    NULL,
    2,
    &alarmHandle
  );

  vTaskSuspend(alarmHandle);
}

void loop() {}
