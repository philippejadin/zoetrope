/*
  Version du 28 septembre 2021 (on est toujours là)

*/



int enable_debug = false;



/* Réglages du zoetrope en lui meme */


int motor_direction = 1;


int inactive_area = 50;



unsigned long frame_duration = 80000; // en microsecondes
unsigned long frame_duration_min = 20000;
unsigned long frame_duration_max = 100000;


unsigned long flash_duration = 1000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
unsigned long flash_duration_min = 100;
unsigned long flash_duration_max = 5000;

unsigned long potentiometer_interval = 50000; // toutes les 50 msecs = 20 x par seconde
unsigned long potentiometer_next = 0;

unsigned long steps = 400; // Nombre de pas du stepper pour un tour
unsigned long frames = 10; // nombre d'images dans l'animation






/* HARDWARE pins */

int motor_clock_pin = 3;      // select the pin for the stepper motor clock
int motor_direction_pin = 6;      // select the pin for the stepper motor data

int motor_disable_pin = 10;      // select the pin to disable the motor data

int ledPin = 8; // pin pour le flash

int frame_duration_potentiometer_pin = 0; // potentiometer pint to set motor interval
int flash_duration_pot_pin = 1; // potentiometer pint to set flash duration
unsigned long frame_duration_potentiometer = 0;

/* setup */
int val = 0;
int compteur = 0;

int flash_on = false;
int step_on = false;
int motor_on = false;

unsigned long last_step;
unsigned long last_step_on;
unsigned long last_step_off;
unsigned long next_flash;
unsigned long micro;
unsigned long step_interval;







void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motor_clock_pin, OUTPUT);
  pinMode(motor_direction_pin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(motor_disable_pin, OUTPUT);

  digitalWrite(motor_disable_pin, HIGH);

  Serial.begin(9600);

  digitalWrite(motor_direction_pin, HIGH);

}

void loop()
{

  readPotentiometers();


  if (motor_on)
  {

    micro = micros();

    // si ça fait suffisement longtemps qu'on a pas avancé d'un pas
    if (micro >= last_step_on) {
      if (step_on) {
        step_on = false;
        last_step_on += step_interval;
        digitalWrite(motor_clock_pin, HIGH);
        debug("step on");
      }
      else
      {
        step_on = true;
        last_step_off += step_interval;
        digitalWrite(motor_clock_pin, LOW);
        debug("step off");
      }
    }




    // si ça fait suffisement longtemps que le flash est éteint et qu'il est effectivement éteint
    // un fash à chaque frame
    if (micro >= next_flash + frame_duration) {
      if (!flash_on) {
        flash_on = true;
        digitalWrite(ledPin, HIGH);
        next_flash += frame_duration;
        debug("flash on");
      }
    }


    // si ça fait suffisement longtemps que le flash est allumé et qu'il est allumé effectivement, on éteint

    if (micro >= next_flash + flash_duration) {
      if (flash_on) {
        flash_on = false;
        digitalWrite(ledPin, LOW);
        debug("flash off");
        //next_flash += flash_duration;
      }
    }



  }
  else // mode standby
  {
    digitalWrite(ledPin, HIGH);
    delayMicroseconds(flash_duration);
    digitalWrite(ledPin, LOW);
    delayMicroseconds(10000);

  }

}


void readPotentiometers()
{

  if (micros() >= potentiometer_next)
  {
    potentiometer_next = micros() + potentiometer_interval;

    // set motor speed from potentiometer
    frame_duration_potentiometer = analogRead(frame_duration_potentiometer_pin);
    frame_duration_potentiometer = constrain(frame_duration_potentiometer, 0, 900);

   
    if (frame_duration_potentiometer < 512 - inactive_area - 50)
    {
      frame_duration = map(frame_duration_potentiometer, 512 - inactive_area, 0, frame_duration_max, frame_duration_min);
      motor_direction = 1;
      digitalWrite(motor_direction_pin, HIGH);
      digitalWrite(motor_disable_pin, LOW);
      motor_on = true;
    }
    else if (frame_duration_potentiometer > 512 + inactive_area + 50)
    {
      frame_duration = map(frame_duration_potentiometer, 512 + inactive_area, 900, frame_duration_max, frame_duration_min);
      motor_direction = -1;
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

    val = analogRead(flash_duration_pot_pin);
    val = constrain(val, 0, 900);
    flash_duration = map(val, 900, 0, flash_duration_min, flash_duration_max);

    //flash_duration = 1000;



    step_interval = frame_duration * frames / steps;
  }
}


void debug(String data)
{
  if (enable_debug)
  {
    Serial.println(data);
    //delay(500);
  }
}
