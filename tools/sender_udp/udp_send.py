#!/usr/bin/python3
#
# takes 4 arguments : 32bits idx  + rgb as decimal
# n'implémente qu'un pseudo protocol !
# envoi un seul pixel à la fois

import time
import struct
import sys
import socket

# "knight" need
import colorsys

if ( len(sys.argv) < 2 ):
    print ("""\
    This script need at least 1 parameter : the command

    Usage:  theScript <cmd> < params ...>
    """);
    quit();


sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM);

## 4 arguments :  idx (32bits), r,g,b (8bits)
def setPixel(idx, r,g,b):
    pix_count=1
    pix_pos=idx
    message=struct.pack( '<3B2h4B', ord('A'),ord('d'),ord('A'), pix_count, pix_pos, 0xA5 , r,g,b );
    sock.sendto(message , ("127.0.0.1",1200) );


## applique la commande all pixels 1 couleur
## 3 arguments :   r,g,b (8bits)
def setAllPixels(r,g,b):
    cmde=1
    message=struct.pack( '<3B1h3B', ord('A'),ord('d'),ord('B'), cmde, r,g,b );
    sock.sendto(message , ("127.0.0.1",1200) );



if sys.argv[1] == "pix":
    if ( len(sys.argv) < 6 ):
        print ("""\
        This script need 4 parameters

        Usage:  Script.py pix idx r g b
        """);
        quit();
    else:
        setPixel( int(sys.argv[2]), int(sys.argv[3]),int(sys.argv[4]),int(sys.argv[5]));
        quit();

if sys.argv[1] == "all":
    if ( len(sys.argv) < 5 ):
        print ("""\
        This script need 5 parameters

        Usage:  Script.py all  r g b
        """);
        quit();
    else:
        setAllPixels( int(sys.argv[2]), int(sys.argv[3]),int(sys.argv[4]) );
        quit();

if sys.argv[1] == "anim1":
    if ( len(sys.argv) < 5 ):
        print ("""\
        This script need 5 parameters

        Usage:  Script.py anim  r g b
        """);
        quit();
    else:
        ## faire une animation simple
        for x in range(30, 40):
            time.sleep(0.2);
            setPixel( x, int(sys.argv[2]),int(sys.argv[3]),int(sys.argv[4]) );
        quit();

# from https://github.com/hyperion-project/hyperion/blob/master/effects/knight-rider.py
if sys.argv[1] == "knight":
    # Get the parameters
    speed      =  1.0
    fadeFactor = 0.7
    color      = (255,0,0)

    # surfaxe de balade : de 0 à width
    width = 80

    # Check parameters
    speed = max(0.0001, speed)
    fadeFactor = max(0.0, min(fadeFactor, 1.0))

    # Initialize the led data
    imageData = bytearray(width * (0,0,0))
    imageData[0] = color[0]
    imageData[1] = color[1]
    imageData[2] = color[2]

    # Calculate the sleep time and rotation increment
    increment = 1
    sleepTime = 1.0 / (speed * width)
    while sleepTime < 0.05:
        increment *= 2
        sleepTime *= 2

    # Start the write data loop
    position = 0
    direction = 1
    while 1==1 :
        # send to drawer
        #hyperion.setImage(width, 1, imageData)
        for i in range(width):
            setPixel( i, imageData[i*3],imageData[i*3+1],imageData[i*3+2]);

        # Move data into next state
        for i in range(increment):
            position += direction
            if position == -1:
                position = 1
                direction = 1
            elif position == width:
                position = width-2
                direction = -1

            # Fade the old data
            for j in range(width):
                imageData[3*j] = int(fadeFactor * imageData[3*j])
                imageData[3*j+1] = int(fadeFactor * imageData[3*j+1])
                imageData[3*j+2] = int(fadeFactor * imageData[3*j+2])

            # Insert new data
            imageData[3*position] = color[0]
            imageData[3*position+1] = color[1]
            imageData[3*position+2] = color[2]
        
        # Sleep for a while
        time.sleep(sleepTime)

print("Failure: command not found .");
