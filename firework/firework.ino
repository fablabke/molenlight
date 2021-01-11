#include <Adafruit_NeoPixel.h>


#define DATA_PIN 3 // pin pour neopixels
#define WHITE_PIN 5 // pin PWM commande leds blanches
#define BTN_PIN 7 // entrée bouton
#define NUMPIXELS 240 // nombre total de neopixels
#define MAX_WHITE 100 // 150 pour batterie 18v, 255 pour batterie 12v
//#define FPS 50 // on travaille en 50 images par seconde
#define LONGEUR_FIRE 20 // nombre de leds qui composent le feu

int slot = 0;
int anim[8]; // de anim[0] à anim[7]
int r[8]; // couleurs des différentes anims possibles
int g[8];
int b[8];

int white = 0; // puissance du blanc

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 10;    // the debounce time; increase if the output flickers
int lastButtonState = HIGH;
int buttonState;


Adafruit_NeoPixel pixels(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {



  // initialise le tableau d'animations
  for (int i = 0; i < 8; i++)
  {
    anim[i] = 10000;
    r[i] = random(255);
    g[i] = random(255);
    b[i] = random(255);
  }

  // on en force quelques une pour avoir des chouettes couleurs (à peaufiner huhuh)
  r[0] = 255;
  g[0] = 0;
  b[0] = 0;

  r[1] = 255;
  g[1] = 255;
  b[1] = 0;

  r[2] = 0;
  g[2] = 0;
  b[2] = 255;

  r[3] = 0;
  g[3] = 255;
  b[3] = 0;

  r[4] = 0;
  g[4] = 0;
  b[4] = 255;

  r[5] = 0;
  g[5] = 255;
  b[5] = 255;

  pixels.begin();

  pinMode(BTN_PIN, INPUT_PULLUP);

  Serial.begin(9600);

}

void loop() {


  // gestion bouton
  int reading = digitalRead(BTN_PIN);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        {

          if (slot > 7)
          {
            slot = 0;
          }
          anim[slot] = 0;
          Serial.println(slot);
          slot ++;
        }
      }
    }
  }
  lastButtonState = reading;




  // boucle sur chaque anim
  for (int i = 0; i < 8; i++) {

    // incrémente de une frame chaque animation
    anim[i] ++;

    
    if (anim[i] > 10000)
    {
      anim[i] = 10000;
    }


    // etape un : la montée !
    if (anim[i] > 0 && anim[i] < 240)
    {
      // éteint la leds précédente
      pixels.setPixelColor(anim[i], pixels.Color(0, 0, 0));


      for (int f = 1; f < LONGEUR_FIRE; f++)
      {
        pixels.setPixelColor(anim[i] + f, pixels.Color(r[i], g[i], b[i]));
      }
    }

    // étape 2 extinction des leds de la montée
    if (anim[i] >= 240 && anim[i] < 260)
    {
      for (int f = 0; f < LONGEUR_FIRE + 1; f++)
      {
        pixels.setPixelColor(anim[i], pixels.Color(0, 0, 0));
      }
    }

    // étape 3, le boom !
    if (anim[i] >= 240 && anim[i] < 285)
    {
      //white = white + (map(anim[i], 260, 285, 0, MAX_WHITE) / 8);
      white = white + (MAX_WHITE / 2);
    }


    // étape 4, réduction du boom !
    if (anim[i] >= 280 && anim[i] < 400)
    {
      //white = white - (map(anim[i], 300, 500, 0, MAX_WHITE)/8);
      white = white - 1;
    }

    // étape 4, réduction du boom plus lent vers la fin
    if (anim[i] >= 400 && anim[i] < 900)
    {
      //white = white - (map(anim[i], 300, 500, 0, MAX_WHITE)/8);
      if (anim[i] % 3  == 0)
      {
        white = white - 1;
      }
    }


  }

  pixels.show();   // Send the updated pixel colors to the hardware.

  // limitation du white à ce que l'on permet en min et max
  if (white < 0)
  {
    white = 0;
  }

  if (white > MAX_WHITE)
  {
    white = MAX_WHITE;
  }
  analogWrite(WHITE_PIN, white);

  delay(2); // finalement plus simple que de faire le calcul en FPS (on fait à l'oeuil :-))

}
