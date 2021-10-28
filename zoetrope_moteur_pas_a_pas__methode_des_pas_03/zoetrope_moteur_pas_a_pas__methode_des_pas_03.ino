/*
  Version du 15 septembre 2021 (on est toujours là)


  Cette méthode consiste simplement à ne pas utiliser le temps comme base de calcul, mais les pas qui font tourner le moteur, y compris pour définir la durée du flash
  (comptée en nombre de pas et pas en microsecondes)

  Le code est bcp plus simple à comprendre comme cela !
*/


/* HARDWARE pins */

int motor_clock_pin = 3;      // stepper motor clock
int motor_direction_pin = 2;      // stepper motor direction
int motor_disable_pin = 4;      // pin to disable the motor data
int flash_pin = 5; // pin pour le flash
int step_duration_pin = A0; // potentiometer pin to set motor interval
int flash_duration_pin = A1; // potentiometer pin to set flash duration
int frames_pin = A2; // potentiometer pin to set number of frames per disc



/* Réglages du zoetrope en lui meme */


int inactive_area = 100; // neutral zone of the potentiometer (the "off" position of the motor)

int step_duration = 0; // en microsecondes
int step_duration_min = 0;
int step_duration_max = 2000;

int previous_step_duration = 0;

int step_acceleration = 2; // maximum speed change in one loop itteration,  1 = very slow ramp up and down / 10+ = no visible acceleration control


int steps = 400; // Nombre de pas du stepper pour un tour
int frames = 16; // nombre d'images dans l'animation


int frames_per_disc[] = {10, 16, 20, 24, 32, 40, 64}; // frames per disc available to choose from third potentiometer


int flash_duration = 1; // en nombre de pas
int flash_duration_min = 100;
int flash_duration_max = steps / frames / 2;




/* Variable à usage interne */
int val = 0;
int motor_on = false;
int compteur = 0;
int flash = 0;


void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motor_clock_pin, OUTPUT);
  pinMode(motor_direction_pin, OUTPUT);
  pinMode(flash_pin, OUTPUT);
  pinMode(motor_disable_pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(motor_disable_pin, LOW);

  Serial.begin(9600);

  digitalWrite(motor_direction_pin, HIGH);


  previous_step_duration = step_duration_max;

}

void loop()
{
  // read user input
  /********************* step duration *************************/
  val = analogRead(step_duration_pin);



  if (val < 512 - inactive_area)
  {
    step_duration = map(val, 512 - inactive_area, 0, step_duration_max, step_duration_min);
    digitalWrite(motor_direction_pin, HIGH);
    digitalWrite(motor_disable_pin, LOW);
    motor_on = true;
  }
  else if (val > 512 + inactive_area)
  {
    step_duration = map(val, 512 + inactive_area, 1023, step_duration_max, step_duration_min);
    digitalWrite(motor_direction_pin, LOW);
    digitalWrite(motor_disable_pin, LOW);
    motor_on = true;
  }
  else
  {
    digitalWrite(motor_disable_pin, HIGH);
    motor_on = false;
    previous_step_duration = step_duration_max;
    step_duration = step_duration_max;
  }

  // Serial.println(step_duration);


  // smooth out values to liit acceleration :


  if (abs(previous_step_duration - step_duration) > step_acceleration)
  {
    // Serial.println("too fast");
    if (step_duration > previous_step_duration)
    {
      step_duration = previous_step_duration + step_acceleration;
    }
    if (step_duration < previous_step_duration)
    {
      step_duration = previous_step_duration - step_acceleration;
    }

    previous_step_duration = step_duration;

  }





  /********************* frames per disc  *************************/
  // set frames from frames potentiometer

  val = analogRead(frames_pin);
  int index = map(val, 1023, 0, 0, 6);

  frames =  frames_per_disc[index];


  //Serial.print("frames : ");
  //Serial.println(frames);

  flash_duration_max = steps / frames / 2;

  if (flash_duration_max < 1)
  {
    flash_duration_max = 1;
  }

  //Serial.print("flash_duration_max : ");
  //Serial.println(flash_duration_max);


  /********************* flash duration  *************************/

  val = analogRead(flash_duration_pin);

 
  
  if (val > 1023 - inactive_area)
  {
    flash_duration = 0;
  }
  else
  {
    flash_duration = map(val, 1023 - inactive_area, 0, 1, flash_duration_max);
  }

  //Serial.print("flash_duration : ");
  //Serial.println(flash_duration);

  if (motor_on)
  {
    // handle flash on
    if (compteur >= steps / frames)
    {
      flash = 0;
      compteur = 0;
      digitalWrite(flash_pin, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
      //Serial.println("-------------flash on!-----------------");
    }


    // handle flash off
    if (flash >= flash_duration)
    {
      digitalWrite(flash_pin, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      flash = 0;
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
    flash = 0;
    compteur = 0;

    digitalWrite(flash_pin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delayMicroseconds(flash_duration * 20);
    digitalWrite(flash_pin, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    delayMicroseconds(1000);
  }
}
