/*
  Version du 15 septembre 2021 (on est toujours là)


  Cette méthode consiste simplement à ne pas utiliser le temps comme base de calcul, mais les pas qui font tourner le moteur, y compris pour définir la durée du flash
  (comptée en nombre de pas et pas en microsecondes)

  Le code est bcp plus simple à comprendre comme cela !
*/

// this requires the smoothed library
#include <Smoothed.h>

/* HARDWARE pins */

int motor_clock_pin = 3;      // stepper motor clock
int motor_direction_pin = 2;      // stepper motor direction
int motor_disable_pin = 4;      // pin to disable the motor data
int flash_pin = 5; // pin pour le flash
int step_duration_pin = A0; // potentiometer pin to set motor interval
int flash_duration_pin = A1; // potentiometer pint to set flash duration



/* Réglages du zoetrope en lui meme */


int inactive_area = 100; // neutral zone of the potentiometer (the "off" position of the motor)

int step_duration; // en microsecondes
int step_duration_min = 10;
int step_duration_max = 2000;


int flash_duration = 1; // en nombre de pas
int flash_duration_min = 0;
int flash_duration_max = 20;


int steps = 400; // Nombre de pas du stepper pour un tour
int frames = 10; // nombre d'images dans l'animation



/* Variable à usage interne */
int val = 0;
int motor_on = false;
int compteur = 0;
int flash = 0;

Smoothed <int> step_duration_pot;
Smoothed <int> flash_duration_pot;


void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motor_clock_pin, OUTPUT);
  pinMode(motor_direction_pin, OUTPUT);
  pinMode(flash_pin, OUTPUT);
  pinMode(motor_disable_pin, OUTPUT);

  digitalWrite(motor_disable_pin, LOW);

  Serial.begin(9600);

  digitalWrite(motor_direction_pin, HIGH);

  step_duration_pot.begin(SMOOTHED_EXPONENTIAL, 50);
  flash_duration_pot.begin(SMOOTHED_EXPONENTIAL, 50);


}

void loop()
{


  // read user input
  // set motor speed from potentiometer
  val = analogRead(step_duration_pin);
  val = constrain(val, 0, 900);

  step_duration_pot.add(val);

  val = step_duration_pot.get();

  if (val < 512 - inactive_area)
  {
    step_duration = map(val, 512 - inactive_area, 0, step_duration_max, step_duration_min);
    digitalWrite(motor_direction_pin, HIGH);
    digitalWrite(motor_disable_pin, LOW);
    motor_on = true;
  }
  else if (val > 512 + inactive_area)
  {
    step_duration = map(val, 512 + inactive_area, 900, step_duration_max, step_duration_min);
    digitalWrite(motor_direction_pin, LOW);
    digitalWrite(motor_disable_pin, LOW);
    motor_on = true;
  }
  else
  {
    digitalWrite(motor_disable_pin, HIGH);
    motor_on = false;
  }


  // set flash speed from potentiometer

  val = analogRead(flash_duration_pin);
  val = constrain(val, 0, 1023);

  flash_duration_pot.add(val);
  val = flash_duration_pot.get();

  flash_duration = map(val, 1000, 0, flash_duration_min, flash_duration_max);




  if (motor_on)
  {
/*
    Serial.print("calc : ");  
    Serial.println(steps / frames);
    */
    // handle steps
    if (compteur == steps / frames)
    {
      flash = 0;
      compteur = 0;
      digitalWrite(flash_pin, HIGH);
      //Serial.println("-------------flash on!-----------------");
    }


    
    if (flash >= flash_duration)
    {
      digitalWrite(flash_pin, LOW);
      //Serial.println("flash off!");

    }


    compteur ++;
    flash ++;

    digitalWrite(motor_clock_pin, HIGH);
    delayMicroseconds(step_duration);
    digitalWrite(motor_clock_pin, LOW);

/*
    Serial.print("compteur : ");
    Serial.println(compteur);

    Serial.print("flash : ");
    Serial.println(flash);

    Serial.print("flash duration: ");
    Serial.println(flash_duration);
    */

    //delay(300);


  }
  else // mode standby
  {

    digitalWrite(flash_pin, HIGH);
    delayMicroseconds(flash_duration * 20);
    digitalWrite(flash_pin, LOW);
    delayMicroseconds(1000);
  }
}
