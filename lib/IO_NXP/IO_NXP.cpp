#include "IO_NXP.h"

NXP_IO::NXP_IO(Adafruit_SH1106 *disp, Encoder *enc)
    : display(disp), encoder(enc),
      receivingImage(false),
      indice(0),
      idx(0),
      change_variable(false),
      exposure(E_DEFAULT),
      program(P_DEFAULT),
      threshold(TH_DEFAULT)
{
  // Eventuali inizializzazioni specifiche della classe
}

void NXP_IO::displayImage()
{
  display->clearDisplay();
  // Disegna l'immagine usando il buffer: qui viene usata una larghezza di 96, modifica se necessario
  display->drawBitmap(0, 0, imageBuffer, 96, SCREEN_HEIGHT, WHITE);
  display->display();
}

void NXP_IO::show_image()
{
  Serial.println("READY"); // Segnale a Python per inviare l'immagine

  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == 'S')
    {
      receivingImage = true;
      indice = 0;
    }
    else if (c == 'E')
    {
      receivingImage = false;
      displayImage();
      Serial.flush();
      break;
    }
    else if (receivingImage)
    {
      imageBuffer[indice++] = c;
    }
  }
}

void NXP_IO::menu_display()
{
  display->clearDisplay();
  for (int i = 0; i < 5; i++)
  {
    int y = 10 + i * 12; // Posizione verticale per ogni voce
    if (i == idx)
    {
      // Elemento selezionato: testo nero su sfondo bianco
      display->fillRect(0, y - 2, display->width(), 10, WHITE);
      display->setTextColor(BLACK, WHITE);
    }
    else
    {
      // Elementi non selezionati: testo bianco su sfondo nero
      display->setTextColor(WHITE, BLACK);
    }
    display->setCursor(0, y);
    switch (i)
    {

    case 0:
      display->print("Set Program");
      break;
    // case 3:
    //   display->print("Reset");
    //   break;
    case 1:
      display->print("Run Program");
      break;
    case 3:
      display->print("Set Straight Acc");
      break;
    case 2:
      display->print("Set Curve Acc");
      break;
    case 4:
      display->print("Set Piedone");
      break;
    default:
      break;
    }
  }
  display->display();
}

void NXP_IO::submenu_display()
{
  display->clearDisplay();
  display->setTextColor(WHITE, BLACK);
  display->setCursor(0, 0);
  switch (idx)
  {
  case 2:
    display->print("Set Curve Acceleration");
    display->setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    display->print(accelerazione_curva_intero);
    break;
  case 3:
    display->print("Set Straight Acceleration");
    display->setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    display->print(accelerazione_dritto_intero);
    break;
  case 0:
    display->print("Set Program");
    display->setCursor((SCREEN_WIDTH / 2)-30, SCREEN_HEIGHT / 2);
    if (program <= 2)
      switch(program){
        case 0:
        display->print("Veloce");
        break;
        case 1:
        display->print("Medio");
        break;
        case 2:
        display->print("Lento");
        break;

      }
    else
      display->print("Exit");
    break;
  case 4:
    //display->print("Set Deceleration");
    display->print("Set Velocità Ventola");
    //display->print("Set Centro");

    display->setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    //display->print(decelerazione_intero);
    display->print(vel_ventola);
    //display->print(centro_servo);
    break;
  case 1:
    display->setCursor(0, SCREEN_HEIGHT / 2);
    display->print("Memiamola");
    break;
  }
  display->display();
}

void NXP_IO::setIdx(int totalItems)
{
  if (encoder->isRight())
    idx++;
  else if (encoder->isLeft())
    idx--;
  if (idx < 0)
    idx = totalItems - 1;
  if (idx >= totalItems)
    idx = 0;
}

int NXP_IO::setIdx2(int totalItems, int variable)
{
  if (encoder->isRight())
    variable++;
  else if (encoder->isLeft())
    variable--;
  if (variable < 0)
    variable = totalItems - 1;
  if (variable >= totalItems)
    variable = 0;
  return variable;
}

