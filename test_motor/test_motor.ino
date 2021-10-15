/*
  Version du 1er octobre 2021

  Le flash s'allume et s'éteint dans la même boucle, plus précis pour le flash, moins précis pour les steps
  Essai de mettre un avant arrière pour le potentiomètre vitesse
  Désactive le moteur quand il ne tourne pas (évite de chauffer)

*/



/* Réglages du zoetrope en lui meme */
unsigned long motor_interval = 2400; // en microsecondes +- minimum 1300

unsigned long motor_interval_min = 1300;
unsigned long motor_interval_max = 8000;


unsigned long inactive_area = 400;


unsigned long flash_duration = 1000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
unsigned long flash_duration_min = 0;
unsigned long flash_duration_max = 3000;


int steps = 200; // Nombre de pas du stepper
int frames = 10; // nombre d'images dans l'animation
unsigned long micro;


/* HARDWARE pins */

int motorclock = 3;      // select the pin for the stepper motor clock
int motordata = 6;      // select the pin for the stepper motor data

int motordisable = 10;      // select the pin to disable the motor data

int ledPin = 8; // pin pour le flash

int motor_interval_pot_pin = 0; // potentiometer pint to set motor interval
int flash_duration_pot_pin = 1; // potentiometer pint to set flash duration


unsigned long last_flash_on = 0;


/* setup */
int val = 0;
int compteur = 0;



void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motorclock, OUTPUT);
  pinMode(motordata, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motordisable, OUTPUT);
  digitalWrite(motordisable, LOW);

  Serial.begin(9600);

}

void loop()
{


  digitalWrite(motordata, LOW);


  
  digitalWrite(motorclock, HIGH);
  delayMicroseconds(200);
  digitalWrite(motorclock, LOW);
  delayMicroseconds(200);


  delayMicroseconds(100000);


}
