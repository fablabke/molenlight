# Molenlight 2020
Code utilisé pour le projet molenlight 2020

Pour cette recette, il vous faut environ : 

- 100 mètres de ruban led
- 50 mètres de rubans neopixels
- 8 arduino nano
- des régulateurs DC-DC 5V et 12V
- des modules mosfets
- des batteries de visseuses 18V 5A
- du cable et des switches
- beaucoup de main d'oeuvre

Ainsi qu'un peu de code, que vous trouverez dans les dossiers de ce repository :

- /firework contient le code d'un module portatif alimenté par batterie de visseuse 18v qui permet de lancer des "feux" d'artifice lumineux (neopixels) qui se terminent en explosion de led blanche (mosfet commandé en PWM)
- /tour contient le code d'un module alimentant 1 long ruban de neopixel de 8 mètres permettant de faire un ping pong lumineux commandé par switch. Un appui long sur le switch permet de lancer une explosion rotative de lumière blanche (mosfet)

Le code propose une méthode pour avoir plusieurs "feux" en parallèle sans utiliser de delay() dans le code arduino.