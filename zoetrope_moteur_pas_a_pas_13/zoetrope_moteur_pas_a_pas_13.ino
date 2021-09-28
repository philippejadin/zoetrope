/*
  Version du 28 septembre 2021 (on est toujours là)

*/



/* Réglages du zoetrope en lui meme */

long frame_duration_min = 50000;
long frame_duration_max = 150000;

int motor_direction = 1;


int inactive_area = 400;


long flash_duration = 1000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
long flash_duration_min = 1;
long flash_duration_max = 5000;


long steps = 200; // Nombre de pas du stepper pour un tour
long frames = 10; // nombre d'images dans l'animation

long frame_duration = 80000; // en microsecondes





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

  /*
    for (int i = 0; i < steps; i++) {
      step();
      delay(10);
      //Serial.println("motor");
    }

    delay(5000);

  */

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



  long step_duration = frame_duration * frames / steps;


  // si ça fait suffisement longtemps qu'on a pas avancé d'un pas
  if (micros() > last_step_on + step_duration / 2) {
    if (step_on) {
      step_on = false;
      last_step_on = micros();


      digitalWrite(motorclock, HIGH);

      debug("step on");
    }
  }

  // si ça fait suffisement longtemps qu'on a pas avancé d'un pas
  if (micros() > last_step_off + step_duration / 2) {
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

int debug(String data)
{
  if (enable_debug)
  {
    Serial.println(data);
  }
}


/*
  void step()
  {
  digitalWrite(motordata, HIGH);
  digitalWrite(motorclock, HIGH);
  delayMicroseconds(1000);
  digitalWrite(motorclock, LOW);
  delayMicroseconds(1000);
  }*/
