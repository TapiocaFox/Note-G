# Note-G
A portable music game console made with Arduino UNO. You can edit your music sheet on web and upload it to Arduino through bluetooth. Open Music XML is supported.

## How to use it
1. Open Web editor in src/browder/index.html
2. Build Note-G.ino to your Arduino board.
3. Upload the misuc sheet from web editor to Arduino. You display will show track name if it's uploaded. Or you can check Serial port log.
4. Press the second button to play. Press again to play again (with fatal bug).
5. Enjoy the game. 100 points for hit within time window. 50 points for hit within 2 times time window. Still 10 points if you sucks.

[Demo video](https://www.youtube.com/watch?v=vzhMY8T0v5A)

## Hardware requirements
* Arduino UNO or better (UNO sucks hard. Any better board will make it.)
* A speaker
* A bluetooth module (We use Bluetooth-4.0 V2)
* 5 buttons (Actually 2 buttons in use cuase UNO sucks)
* A TFT display shield (We use 3.5" TFT shield with ILI9486 driver)
* Lots of cables

A simple [document](https://docs.google.com/presentation/d/146XQ0_US55G4nBLCPsQQ8guDu7obMFU0QmIVaeavj1w/edit?usp=sharing) for your reference. Implentation details and architecture included.

## ToDo
* Game becomes unplayable when it starts again because of wrong bPC reset(byte reading index counter for bars).
* The bar of the first note is missing.
* Bars won't disappear even they go over the line, only when buffer is full, causing player still hitting over-line bars instead of upcoming bars.
