#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); 

int motor1_en1 = 6;
int motor1_in1 = 9;
int motor1_in2 = 8;

int motor2_en2 = 5;
int motor2_in3 = 4;
int motor2_in4 = 3;

int pin_Collect = A0;

int pot_velocity = A5;
int velocity = 50;
int direction_motor = 0;

int interuptor_1 = 7;
int sens_reserve = 0;

String value_Collector_active;
int velocity_Collect = 0;

void setup() {
  Serial.begin(38400);
  Serial.println("Bluetooth send : ");
  
  mySerial.begin(9600);
  mySerial.println("Receiver");
  
  pinMode(motor1_en1, OUTPUT);
  pinMode(motor1_in1, OUTPUT);
  pinMode(motor1_in2, OUTPUT);
  
  pinMode(motor2_en2, OUTPUT);
  pinMode(motor2_in3, OUTPUT);
  pinMode(motor2_in4, OUTPUT);

  pinMode(pin_Collect, INPUT);
  analogWrite(pin_Collect, velocity_Collect);
  
  pinMode(interuptor_1, OUTPUT);
  
  digitalWrite(motor1_in1, HIGH);
  digitalWrite(motor1_in2, LOW);
  
  // put your setup code here, to run once:

}
void loop() {
  int sens_motor = digitalRead(interuptor_1);  
  
  int pot_receive = analogRead(A0);
  direction_motor = map(pot_receive,0,1023, 55, 255);

  if(direction_motor>155){
    direction_motor = direction_motor-155;
    velocity = map(direction_motor, 0,100,55, 255);
    if(sens_motor == 1){
      Motor_forward(motor1_in1, motor1_in2, motor1_en1);
      Motor_backward(motor2_in3, motor2_in4, motor2_en2);
      Serial.print("2 sens: left, velocity: ");
      Serial.println(velocity);      
    }else{
      Motor_forward(motor1_in1, motor1_in2, motor1_en1);
      Motor_forward(motor2_in3, motor2_in4, motor2_en2);
      Serial.print("2 direction: positive, velocity: ");
      Serial.println(velocity);       
    }
  }else if(direction_motor<155){
    direction_motor = 155-direction_motor;   
    velocity = map(direction_motor, 0,100,55, 255); 
    if(sens_motor == 1){
      Motor_backward(motor1_in1, motor1_in2, motor1_en1);
      Motor_forward(motor2_in3, motor2_in4, motor2_en2);
      Serial.print("2 sens: right, velocity: ");
      Serial.println(velocity);  
    }else{    
      Motor_backward(motor1_in1, motor1_in2, motor1_en1);
      Motor_backward(motor2_in3, motor2_in4, motor2_en2);
      Serial.print("2 direction: Negative, velocity: ");
      Serial.println(velocity);         
    }
  }else{
    desable_motor(motor1_in1, motor1_in2);
    desable_motor(motor2_in3, motor2_in4);
    Serial.println("Motor stopped ");
    
  }


  //Част Сбора
  value_Collector_active = mySerial.read();
  if(value_Collector_active!="5"){
    analogWrite(pin_Collect, 0);
  }else if(value_Collector_active=="8"){
    velocity_Collect+= 10;
    if(velocity_Collect>1024){
      velocity_Collect = 1023;
    }
    analogWrite(pin_Collect, velocity_Collect);
  }else if(value_Collector_active=="2"){
    velocity_Collect-= 10;
    if(velocity_Collect<50){
      velocity_Collect = 50;
    }    
    analogWrite(pin_Collect, velocity_Collect);
  }
  Serial.print("\n\n\nValue : ");
  Serial.println(velocity_Collect);
   
   
}

void Motor_forward(int pin_in1, int pin_in2, int pin_motor){
    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, HIGH);
    analogWrite(pin_motor, velocity);  
    delay(5);
}
void Motor_backward(int pin_in1, int pin_in2, int pin_motor){
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, LOW);  
    analogWrite(pin_motor, velocity);
    delay(5);
}
void desable_motor(int pin_in1, int pin_in2){
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, HIGH);
    delay(250);    
}
