This is visual simulator of a led Strip (WS28412B) in Qt5.

It accepts commands by UDP port 1200.

Purpose is to validate animation code without hardware.

Local commands
--------------
* key q : quit application
* key a : append a new pixel of current color after previous one
* key r : set current color to red + reset pixel counter
* key g : set current color to green + reset pixel counter
* key b : set current color to blue + reset pixel counter
* key y : set current color to yellow + reset pixel counter
* key w : set current color to white + reset pixel counter
* key PgDown : light down current color + reset pixel counter
* key Pgup : light up current color + reset pixel counter

UDP protocole
-------------

Command to send a "framebuffer"
* AdA < 16 bits pixel count> <16 bit ofsset start> red1,green1,blue1, red2,green2,blue2, ... 

Command to send a led command
* AdB <16 its command id> < ... optional specific parameters >
* AdB <0x0001> r g b : Command to set a solid color to all leds of strip

See [tools/sender_udp/](../sender_udp) C & Python3 implementation examples.
