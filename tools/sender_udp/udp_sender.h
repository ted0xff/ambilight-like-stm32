#ifndef __UDP_SENDER__H_
#define __UDP_SENDER__H_

/*
UDP port 1200 vers un serveur de rendu graphique

*/

#include <stdint.h>  

/* pratique, même si pas utilisé dans render.... */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixel_t;


int  renderInit(void);

void renderSetpixel(int socket, uint16_t idx, uint8_t r, uint8_t g, uint8_t b);
void renderSetAllpixels(int socket, uint8_t r, uint8_t g, uint8_t b);

void getpixel(uint32_t idx, uint8_t *r, uint8_t *g, uint8_t *b);

void renderClose(int *socket);

#endif /* __UDP_SENDER__H_ */