void NXP_IO::set_variable()
{
  switch (idx)
  {
  case 0:
    if (program <= 2)
    {
      debix.print("P ");
      debix.println(program);
      if (program == 0)
      {
      }
      else if (program == 1)
      {
        accelerazione_dritto = ACCELERAZIONE_DRITTO_MED;
        accelerazione_curva = ACCELERAZIONE_CURVA_MED;
        decelerazione = DECELERAZIONE;
        accelerazione_dritto_intero = accelerazione_dritto*10;
        accelerazione_curva_intero = accelerazione_curva*10;
        decelerazione_intero = decelerazione*10;
      }

      else if (program == 2)
      {
        accelerazione_dritto = ACCELERAZIONE_DRITTO_LOW;
        accelerazione_curva = ACCELERAZIONE_CURVA_LOW;
        decelerazione = DECELERAZIONE;
        accelerazione_dritto_intero = accelerazione_dritto*10;
        accelerazione_curva_intero = accelerazione_curva*10;
        decelerazione_intero = decelerazione*10;
      }
    }
    break;
  case 4:
    decelerazione = decelerazione_intero / 10;
    // Serial.print("Decelerazione: ");
    // Serial.println(decelerazione);
    break;
  case 2:
    accelerazione_curva = accelerazione_curva_intero / 10;
    // Serial.print("Accelerazione curva: ");
    // Serial.println(accelerazione_curva);

    break;
  case 3:
    accelerazione_dritto = accelerazione_dritto_intero / 10;
    // Serial.print("Accelerazione dritto: ");
    // Serial.println(accelerazione_dritto);
    break;
  case 1:
    emergency_flag = false;
    // show_image();
    debix.println("RUN");

    for (int i = 8; i > 0; i--)
    {
      // if(i==1)
      //   analogWrite(VENTOLA,VEL_VENTOLA);
      display->clearDisplay();
      display->setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
      display->print(i);
      display->display();
      delay(1000);
    }
    display->clearDisplay();
    display->setCursor(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2);
    display->print("RUN!");
    display->display();
    oldtime = millis();
    // delay(PERIOD);
    // main_running();
    // do
    // {
    //   debix.println("SET");
    // } while (debix.readString() == "SET OK");
    parmigiano = true;
    break;
  }
}

void NXP_IO::menu_interface()
{
  setIdx(5);
  menu_display();
  if (encoder->isClick())
  {
    // Entra in un ciclo finché l'encoder non viene rilasciato
    while (!encoder->isClick())
    {
      if (idx == 2)
        accelerazione_curva_intero = setIdx2(101, accelerazione_curva_intero);
      else if (idx == 3)
        accelerazione_dritto_intero = setIdx2(101, accelerazione_dritto_intero);
      else if (idx == 0)
        program = setIdx2(4, program);
      else if (idx == 4){
        //decelerazione_intero = setIdx2(101, decelerazione_intero);
        vel_ventola = setIdx2(256,vel_ventola);
        //centro_servo = setIdx2(111, centro_servo);
      }
      // else
      // {
      //   exposure = E_DEFAULT;
      //   program = P_DEFAULT;
      //   threshold = TH_DEFAULT;
      // }
      submenu_display();
    }
    set_variable();
    // debix.flush();
  }
}

void NXP_IO::main_running()
{
  int distance;
  while (!emergency_stop())
  {
    if ((millis() > oldtime + PERIOD) && fine_programma == false)
    {
      static float vel_target, ang_target;
      static int traguardo;
      ricezione_uart(vel_target, ang_target, traguardo);
      // Serial.print(vel_target);
      // Serial.print(" ");
      // Serial.print(ang_target);
      // Serial.print(" ");
      // Serial.println(traguardo);
      if (fine_giro(traguardo))
      {
        read_distance(distance);
        // controllo_distanza(distance, vel_target, bool_distanza);
      }
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
}
