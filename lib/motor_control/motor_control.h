#include <iostream>
#include <Servo.h>
#include <Arduino.h>
//#include <SerialPIO.h>
#include "global_defines.h"

extern Servo sterzo;
// Sostituisci le dichiarazioni esterne:
extern HardwareSerial &motor1;
extern HardwareSerial &motor2;
extern HardwareSerial &debix;
extern bool emergency_flag;
extern float velocita_lineare;
extern double oldtime;
extern int counter;
extern bool prima_volta;
extern long unsigned int tempo_inizio;
extern bool fine_programma;
extern float vel1, vel2;
extern bool parmigiano;
extern float accelerazione_curva;
extern float accelerazione_dritto;
extern float decelerazione;	
extern float accelerazione_curva_intero;
extern float accelerazione_dritto_intero;
extern float decelerazione_intero;
extern int vel_ventola;
extern int centro_servo;


void controllo_velocita(float velocita_des,float &velocita_attuale);
void stop_motors();
void ricezione_uart(float &num1, float &num2, int &num3);
void controllo_motori(float vel_target, float ang_target);
bool emergency_stop();
bool fine_giro(int num3);
void reset_programma();
