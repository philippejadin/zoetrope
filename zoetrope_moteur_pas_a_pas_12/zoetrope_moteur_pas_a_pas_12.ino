/*
  Version du 28 septembre 2021 (on est toujours là)

*/



/* Réglages du zoetrope en lui meme */
int motor_interval = 2400; // en microsecondes +- minimum 1300

int motor_interval_min = 900;
int motor_interval_max = 5000;

int motor_direction = 1;


int inactive_area = 400;


long flash_duration = 10000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
long flash_duration_min = 10;
long flash_duration_max = 2500;


long steps = 800; // Nombre de pas du stepper pour un tour
long frames = 10; // nombre d'images dans l'animation

long frame_duration = 1000000; // en microsecondes

long step_duration = frame_duration * frames / steps;



/* HARDWARE pins */

int motorclock = 3;      // select the pin for the stepper motor clock
int motordata = 6;      // select the pin for the stepper motor data

int motordisable = 10;      // select the pin to disable the motor data

int ledPin = 8; // pin pour le flash

int motor_interval_pot_pin = 0; // potentiometer pint to set motor interval
int flash_duration_pot_pin = 1; // potentiometer pint to set flash duration


/* setup */
int val = 0;
int compteur = 0;

int flash_on = false;

unsigned long last_step;
unsigned long last_flash;





void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motorclock, OUTPUT);
  pinMode(motordata, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motordisable, OUTPUT);

  digitalWrite(motordisable, LOW);

  Serial.begin(9600);

  Serial.println(step_duration);

}

void loop()
{


  for (int i = 0; i < 800; i++) {
    step();
    delay(5);
    Serial.println("motor");
  }

delay(5000);


  //Serial.println(micros());
  /*
    // set motor speed from potentiometer
    val = analogRead(motor_interval_pot_pin);
    int motorspeed = map(val, 0, 1024, 0, 1024);


    if (motorspeed < 512)
    {
      motor_interval = map(motorspeed, 512, 0, motor_interval_max, motor_interval_min);
      motor_direction = 1;
    }
    else
    {
      motor_interval = map(motorspeed, 512, 1024, motor_interval_max, motor_interval_min);
      motor_direction = -1;
    }

  */

  // set flash speed from potentiometer
  /*
    val = analogRead(flash_duration_pot_pin);
    flash_duration = map(val, 1024, 0, flash_duration_min, flash_duration_max);
    flash_duration = constrain(flash_duration, flash_duration_min, flash_duration_max);
  */


/*
  // si ça fait suffisement longtemps qu'on a pas avancé d'un pas
  if (micros() > last_step + step_duration) {
    last_step = micros();
    step();
    //Serial.println("step");
  }

  // si ça fait suffisement longtemps que le flash est allumé et qu'il est allumé effectivement

  if (micros() > last_flash + flash_duration) {
    if (flash_on) {
      last_flash = micros();
      flash_on = false;
      digitalWrite(ledPin, LOW);
      //Serial.println("flash off");
    }
  }


  // si ça fait suffisement longtemps que le flash est éteint et qu'il est effectivement éteint

  if (micros() > last_flash + frame_duration) {
    if (!flash_on) {
      flash_on = true;
      digitalWrite(ledPin, HIGH);
      //Serial.println("flash on");
    }
  }

*/

  //delay(1000);

  /*

    if (motor_interval < motor_interval_max - inactive_area)
    {


      // enable stepper output
      digitalWrite(motordisable, LOW);

      if (motor_direction == 1)
      {
        digitalWrite(motordata, HIGH);
      }
      else
      {
        digitalWrite(motordata, LOW);
      }



      digitalWrite(motorclock, HIGH);
      delayMicroseconds(motor_interval / 2);
      digitalWrite(motorclock, LOW);
      delayMicroseconds(motor_interval / 2);


      compteur = compteur + 1;
      // Serial.println(compteur % steps);


      if (compteur == (steps / frames))
      {
        digitalWrite(ledPin, HIGH);
        delayMicroseconds(flash_duration);
        digitalWrite(ledPin, LOW);
        compteur = 0;
      }


  */
  /*
    }
    else // on allume gentiment la led pour y voir quelque chose
    {

      val = analogRead(flash_duration_pot_pin);
      flash_duration = map(val, 1024, 0, flash_duration_min, flash_duration_max / 5);
      flash_duration = constrain(flash_duration, flash_duration_min, flash_duration_max / 5);



      digitalWrite(ledPin, HIGH);
      delayMicroseconds(flash_duration);
      digitalWrite(ledPin, LOW);
      delayMicroseconds(1500);


      // disable stepper output
      digitalWrite(motordisable, HIGH);

    }
  */



}


void step()
{
  digitalWrite(motorclock, HIGH);
  delayMicroseconds(10000);
  digitalWrite(motorclock, LOW);
  delayMicroseconds(10000);
}
