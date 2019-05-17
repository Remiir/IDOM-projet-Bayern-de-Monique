#include <MotorWheel.h>
#include <Omni3WD.h>
#include <Omni4WD.h>
#include <PID_Beta6.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>

const int avant = 6;
const int cote = 13;

irqISR(irq1,isr1); 
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3); 
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);

Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

void setup() {
  // initialise la communication série
  Serial.begin(9600);

  
 TCCR1B=TCCR1B&0xf8|0x01; // Pin9,Pin10 PWM 31250Hz
 TCCR2B=TCCR2B&0xf8|0x01; // Pin3,Pin11 PWM 31250Hz



 Omni.PIDEnable(0.31,0.01,0,10); //PID enable
}

long fonctioncm(const int a){
    // déclaration des variable pour le temps entre deux ondes et la distance
  long duration, cm;

  // Morceau de code permettant de réinitialiser le capteur pour l’envoie d’une nouvelle onde
  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);
  delayMicroseconds(2);
  digitalWrite(a, HIGH);
  delayMicroseconds(5);
  digitalWrite(a, LOW);

  // on utilise pulseIn pour recevoir une durée de type long
  pinMode(a, INPUT);
  duration = pulseIn(a, HIGH);

  // On converti la durée en distance grâce à une fonction que vous trouverez plus bas
  cm = microsecondsToCentimeters(duration); 
  return cm;

    Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(10);
}

void loop()
{

  while (fonctioncm(6) > 30 && fonctioncm(13) > 30){
 Omni.setCarAdvance();
 Omni.setCarSpeedMMPS(50,500);
 Omni.delayMS(10,false);
}
  Omni.setCarStop();
  Omni.setCarRotateRight(30);
  Omni.setCarSpeedMMPS(30,400);
  delay(850);
  Omni.setCarStop();
  Omni.delayMS(900);
//Omni.cercle(wheel1,wheel2, wheel3,wheel4, 10, 500);


}

long microsecondsToCentimeters(long microseconds)
{
  // Comme expliqué auparavant nous divisons la durée par la vitesse et par 2 pour avoir la distance d’un aller simple.
  return microseconds / 29 / 2;
}
