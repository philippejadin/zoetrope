/*
Version du 30 aout 2013
 
 Essai de mettre un avant arrière pour le potentiomètre vitesse
 Désactive le moteur quand il ne tourne pas (évite de chauffer)
 
 */



/* Réglages du zoetrope en lui meme */
int motor_interval = 2400; // en microsecondes +- minimum 1300 

int motor_interval_min = 900;
int motor_interval_max = 5000;


int inactive_area = 400;


int flash_duration = 1000; // motion blur : 1000 : nickel, 1500 plus lumineux, jusque 5500 (ultra blanc)
int flash_duration_min = 10;
int flash_duration_max = 2500;


int steps = 200; // Nombre de pas du stepper
int frames = 20; // nombre d'images dans l'animation


/* HARDWARE pins */
/*
int motorclock = 4;      // select the pin for the stepper motor clock
 int motordata = 7;      // select the pin for the stepper motor data
 */


int motorclock = 3;      // select the pin for the stepper motor clock
int motordata = 6;      // select the pin for the stepper motor data

int motordisable = 10;      // select the pin to disable the motor data

int ledPin = 8; // pin pour le flash 

int motor_interval_pot_pin = 0; // potentiometer pint to set motor interval
int flash_duration_pot_pin = 1; // potentiometer pint to set flash duration


/* setup */
int val = 0;
int compteur = 0;



void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(motorclock, OUTPUT);  
  pinMode(motordata, OUTPUT);  
  pinMode(ledPin, OUTPUT); 
  pinMode(motordisable, OUTPUT); 

  Serial.begin(9600);  

  /*
  for (int i=0; i<steps; i++)
   {
   digitalWrite(motorclock, HIGH); 
   delayMicroseconds(motor_interval / 2);
   digitalWrite(motorclock, LOW); 
   delayMicroseconds(motor_interval / 2);
   }
   
   */


}

void loop() 
{

  // set motor speed from potentiometer
  val = analogRead(motor_interval_pot_pin); 
  int motorspeed = map(val, 0, 1024, 0, 1024);


  if (motorspeed < 512)
  {
    motor_interval = map(motorspeed, 512, 0, motor_interval_max, motor_interval_min);
     digitalWrite(motordata, HIGH); 
  }
  else
  {
    motor_interval = map(motorspeed, 512, 1024, motor_interval_max, motor_interval_min);
    digitalWrite(motordata, LOW); 
  }




  // set flash speed from potentiometer
  val = analogRead(flash_duration_pot_pin); 
  flash_duration = map(val, 1024, 0, flash_duration_min, flash_duration_max);
  flash_duration = constrain(flash_duration, flash_duration_min, flash_duration_max);



  if (motor_interval < motor_interval_max - inactive_area)
  {


    // enable stepper output
    digitalWrite(motordisable, LOW); 

  

    digitalWrite(motorclock, HIGH); 
    delayMicroseconds(motor_interval / 2);
    digitalWrite(motorclock, LOW); 
    delayMicroseconds(motor_interval / 2);
    
    compteur = compteur + 1;

    if (compteur == (steps / frames))
    {
      digitalWrite(ledPin, HIGH); 
      delayMicroseconds(flash_duration);
      digitalWrite(ledPin, LOW);
      compteur = 0;
    }


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

}
