/*

Envoi des commandes pixel unitaire à un server/render UDP port 1200
pour tests de pattern

lib d'interaction

n'implémente léger protocol ! (1 pixel a la fois en dur)

*/

#include "udp_sender.h"

#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>  
#include <string.h> 

#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 


#define UDP_PORT     1200 

/* rémanent ! */
struct sockaddr_in     servaddr; 

/* protocol AdA*/
typedef struct __attribute__((__packed__)){
    uint8_t  header[3];
    uint16_t count;
    uint16_t idx;
    uint8_t  crc;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixelADACmdStr_t;
#define D_PIXELADACMD_SIZE  ( sizeof(pixelADACmdStr_t) )

/* protocol AdB*/
typedef struct __attribute__((__packed__)){
    uint8_t  header[3];
    uint16_t cmde;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixelADBCmdStr_t;
#define D_PIXELADBCMD_SIZE  ( sizeof(pixelADBCmdStr_t) )

/*stocker les infos car veut pouvoir lire aussi pour certaines anim. !*/
uint8_t pix_array[3 * 65536]; // met un grand buffer taille fixe pour ne pas être dépendant des strings 
// et aussi grand pour tester des soft qui jardinnent ...


/** mon protole n'est pas très efficace, mais est simple */
void renderSetpixel(int socket, uint16_t idx, uint8_t r, uint8_t g, uint8_t b) {
    pixelADACmdStr_t pixel; // AdA

    pixel.header[0] = 'A';
    pixel.header[1] = 'd';
    pixel.header[2] = 'A';
    pixel.idx =idx;
    pixel.count =1; // only 1 pixel everytime !!
    pixel.crc =0x5A;  //crc not managed
    pixel.r =r;
    pixel.g =g;
    pixel.b =b;

    // storage
    pix_array[idx*3  ] = r;
    pix_array[idx*3+1] = g;
    pix_array[idx*3+2] = b;

    //printf("pix %d (0x%02x,0x%02x,0x%02x) - %ldbytes\n", pixel.idx, pixel.r,pixel.g, pixel.b, D_PIXELADACMD_SIZE);  
    sendto(socket, &pixel, D_PIXELADACMD_SIZE, 0 /*MSG_CONFIRM*/, (const struct sockaddr *) &servaddr,  sizeof(servaddr)); 
}

void renderSetAllpixels(int socket, uint8_t r, uint8_t g, uint8_t b) {
    pixelADBCmdStr_t pixel; // AdB

    pixel.header[0] = 'A';
    pixel.header[1] = 'd';
    pixel.header[2] = 'B';
    pixel.cmde =1; // c'est la commande en 'B' : 1 == solid color
    pixel.r = r;
    pixel.g = g;
    pixel.b = b;

#if 0
    // storage: TODO : faut remplir TOUTE l'array ...
    pix_array[idx*3  ] = r;
    pix_array[idx*3+1] = g;
    pix_array[idx*3+2] = b;
#endif
    //printf("pix %d (0x%02x,0x%02x,0x%02x) - %ldbytes\n", pixel.idx, pixel.r,pixel.g, pixel.b, D_PIXELADBCMD_SIZE);  
    sendto(socket, &pixel, D_PIXELADBCMD_SIZE, 0 /*MSG_CONFIRM*/, (const struct sockaddr *) &servaddr,  sizeof(servaddr)); 
}

/** local read value ! */
void getpixel(uint32_t idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = pix_array[idx*3  ];
    *g = pix_array[idx*3+1];
    *b = pix_array[idx*3+2];
}

int renderInit(void){
    int sockfd; 

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(UDP_PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 

    return sockfd;
}

void renderClose(int *socket){
    close(*socket); 
}

