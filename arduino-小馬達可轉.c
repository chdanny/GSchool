int motorIn3 = 5;     
int motorIn4 = 6;
void setup(){
  Serial.begin(9600);
  pinMode(motorIn3, OUTPUT); 
  pinMode(motorIn4, OUTPUT);
  }
void loop() {
analogWrite(motorIn3, 60); //給予馬達訊號是150~255之間
analogWrite(motorIn4, 0);
delay(3000);
//analogWrite(motorIn3, 0); //給予馬達訊號是150~255之間
//analogWrite(motorIn4, 40);
//delay(3000);
}

///==========================================================

/*void setup() {
 Serial.begin(9600);
}
void loop() {
 if (Serial.available() > 0) {
 int c = Serial.read();
 Serial.print("I received: ");
 Serial.println(c);
 }
}
*/

int motorIn1 = 5; //馬達    
int motorIn2 = 6; //馬達
int motorIn3 = 9; //磁鐵    
int motorIn4 = 10; //磁鐵

void setup(){
  Serial.begin(9600);
  pinMode(motorIn1, OUTPUT); 
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn3, OUTPUT); 
  pinMode(motorIn4, OUTPUT);
  }
void loop() {
/*analogWrite(motorIn1, 0); //給予馬達訊號是150~255之間
analogWrite(motorIn2, 255);
delay(30000);
analogWrite(motorIn1, 255); //給予馬達訊號是150~255之間+
analogWrite(motorIn2, 0);
delay(30000);
*/
analogWrite(motorIn3, 255); //磁鐵吸住是255
analogWrite(motorIn4, 0);
delay(3000);
analogWrite(motorIn3, 0); //磁鐵不要吸
analogWrite(motorIn4, 0);
delay(3000);
}

//=========================================================================
//=========================================================================




int motorIn1 = 5; //馬達    
int motorIn2 = 6; //馬達
int motorIn3 = 9; //磁鐵    
int motorIn4 = 10; //磁鐵
//void MagYes();

void setup(){
  Serial.begin(9600);
  pinMode(motorIn1, OUTPUT); 
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn3, OUTPUT); 
  pinMode(motorIn4, OUTPUT);
  }
void loop() {

MotorReleaseT();//放線

//MotorReturnT();//收回線

//MagYes();//磁鐵吸住

//MagNo();//磁鐵不要吸


}// end of void loop()

void MagYes(){
  analogWrite(motorIn3, 255); //磁鐵吸住是255
  analogWrite(motorIn4, 0);
  delay(3000);
  }
void MagNo(){
  analogWrite(motorIn3, 0); //磁鐵不要吸
  analogWrite(motorIn4, 0);
  delay(3000);
  }
void MotorReleaseT(){  
  analogWrite(motorIn1, 0); //給予馬達訊號是150~255之間 放線
  analogWrite(motorIn2, 255);
  delay(30000);  
  }
void MotorReturnT(){  
  analogWrite(motorIn1, 255); //給予馬達訊號是150~255之間+
  analogWrite(motorIn2, 0);
  delay(30000);
  }  

  //=========================================================================
//=========================================================================
int buttonState = 0;         // 存放按鈕狀態
int old_buttonState = 0;     // 存放按鈕上一次的狀態
int MagState = 0;            // 存放LED狀態，0:關;1:亮


int motorIn1 = 5; //馬達    
int motorIn2 = 6; //馬達
int motorIn3 = 9; //磁鐵    
int motorIn4 = 10; //磁鐵
//void MagYes();

void setup(){
  Serial.begin(9600);
  pinMode(motorIn1, OUTPUT); 
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn3, OUTPUT); 
  pinMode(motorIn4, OUTPUT);
  pinMode(11, INPUT);//button
  pinMode(12, INPUT);//button
  pinMode(13, INPUT);//button
  }
void loop() {



if(digitalRead(11)==HIGH){
   Serial.println("11ok");
   MotorReleaseT();//放線
}else if(digitalRead(12)==HIGH){
   Serial.println("12ok");
   MotorReturnT();//收回線
}else{
  MotorSTOP();
  }

  buttonState = digitalRead(13); //讀取按鈕的狀態
  if ((buttonState == HIGH) && (old_buttonState == LOW)) {     //假如按鈕是從斷開的情況下被按下時
    MagState = 1 - MagState;         //反置 LED 狀態
    delay(300);
    Serial.println("13ok");
  }  
  
  old_buttonState == buttonState;  //把這次的按鈕狀態存起來供下次判斷

  if (MagState == 1) {
    MagYes();//磁鐵吸住
     Serial.println("yes");
  }
  else { 
    MagNo();//磁鐵不要吸
     Serial.println("no");
  } 


}// end of void loop()

void MagYes(){
  analogWrite(motorIn3, 255); //磁鐵吸住是255
  analogWrite(motorIn4, 0);
  //delay(3000);
  }
void MagNo(){
  analogWrite(motorIn3, 0); //磁鐵不要吸
  analogWrite(motorIn4, 0);
 // delay(3000);
  }
void MotorReleaseT(){  
  analogWrite(motorIn1, 0); //給予馬達訊號是150~255之間 放線
  analogWrite(motorIn2, 255);
 // delay(30000);  
  }
void MotorReturnT(){  
  analogWrite(motorIn1, 255); //給予馬達訊號是150~255之間+
  analogWrite(motorIn2, 0);
 // delay(30000);
  }  
void MotorSTOP(){  
  analogWrite(motorIn1, 0); //給予馬達訊號是150~255之間+
  analogWrite(motorIn2, 0);
  //delay(3000);
  }    
  //=========================================================================
//=========================================================================

