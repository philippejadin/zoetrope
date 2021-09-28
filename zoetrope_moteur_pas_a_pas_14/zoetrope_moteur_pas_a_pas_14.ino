/*
  Version du 28 septembre 2021 (on est toujours là)

*/



/* Réglages du zoetrope en lui meme */


int motor_direction = 1;


int inactive_area = 400;


unsigned long flash_duration = 1000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
unsigned long flash_duration_min = 1;
unsigned long flash_duration_max = 5000;


int steps = 200; // Nombre de pas du stepper pour un tour
int frames = 10; // nombre d'images dans l'animation

unsigned long frame_duration = 80000; // en microsecondes
unsigned long frame_duration_min = 40000;
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

  digitalWrite(motordata, HIGH);

}

void loop()
{
  //Serial.println(micros());

  // set motor speed from potentiometer
  val = analogRead(frame_duration_potentiometer_pin);
  int frame_duration_potentiometer = map(val, 0, 1024, 0, 1024);


  if (frame_duration_potentiometer < 512)
  {
    frame_duration = map(frame_duration_potentiometer, 512, 0, frame_duration_max, frame_duration_min);
    motor_direction = 1;
    digitalWrite(motordata, HIGH);
  }
  else
  {
    frame_duration = map(frame_duration_potentiometer, 512, 1024, frame_duration_max, frame_duration_min);
    motor_direction = -1;
    digitalWrite(motordata, LOW);
  }

  //Serial.println(frame_duration);


  // set flash speed from potentiometer

  val = analogRead(flash_duration_pot_pin);
  flash_duration = map(val, 1024, 0, flash_duration_min, flash_duration_max);
  flash_duration = constrain(flash_duration, flash_duration_min, flash_duration_max);



  long step_duration = frame_duration * frames / steps / 2;


  // si ça fait suffisement longtemps qu'on a pas avancé d'un pas
  if (micros() > last_step_on + step_duration) {
    if (step_on) {
      step_on = false;
      last_step_on = micros();
      digitalWrite(motorclock, HIGH);
      debug("step on");
    }
  }

  // si ça fait suffisement longtemps qu'on a pas avancé d'un pas
  if (micros() > last_step_off + step_duration) {
    if (!step_on) {
      step_on = true;
      last_step_off = micros();
      digitalWrite(motorclock, LOW);
      debug("step off");
    }

  }

  // si ça fait suffisement longtemps que le flash est allumé et qu'il est allumé effectivement

  if (micros() > last_flash + flash_duration) {
    if (flash_on) {
      flash_on = false;
      digitalWrite(ledPin, LOW);
      debug("flash off");
    }
  }


  // si ça fait suffisement longtemps que le flash est éteint et qu'il est effectivement éteint

  if (micros() > last_flash + frame_duration / 2) {
    if (!flash_on) {
      last_flash = micros();
      flash_on = true;
      digitalWrite(ledPin, HIGH);
      debug("flash on");
    }
  }

}

int debug(String data)
{
  if (enable_debug)
  {
    Serial.println(data);
  }
}
