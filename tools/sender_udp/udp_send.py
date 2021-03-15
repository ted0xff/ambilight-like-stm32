#!/usr/bin/python3
#
# takes 4 arguments : 32bits idx  + rgb as decimal
# n'implémente qu'un pseudo protocol !
# envoi un seul pixel à la fois

import time
import struct
import sys
import socket

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

if sys.argv[1] == "anim":
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

print("Failure: command not found .");
