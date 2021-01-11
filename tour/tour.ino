#include <Adafruit_NeoPixel.h>
#include <EasyButton.h> // https://easybtn.earias.me/docs/fundamentals

#define DATA_PIN 3 // pin pour neopixels
#define WHITE_PIN 5 // pin PWM commande leds blanches
#define BTN_PIN 7 // entrée bouton
#define NUMPIXELS 480 // nombre total de neopixels

#define MAX_WHITE 150 // 150 pour batterie 18v, 255 pour batterie 12v
#define MAX_NEO_WHITE 120 // blanc max pour neopixels
//#define FPS 50 // on travaille en 50 images par seconde
#define LONGEUR_FIRE 3 // nombre de leds qui composent le feu
#define LONGEUR_QUEUE 15 // nombre de leds qui composent le feu
#define MAX_SHOT 10 // nombre de fusée en même temps
#define B_MAX 45 // bleu max plus le chiffre est petit plus ce sera blanc

#define CYCLE_WHITE 15 // pour void cycle
#define MAX_COLOR 255 // colour max pour neopixel


int bande_white_PIN[] = {5, 9};
int slot = 0; //de 0 à MAX_SHOT = dernière animation déclecnhés avec le bouton
int anim[MAX_SHOT], r[MAX_SHOT], g[MAX_SHOT], b[MAX_SHOT]; // // position de chaque anim[0] à anim[7]+ couleurs des différentes anims possibles
int white = 0; // puissance du blanc


// pin, debounce, pullup
EasyButton button(BTN_PIN, 10, true);


Adafruit_NeoPixel pixels(NUMPIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  // initialize le bouton
  button.begin();

  // lance l'explosion après un triple clic sur une seconde
  //button.onSequence(3, 1000, cycle_white);

  // lance l'explosion après un clic de 3 secondes
  button.onPressedFor(1000, cycle_white);


  // initialise le tableau d'animations
  for (int i = 0; i < MAX_SHOT; i++)
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

  r[6] = 255;
  g[6] = 0;
  b[6] = 0;

  r[7] = 255;
  g[7] = 255;
  b[7] = 0;

  r[9] = 0;
  g[9] = 0;
  b[9] = 255;

  r[11] = 0;
  g[11] = 255;
  b[1] = 0;

  r[13] = 0;
  g[13] = 0;
  b[13] = 255;

  r[14] = 0;
  g[14] = 255;
  b[14] = 255;

  pixels.begin();
  Serial.begin(9600);
}

void loop() {

  button.read();


  if (button.wasReleased()  || random(500) == 1) {
    if (slot > MAX_SHOT - 1)
    {
      slot = 0;
    }
    anim[slot] = 0;
    Serial.write('slot : ');
    Serial.println(slot);
    slot ++;
  }


  // boucle sur chaque anim
  for (int i = 0; i < MAX_SHOT; i++) {

    // incrémente de une frame chaque animation
    anim[i] ++;

    if (anim[i] > 10000)
    {
      anim[i] = 10000;
    }


    // etape 1 : la montée !
    if (anim[i] > 0 && anim[i] < 480)
    {
      // éteint la leds précédente
      pixels.setPixelColor(anim[i], pixels.Color(0, 0, 0));


      // la queue de fusée
      for (int f = LONGEUR_QUEUE; f < LONGEUR_QUEUE + LONGEUR_FIRE; f++)
      {
        pixels.setPixelColor(anim[i] + f, pixels.Color(r[i] , g[i], b[i]));
      }

      // la fusée en elle même
      for (int f = 1; f < LONGEUR_QUEUE; f++)
      {
        pixels.setPixelColor(anim[i] + f, pixels.Color(r[i] / LONGEUR_QUEUE * f , g[i] / LONGEUR_QUEUE * f, b[i] / LONGEUR_QUEUE * f));
      }

    }


    // etape 2 : la descente
    if (anim[i] > 480 && anim[i] < 961)
    {
      // éteint la leds précédente
      pixels.setPixelColor(960 - anim[i], pixels.Color(0, 0, 0));


      // la queue de fusée
      for (int f = LONGEUR_QUEUE; f < LONGEUR_QUEUE + LONGEUR_FIRE; f++)
      {
        pixels.setPixelColor(960 - anim[i] - f, pixels.Color(r[i] , g[i], b[i]));
      }

      // la fusée en elle même
      for (int f = 1; f < LONGEUR_QUEUE; f++)
      {
        pixels.setPixelColor(960 - anim[i] - f, pixels.Color(r[i] / LONGEUR_QUEUE * f , g[i] / LONGEUR_QUEUE * f, b[i] / LONGEUR_QUEUE * f));
      }
    }


  }

  pixels.show();   // Send the updated pixel colors to the hardware.

  delay(1);

}





