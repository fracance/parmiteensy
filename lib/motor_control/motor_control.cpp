#include "motor_control.h"

HardwareSerial &motor1 = Serial3;
HardwareSerial &motor2 = Serial5;
HardwareSerial &debix = Serial2;
void telemetria_motori()
{
  debix.print(velocita_lineare);
  debix.print(" ");
  debix.print(vel1);
  debix.print(" ");
  debix.println(vel2);
}

void controllo_velocita(float velocita_des, float &velocita_attuale, float acc)
{
  double deltat = (millis() - oldtime) / 1000;

  if (velocita_attuale < velocita_des - 0.01)
  {
    velocita_attuale = velocita_attuale + deltat * acc;
    if (velocita_attuale > velocita_des)
    {
      velocita_attuale = velocita_des;
    }
  }
  else if (velocita_attuale > velocita_des + 0.01)
  {
    velocita_attuale = velocita_attuale - deltat * decelerazione;
    if (velocita_attuale < velocita_des)
    {
      velocita_attuale = velocita_des;
    }
  }
}

void reset_programma()
{
  if (emergency_stop())
  {
    counter = 0;
    fine_programma = false;
    parmigiano = false;
    analogWrite(VENTOLA, 0);
    debix.println("SET");
  }
}

void stop_motors()
{
  emergency_flag = true;
  motor1.print("V");
  motor1.print(0);
  motor1.print("\n");

  motor2.print("V");
  motor2.print(0);
  motor2.print("\n");

  sterzo.write(centro_servo);
  velocita_lineare = 0.0;
  telemetria_motori();
  reset_programma();
}

bool fine_giro(int num3)
{
  if (num3 == 1)
  {
    if (prima_volta == true)
    {
      counter++;
      prima_volta = false;
      tempo_inizio = millis();
    }
  }
  if (millis() > tempo_inizio + 3000)
    prima_volta = true;
  if (counter >= NUM_GIRI && millis() > tempo_inizio + RITARDO_TRAGUARDO)
  {
    if (millis() > tempo_inizio + RITARDO_TRAGUARDO + FRENATA_TRAGUARDO)
      analogWrite(VENTOLA, 100);
    return true;
  }
  else
    return false;
}

bool emergency_stop()
{
  if (pulseIn(EMERGENCY, HIGH) > 1500)

    // if (!digitalRead(EMERGENCY))
    return false;

  else
    return true;
}

void ricezione_uart(float &num1, float &num2, int &num3)
{
  if (debix.available())
  {
    // Leggiamo la riga fino al carattere di fine linea
    String line = debix.readStringUntil('\n');
    line.trim(); // Rimuove eventuali spazi o caratteri di fine linea extra

    // Cerchiamo di individuare gli spazi che separano i numeri
    int firstSpace = line.indexOf(' ');
    int secondSpace = line.indexOf(' ', firstSpace + 1);

    // Se non troviamo gli spazi, i dati non sono validi: usciamo
    // if (firstSpace == -1 || secondSpace == -1) {
    //   return;
    // }
    if (firstSpace == -1)
    {
      return;
    }

    // Estraiamo il primo e il secondo numero come stringhe
    String num1Str = line.substring(0, firstSpace);
    String num2Str = line.substring(firstSpace + 1, secondSpace);
    String num3Str = line.substring(secondSpace + 1);
    // String num2Str = line.substring(firstSpace + 1);

    // Convertiamo le stringhe in numeri interi
    num1 = num1Str.toFloat();
    num2 = num2Str.toFloat();
    num3 = num3Str.toInt();
  }
  // else
  //   Serial.println("vaffanculo");
  // {
  //   num1 = 0;
  //   num2 = 0;
  //   num3 = 0;
  // }
}

void controllo_motori(float vel_target, float ang_target)
{
  if (!emergency_stop())
  {
    int angle_sterzo;
    float R;

    // Calcoliamo l'angolo di sterzo
    angle_sterzo = GAIN * degrees(ang_target);
    if (angle_sterzo <= SATURAZIONE_MINI && angle_sterzo >= 0)
      angle_sterzo = SATURAZIONE_MINI;
    if (angle_sterzo >= -SATURAZIONE_MINI && angle_sterzo <= 0)
      angle_sterzo = -SATURAZIONE_MINI;

    if (angle_sterzo >= SATURAZIONE)
      angle_sterzo = SATURAZIONE;
    if (angle_sterzo <= -SATURAZIONE)
      angle_sterzo = -SATURAZIONE;

    if (abs(angle_sterzo * GAIN_STORTO) < LIM_DRITT0 && ang_target < 0)
      controllo_velocita(vel_target, velocita_lineare, accelerazione_dritto);
    else if (abs(angle_sterzo) < LIM_DRITT0 && ang_target > 0)
      controllo_velocita(vel_target, velocita_lineare, accelerazione_dritto);
    else
      controllo_velocita(vel_target, velocita_lineare, accelerazione_curva);

    if (abs(angle_sterzo) < MORTA_ZONA_DIFFERENZIALE)
    {
      vel1 = velocita_lineare;
      vel2 = velocita_lineare;
    }
    else
    {
      if (abs(angle_sterzo) < CONFINE)
      {
        R = C1 * abs(angle_sterzo) + Q1;
        // if (angle_sterzo < 0)
        //   R = C1 * abs(angle_sterzo * GAIN_STORTO) + Q1;
      }
      else
      {
        R = C2 * abs(angle_sterzo) + Q2;
        // if (angle_sterzo < 0)
        //   R = C2 * abs(angle_sterzo * GAIN_STORTO) + Q2;
      }
      float vel_ang = velocita_lineare / R;
      float r1 = R - (GAIN_DIFF * C) / 2;
      float r2 = R + (GAIN_DIFF * C) / 2;
      // controllo_velocita(vel_ang * r1, vel1);
      // controllo_velocita(vel_ang * r2, vel2);
      if (ang_target < 0)
      {
        vel1 = vel_ang * r2;
        vel2 = vel_ang * r1;
        // if (vel1 >= vel2 + DIFF_VEL_DIFF)
        //   vel1 = vel2 + DIFF_VEL_DIFF;
      }
      else
      {
        vel1 = vel_ang * r1;
        vel2 = vel_ang * r2;
        // if (vel2 >= vel1 + DIFF_VEL_DIFF)
        //   vel2 = vel1 + DIFF_VEL_DIFF;
      }
    }
    telemetria_motori();
    float rad1 = vel1 / R_RUOTA;
    float rad2 = vel2 / R_RUOTA;

    // Inviamo il primo numero su motor1 e il secondo su motor2
    motor1.print("V");
    motor1.print(rad1);
    motor1.print("\n");

    // secondo motore
    motor2.print("V");
    motor2.print(rad2);
    motor2.print("\n");

    //  servo
    if (ang_target < 0)
      sterzo.write((abs(angle_sterzo)* GAIN_STORTO  + centro_servo));
    else
      sterzo.write((-abs(angle_sterzo) + centro_servo));
  }
  else
    stop_motors();
}
