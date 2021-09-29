/*
  Version du 28 septembre 2021 (on est toujours là)

*/



/* Réglages du zoetrope en lui meme */


int motor_direction = 1;


int inactive_area = 400;


unsigned long flash_duration = 1000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
unsigned long flash_duration_min = 1;
unsigned long flash_duration_max = 5000;


unsigned long steps = 200; // Nombre de pas du stepper pour un tour
unsigned long frames = 10; // nombre d'images dans l'animation

unsigned long frame_duration = 80000; // en microsecondes
unsigned long frame_duration_min = 60000;
unsigned long frame_duration_max = 350000;





/* HARDWARE pins */

int motorclock = 3;      // select the pin for the stepper motor clock
int motordata = 6;      // select the pin for the stepper motor data

int motordisable = 10;      // select the pin to disable the motor data

int ledPin = 8; // pin pour le flash

int frame_duration_potentiometer_pin = 0; // potentiometer pint to set motor interval
int flash_duration_pot_pin = 1; // potentiometer pint to set flash duration


/* setup */
int val = 0;
int compteur = 0;

int flash_on = false;
int step_on = false;

unsigned long last_step;
unsigned long last_step_on;
unsigned long last_step_off;
unsigned long last_flash;

int enable_debug = false;





void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motorclock, OUTPUT);
  pinMode(motordata, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motordisable, OUTPUT);

  digitalWrite(motordisable, LOW);

  Serial.begin(9600);

  digitalWrite(motordata, LOW);

}

void loop()
{


  digitalWrite(ledPin, LOW);
  for (int i = 0; i < 20; i++)
  {
    digitalWrite(motorclock, LOW);
    delayMicroseconds(700);
    digitalWrite(motorclock, HIGH);
    delayMicroseconds(700);
  }

  digitalWrite(ledPin, HIGH);
  delayMicroseconds(1000);



}
