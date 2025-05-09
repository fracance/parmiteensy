
#define IO_NXP_H

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "GyverEncoder.h"
#include "motor_control.h"
#include "sensor_control.h"
#include "global_defines.h"

#define SCREEN_WIDTH 128    // Larghezza del display in pixel
#define SCREEN_HEIGHT 64    // Altezza del display in pixel
#define SCREEN_ADDRESS 0x3C ///< 0x3D per 128x64, 0x3C per 128x32
#define OLED_RESET -1       // Pin di reset (usa -1 se non usato)
#define PIN_A 10
#define PIN_B 11
#define BUTTON_PIN 12

#define TH_DEFAULT 100
#define P_DEFAULT 1
#define E_DEFAULT 100

extern bool parmigiano;

class NXP_IO {
  public:
    // Costruttore: riceve i puntatori agli oggetti già inizializzati nel main
    NXP_IO(Adafruit_SH1106* disp, Encoder* enc);

    // Metodo principale per gestire l'interfaccia
    void menu_interface();

  private:
    // Puntatori agli oggetti passati dal main
    Adafruit_SH1106* display;
    Encoder* encoder;

    // Variabili interne
    uint8_t imageBuffer[2048]; // Buffer per l'immagine (128x64 / 8)
    bool receivingImage;
    int indice;
    int idx;
    volatile bool change_variable;
    int exposure;
    int program;
    int threshold;

    // Metodi ausiliari interni alla classe
    void displayImage();
    void show_image();
    void menu_display();
    void submenu_display();
    void setIdx(int totalItems);
    int setIdx2(int totalItems, int variable);
    void set_variable();
    void main_running();
};
