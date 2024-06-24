#include <Stepper.h>

#define pinSubido 2
#define pinDescido 4
#define pinSensor A0


const int stepsPerRevelution =2038; // auto adjust to motor

//start Lib on pins 8,9,10,11
Stepper myStepper(stepsPerRevelution,8,10,9,11);

int inicial_stepCount=0;
int max_stepCount;
bool setup_dist = false;
bool stop=false;
int lightmap;

int stopsafe(){
  myStepper.step(0);
  Serial.println("Safe STOP!!");
  delay(1000);
}


void setup() {
  Serial.begin(9600);
  pinMode(pinSubido, INPUT);
  pinMode(pinDescido, INPUT);
  pinMode(pinSensor, INPUT);

}

void loop() {
  float leitura_sensor = analogRead(pinSensor);

  int leitura_up = digitalRead(pinSubido);
  int leitura_down = digitalRead(pinDescido);
  
  if (setup_dist == false){
    Serial.println("Runing setup.");
    delay(10);
    if (leitura_down == 0 && leitura_up ==0 && stop == false){
      Serial.println("Window going up.");
      myStepper.setSpeed(100);
      myStepper.step(1);
      delay(10);
    }
    if(leitura_up == 1){
      stop = true;
      Serial.println("STOP");
      delay (1000);
    }
    if(stop == true){
      Serial.println("Window Going Down.");
      myStepper.setSpeed(100);
      myStepper.step(-1);
      delay(10);
      inicial_stepCount++;
      Serial.println(inicial_stepCount);
      
      if (leitura_down == 1){
        Serial.println(inicial_stepCount);
        max_stepCount= inicial_stepCount;
        setup_dist = true;
        Serial.print("Max step count:");
        Serial.println(max_stepCount);
        inicial_stepCount =0;

        Serial.println("Setup Complete");
        stop=false;
        delay(3000);
        
        
      }
    }
  } 

  if (setup_dist ==true){
    const int lightmap = map(leitura_sensor,0,1023,0,max_stepCount);
    
    Serial.println("----------------------------");
    Serial.print("Leitura fim de curso de fundo: ");
    Serial.println(leitura_down);
    Serial.print("Leitura fim de curso de topo: ");
    Serial.println(leitura_up);
    Serial.print("Max step count: ");
    Serial.println(max_stepCount);
    Serial.print("Leitura sensor: ");
    Serial.println(leitura_sensor);
    Serial.print("Light map: ");
    Serial.println(lightmap);
    if (leitura_up == 1 || leitura_down == 1 || inicial_stepCount == max_stepCount){
      stopsafe();
      delay(1000);
    }

    else if (leitura_down == 0 && leitura_up == 0) {
      if (inicial_stepCount < lightmap){
        myStepper.step(1);
        inicial_stepCount++;
        Serial.print("Count = ");
        Serial.print(inicial_stepCount);

        delay(10);
      }
      if (inicial_stepCount > lightmap){
        myStepper.step(-1);
        inicial_stepCount--;
        Serial.print("Count = ");
        Serial.print(inicial_stepCount);
        delay(10);
      }
      delay(1000);
      
    }
  }
}
