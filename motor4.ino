#define ROSSERIAL_ARDUINO_TCP
#include <ESP8266WiFi.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
String msg_data;
#define pinA D1
#define pinB D6
#define pinZ 
#define pwm D8
#define dir D7

int ppr=96; //holes on the encoder disc
volatile long pos=0;
volatile long last_pos=0;
float kp = 0.5;
float kd = 0;
bool rotation=0;
int gear_ratio=112; //gear ratio of motor
float goal_angle=360; //angle where the motor should turn to
float goal = (goal_angle/360)*(ppr*gear_ratio);

void chatterCallback(const geometry_msgs::Twist &msg)
{
  Serial.println(msg.angular.y);
  goal_angle=msg.angular.y;
  goal= (goal_angle/360)*(ppr*gear_ratio);
}
ros::NodeHandle nh;

geometry_msgs::Twist str_msg;
ros::Subscriber<geometry_msgs::Twist> chatter("chatter", &chatterCallback);
char hello[13] = "hello world!";


const char *ssid =  "Top_station";     // replace with your wifi ssid and wpa2 key
const char *pass =  "rig_nitc";

IPAddress server(192, 168, 0, 69);  //40/50
const uint16_t server_port = 11411;


WiFiClient client;


ICACHE_RAM_ATTR void countA(){
  bool a=digitalRead(pinA);
  bool b=digitalRead(pinB);
  if(b==HIGH){
    pos--;
  }
  if(b==LOW){
    pos++;
  }
}

ICACHE_RAM_ATTR void countB(){
  bool a=digitalRead(pinA);
  bool b=digitalRead(pinB);
  if (b==HIGH){
    if(a==HIGH){
      pos--;
      rotation=1;//counterclockwise count
    }
  }
  else{
    Serial.println("huh?");
  }
}

ICACHE_RAM_ATTR void syncZ(){ //could be shorter
  if(pos>0){
    if (rotation==0){ //clockwise
      pos= ((pos-1)/ppr+1)*ppr; //round it to the nearest multiple of ppr greater than pos
    }
    if (rotation==1){ //anti-clockwise
      pos= ((pos)/ppr)*ppr; //round it to the nearest multiple of ppr lesser than pos
    }
  }
    if(pos<0){
    if (rotation==0){ //clockwise
      pos= ((pos)/ppr)*ppr; //round it to the nearest multiple of ppr greater than pos
    }
    if (rotation==1){ //anti-clockwise
      pos= ((pos+1)/ppr-1)*ppr; //round it to the nearest multiple of ppr lesser than pos
    }
  }      
}

void setup() {
  Serial.begin(9600);
  pinMode(pinA,INPUT_PULLUP);
  pinMode(pinB,INPUT_PULLUP);
  //pinMode(pinZ,INPUT_PULLUP);
  pinMode(pwm,OUTPUT);
  pinMode(dir,OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(pinA),countA,RISING);
  //attachInterrupt(digitalPinToInterrupt(pinB),countB,RISING);
  //attachInterrupt(digitalPinToInterrupt(pinZ),syncZ,RISING);
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  Serial.println("WiFi connected");
  nh.getHardware()->setConnection(server, server_port);
  nh.initNode();
  nh.subscribe(chatter);

  Serial.println("node initialized");
}

void loop() {
  nh.spinOnce();
  int diff = goal-pos;
  float pwm_value= abs(kp*diff);
  if (pwm_value>255){
    pwm_value=255;
  }
  if (pwm_value<10){
    pwm_value=0;
  }
  if(diff>0){
    digitalWrite(dir,LOW);
    analogWrite(pwm,pwm_value);
  }
  if(diff<0){
    digitalWrite(dir,HIGH);
    analogWrite(pwm,pwm_value);
  }
//  Serial.println(360*pos/(80*256));//debugging
Serial.println(pos);
}
