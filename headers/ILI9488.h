#ifndef ILI9488_H
#define ILI9488_H

#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <iostream>

class ILI9488 {
public:
    ILI9488();

    void initDisplay();

    void setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    void sendFramebuffer(uint8_t* buffer, int length);
private:
    void sendCommand(uint8_t cmd);

    void sendData(uint8_t* data, int len);

    void initSPIandGPIO();
};

#endif