#include <MotorWheel.h>
#include <Omni3WD.h>
#include <Omni4WD.h>
#include <PID_Beta6.h>
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>

const int avant = 13;
const int droite = 6;
const int gauche = 0;

irqISR(irq1, isr1);
MotorWheel wheel1(3, 2, 4, 5, &irq1);

irqISR(irq2, isr2);
MotorWheel wheel2(11, 12, 14, 15, &irq2);

irqISR(irq3, isr3);
MotorWheel wheel3(9, 8, 16, 17, &irq3);

irqISR(irq4, isr4);
MotorWheel wheel4(10, 7, 18, 19, &irq4);

Omni4WD Omni(&wheel1, &wheel2, &wheel3, &wheel4);

void setup() {
  // initialise la communication série
  Serial.begin(9600);


  TCCR1B = TCCR1B & 0xf8 | 0x01; // Pin9,Pin10 PWM 31250Hz
  TCCR2B = TCCR2B & 0xf8 | 0x01; // Pin3,Pin11 PWM 31250Hz



  Omni.PIDEnable(0.31, 0.01, 0, 10); //PID enable
}

long fonctioncm(const int a) {
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
void rd(int speedMMPS, int duration, int uptime) {
  Omni.setCarRotateRight(speedMMPS);
  Omni.setCarSpeedMMPS(speedMMPS, uptime);
  delay(duration);
  Omni.setCarSlow2Stop(uptime);
}
void rg(int speedMMPS, int duration, int uptime) {
  Omni.setCarRotateLeft(speedMMPS);
  Omni.setCarSpeedMMPS(speedMMPS, uptime);
  delay(duration);
  Omni.setCarSlow2Stop(uptime);
}
void loop()
{

  while (fonctioncm(avant) > 30 && fonctioncm(droite) < 30) {
    Omni.setCarAdvance(25);
    Omni.setCarSpeedMMPS(25, 500);
    Omni.delayMS(10, false);
  }
  Omni.setCarStop();
  delay(1000);
  if (fonctioncm(droite) > 30) {
    rd(25, 0, 400);
    Omni.setCarAdvance(30);
    Omni.setCarSpeedMMPS(30, 400);
    delay(250);
    Omni.setCarSlow2Stop(400);
  }
  if (fonctioncm(gauche) > 30) {
    rg(25, 0, 400);
    Omni.setCarAdvance(30);
    Omni.setCarSpeedMMPS(30, 400);
    delay(250);
    Omni.setCarSlow2Stop(400);
  }
  else {
    rd(25, 0, 400);
    rd(25, 0, 400);
    Omni.setCarAdvance(30);
    Omni.setCarSpeedMMPS(30, 400);
    delay(250);
    Omni.setCarSlow2Stop(400);
  }
}

long microsecondsToCentimeters(long microseconds)
{
  // Comme expliqué auparavant nous divisons la durée par la vitesse et par 2 pour avoir la distance d’un aller simple.
  return microseconds / 29 / 2;
}