void cycle_white() {
  pixels.clear();
  pixels.show();

  noise();

  int speed_white = 1;
  int whiteval = 0;
  int pinwhite = 0;
  int cyclenumero;
  int b = 0;

  for (cyclenumero = 0; cyclenumero < CYCLE_WHITE; cyclenumero++ ) {
    pinwhite = 0;
    whiteval = 0;
    //selection de la pin

    //montée neo

    for ( whiteval = 0; whiteval < MAX_NEO_WHITE; whiteval = whiteval + speed_white) {
      for (int i = 0; i < NUMPIXELS; i++) {


        b = whiteval * B_MAX / MAX_NEO_WHITE;

        pixels.setPixelColor(i, pixels.Color(whiteval, whiteval, b));
      }
      pixels.show();
    }

    delay(10);
    Serial.println(whiteval);

    //descente

    for ( whiteval ; whiteval > 0; whiteval = whiteval - speed_white) {
      for (int i = 0; i < NUMPIXELS; i++) {
        b = whiteval * B_MAX / MAX_NEO_WHITE;
        //b = whiteval / B_MAX;
        pixels.setPixelColor(i, pixels.Color(whiteval, whiteval, b));
      }
      pixels.show();
    }

    pixels.clear();
    pixels.show();



    //fin neo

    //montée bande 1

    for ( whiteval = 0; whiteval < MAX_WHITE; whiteval = whiteval + speed_white) {
      analogWrite(bande_white_PIN[0], whiteval);
      delay(10);
    }
    //descente
    for ( whiteval ; whiteval > 0; whiteval = whiteval - speed_white) {
      analogWrite(bande_white_PIN[0], whiteval);
      delay(10);
    }
    analogWrite(bande_white_PIN[0], 0);// pas necessaire
    delay(10);
    //montée bande 2

    for ( whiteval = 0; whiteval < MAX_WHITE; whiteval = whiteval + speed_white) {
      analogWrite(bande_white_PIN[1], whiteval);
      delay(10);
    }
    //descente
    for ( whiteval ; whiteval > 0; whiteval = whiteval - speed_white) {
      analogWrite(bande_white_PIN[1], whiteval);
      delay(10);
    }
    analogWrite(bande_white_PIN[1], 0);// pas necessaire
    delay(10);
    Serial.println("neo");

    if (speed_white < 65) {
      speed_white = speed_white * 2;
    }
    pinwhite = 0;
    Serial.print("speed_white ");
    Serial.println(speed_white);
  }


  // fin cycle


  delay(2000);


  // boom
  ALL_ON();
  delay (3000);

  //fin
  pixels.clear();
  pixels.show();
  analogWrite(bande_white_PIN[0], 0);
  analogWrite(bande_white_PIN[1], 0);

  delay(15000);// 15 secondes avant relance
}

void ALL_ON () {
  analogWrite(bande_white_PIN[0], MAX_WHITE);

  analogWrite(bande_white_PIN[1], MAX_WHITE);

  pixels.clear();

  for (int i = 0; i < NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(MAX_NEO_WHITE, MAX_NEO_WHITE, B_MAX));


  } pixels.show();

}

void ALL_OFF () {
  analogWrite(bande_white_PIN[0], 0);

  analogWrite(bande_white_PIN[1], 0);

  pixels.clear();

  pixels.show();
}
void noise() {
  //noise
  for (int i = 0; i < 10; i++) {
    int r = random(10, 100);
    delay(r);
    ALL_ON();
    delay(r);
    ALL_OFF();
  }
}
