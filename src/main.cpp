#include <Arduino.h>
// #include <SerialPIO.h>
#include <Wire.h>
#include "IO_NXP.h"


Adafruit_SH1106 display(OLED_RESET);
Encoder enc(PIN_A, PIN_B, BUTTON_PIN);

Servo sterzo;
#if SENSOR
VL53L1X sensor_distance;
#else
VL53L0X sensor_distance;
#endif

int distance;
bool emergency_flag = false;
float velocita_lineare = 0.0;
float vel1, vel2;
double oldtime = 0.0;
int counter = 0;
bool prima_volta = true;
long unsigned int tempo_inizio;
bool fine_programma = false;
bool parmigiano = false;
float accelerazione_dritto = ACCELERAZIONE_DRITTO_LOW;
float accelerazione_curva = ACCELERAZIONE_CURVA_LOW;
float decelerazione = DECELERAZIONE;
float accelerazione_dritto_intero = accelerazione_dritto * 10;
float accelerazione_curva_intero = accelerazione_curva * 10;
float decelerazione_intero = decelerazione * 10;
int centro_servo = CENTRO_SERVO;
int vel_ventola = VEL_VENTOLA;

// Creazione dell'istanza della libreria, passandogli gli oggetti
NXP_IO io(&display, &enc);

void isrCLK()
{
  enc.tick(); // отработка в прерывании
}
void isrDT()
{
  enc.tick(); // отработка в прерывании
}
void isrClick()
{
  enc.tick(); // отработка в прерывании
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VENTOLA, OUTPUT);
  digitalWrite(VENTOLA,0);
  digitalWrite(LED_BUILTIN, HIGH);
  // // Inizializza il display, specificando l'indirizzo I2C (spesso 0x3C per questi moduli)
  display.begin(SH1106_SWITCHCAPVCC, SCREEN_ADDRESS);
  enc.setType(TYPE2);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_A), isrCLK, CHANGE); // прерывание на 2 пине! CLK у энка
  attachInterrupt(digitalPinToInterrupt(PIN_B), isrDT, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isrClick, CHANGE);

  debix.begin(115200);

  motor1.begin(115200);
  motor2.begin(115200);

  sterzo.attach(PIN_SERVO);
  sterzo.write(centro_servo);
  pinMode(EMERGENCY, INPUT_PULLUP);
  oldtime = millis();
  setup_sensor();

  display.display();
  delay(2000);
}

void loop()
{
 if (parmigiano == false)
  { // 
    io.menu_interface();
    oldtime = millis();
  }
  else
  {
    if ((millis() > oldtime + PERIOD) && fine_programma == false)
    {
      //analogWrite(VENTOLA, vel_ventola);
      static float vel_target, ang_target;
      static int traguardo;
      ricezione_uart(vel_target, ang_target, traguardo);
      if (fine_giro(traguardo))
      {
        read_distance(distance);
        //distance = 200;
        controllo_distanza(distance, vel_target);
      } else 
        analogWrite(VENTOLA, vel_ventola);
      if (!fine_programma)
        controllo_motori(vel_target, ang_target);
      oldtime = millis();
    }
    else if (fine_programma)
    {
      reset_programma();
      oldtime = millis();
    }
  }

  // sterzo.write(CENTRO_SERVO + SATURAZIONE);
  // delay(500);
  // sterzo.write(CENTRO_SERVO - SATURAZIONE);
  // delay(500);

  //  read_distance(distance);
  //  Serial.println(distance);
  // delay(100);
}