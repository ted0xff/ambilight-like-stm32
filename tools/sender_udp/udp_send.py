#!/usr/bin/python3
#
# takes 4 arguments : 32bits idx  + rgb as decimal
# n'implémente qu'un pseudo protocol !
# envoi un seul pixel à la fois

import time
import struct
import sys
import socket

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM);

## 4 arguments :  idx 32bits, r,g,b 8bits
def setPixel(idx, r,g,b):
    pix_count=1
    pix_pos=idx
    message=struct.pack( '<3B2h4B', ord('A'),ord('d'),ord('A'), pix_count, pix_pos, 0xA5 , r,g,b );
    sock.sendto(message , ("127.0.0.1",1200) );


# applique la commande all pixels 1 couleur
def setAllPixels(r,g,b):
    cmde=1
    message=struct.pack( '<3B1h3B', ord('A'),ord('d'),ord('B'), cmde, r,g,b );
    sock.sendto(message , ("127.0.0.1",1200) );



setAllPixels( int(sys.argv[1]), int(sys.argv[2]),int(sys.argv[3]) );
quit()

## call
setPixel( int(sys.argv[1]), int(sys.argv[2]),int(sys.argv[3]),int(sys.argv[4]));


## faire une animation simple
for x in range(30, 40):
    time.sleep(0.2);
    setPixel( x, int(sys.argv[2]),int(sys.argv[3]),int(sys.argv[4]) );
