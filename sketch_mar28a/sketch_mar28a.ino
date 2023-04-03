#define echoPin A0 
#define echoPin A0
#define  trigPin A1 

#define  MotorR1 8
#define MotorR2 9
#define MotorRE 10  
#define  MotorL1 6
#define MotorL2 7
#define MotorLE 5


byte timer=0;
long  sure, uzaklik;

void setup() {


 
  pinMode(echoPin,  INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(MotorL1, OUTPUT);
  pinMode(MotorL2,  OUTPUT);
  pinMode(MotorLE, OUTPUT); 
  pinMode(MotorR1,  OUTPUT);
  pinMode(MotorR2, OUTPUT);
  pinMode(MotorRE, OUTPUT);
  pinMode(11,  INPUT);
  pinMode(12, INPUT);

 pinMode(4, OUTPUT);
  Serial.begin(9600);

}

void  loop() {


 
  if(digitalRead(11)==1 && digitalRead(12)==0)
  {
   sag(); // ileri git
    digitalWrite(4,LOW);
  }
  if(digitalRead(11)==0&&  digitalRead(12)==1)
  {
   sol(); // ileri git
    digitalWrite(4,LOW);
  }
  if(digitalRead(11)==1 && digitalRead(12)==1)
  {
   ileri(); // ileri  git
    digitalWrite(4,LOW);
  }
  if(digitalRead(11)==0 && digitalRead(12)==0)
  {
    timer++;
    if (timer<100)
    {
   sol(); // ileri git
    digitalWrite(4,LOW);
    timer=0;
    } 
    
  }
  
}



void  ileri(){  

  digitalWrite(MotorR1,  HIGH); 
  digitalWrite(MotorR2, LOW); 
  analogWrite(MotorRE, 100); 

  digitalWrite(MotorL1, HIGH);
  digitalWrite(MotorL2,  LOW); 
  analogWrite(MotorLE, 100); 
  
}


void sag(){ 
  digitalWrite(MotorR1, HIGH); 
  digitalWrite(MotorR2, LOW); 
  analogWrite(MotorRE, 0); 

  digitalWrite(MotorL1,  HIGH); 
  digitalWrite(MotorL2, LOW); 
  analogWrite(MotorLE, 100); 
  
  
}

void sol(){ 

  digitalWrite(MotorR1, HIGH); 
  digitalWrite(MotorR2,  LOW); 
  analogWrite(MotorRE, 100); 

  digitalWrite(MotorL1, LOW); 
  digitalWrite(MotorL2, HIGH); 
  analogWrite(MotorLE,  0); 
  
  
}


void geri(){ 
  digitalWrite(MotorR1, LOW); 
  digitalWrite(MotorR2, HIGH); 
  analogWrite(MotorRE, 150); 

  digitalWrite(MotorL1, LOW); 
  digitalWrite(MotorL2,  HIGH); 
  analogWrite(MotorLE, 150); 
  
}
void dur(){  

  digitalWrite(MotorR1, HIGH); 
  digitalWrite(MotorR2, LOW); 
  analogWrite(MotorRE,  0); 

  digitalWrite(MotorL1, HIGH); 
  digitalWrite(MotorL2, LOW); 
  analogWrite(MotorLE, 0); 
}
