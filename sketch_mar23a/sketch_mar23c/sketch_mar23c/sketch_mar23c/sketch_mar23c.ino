#include "HX711.h"
HX711 scale(12,13);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(scale.read());
}
