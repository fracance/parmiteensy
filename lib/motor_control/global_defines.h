// definzione pin
#define TX_INPUT 8
#define RX_INPUT 9
#define TX_M1 14
#define RX_M1 15
#define TX_M2 2
#define RX_M2 3
#define EMERGENCY 18
#define PIN_SERVO 23

// parametri auto
#define L 0.175                      // passo auto
#define C 0.174                      // carreggiate 174 originale
#define R_RUOTA 0.0318               // raggio ruota
#define CENTRO_SERVO 96         // centro del servo
#define ACCELERAZIONE_DRITTO_LOW 6.0 // accelerazione
#define ACCELERAZIONE_CURVA_LOW 0.1  // accelerazione
#define ACCELERAZIONE_DRITTO_MED 6.0 // accelerazione
#define ACCELERAZIONE_CURVA_MED 0.1  // accelerazione
#define LIM_DRITT0 5                // angolo di sterzo che definisce dritto e curva
#define DIFF_VEL_DIFF 0.6
#define DECELERAZIONE 10    // decelerazione
#define GAIN 1.1           // gain per far combaciare angolo ruota con angolo servo
#define SATURAZIONE 40     // saturazione in gradi del servo
#define SATURAZIONE_MINI 2 // saturazione mini in gradi del servo
#define MORTA_ZONA_DIFFERENZIALE 5
#define GAIN_DIFF 1.0
#define GAIN_STORTO 1.15
// linearizzazione funzione
#define CONFINE 20 // confine per della linearizzazione della funzione raggio sterzata, angolo servo
#define C1 -0.0759
#define Q1 1.9175
#define C2 -0.0113
#define Q2 0.6994

// i2c sensore
#define SDA_PIN 12
#define SCL_PIN 13

#define SOGLIA_DISTANZA 100

#define PERIOD 10 // periodo di aggiornamento

#define V_MIN 0.1
#define V_MAX 0.3

#define RITARDO_TRAGUARDO 50

#define NUM_GIRI 2
#define VENTOLA 22
#define VEL_VENTOLA 220
#define FRENATA_TRAGUARDO 500