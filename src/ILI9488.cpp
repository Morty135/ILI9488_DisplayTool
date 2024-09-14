#include "ILI9488.h"



#define ILI9488_CMD_SWRESET 0x01    // Software reset
#define ILI9488_CMD_SLPOUT  0x11    // Sleep out
#define ILI9488_CMD_DISPON  0x29    // Display on
#define ILI9488_CMD_CASET   0x2A    // Column address set
#define ILI9488_CMD_RASET   0x2B    // Row address set
#define ILI9488_CMD_RAMWR   0x2C    // Memory write

// Define your SPI channel and GPIO pins
#define SPI_CHANNEL 0
#define SPI_SPEED   10000000 // 10MHz

// Define GPIO pins for D/C, RESET, and CS
#define GPIO_DC     24   // Data/Command pin
#define GPIO_RST    25   // Reset pin
#define GPIO_CS     8    // Chip Select pin (CE0)

// Constructor to initialize the SPI and GPIO
ILI9488::ILI9488() {
    initSPIandGPIO();
}

// Initialize the SPI interface and GPIO pins
void ILI9488::initSPIandGPIO() {
    // Initialize SPI
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) {
        std::cerr << "Failed to setup SPI" << std::endl;
        exit(1);
    }

    // Initialize GPIO (D/C and RESET)
    wiringPiSetupGpio();  // Use GPIO numbering

    pinMode(GPIO_DC, OUTPUT);
    pinMode(GPIO_RST, OUTPUT);
    pinMode(GPIO_CS, OUTPUT);

    // Reset the display
    digitalWrite(GPIO_RST, LOW);
    usleep(100000); // 100ms delay
    digitalWrite(GPIO_RST, HIGH);
    usleep(100000); // 100ms delay
}

// Send command to ILI9488
void ILI9488::sendCommand(uint8_t cmd) {
    digitalWrite(GPIO_DC, LOW);  // Command mode
    digitalWrite(GPIO_CS, LOW);  // Enable SPI device
    wiringPiSPIDataRW(SPI_CHANNEL, &cmd, 1);
    digitalWrite(GPIO_CS, HIGH); // Disable SPI device
}

// Send data to ILI9488
void ILI9488::sendData(uint8_t* data, int len) {
    digitalWrite(GPIO_DC, HIGH); // Data mode
    digitalWrite(GPIO_CS, LOW);  // Enable SPI device
    wiringPiSPIDataRW(SPI_CHANNEL, data, len);
    digitalWrite(GPIO_CS, HIGH); // Disable SPI device
}

// Initialize the ILI9488 display
void ILI9488::initDisplay() {
    sendCommand(ILI9488_CMD_SWRESET);  // Software reset
    usleep(150000);                    // Wait 150ms after reset

    sendCommand(ILI9488_CMD_SLPOUT);   // Sleep out
    usleep(500000);                    // Wait 500ms

    sendCommand(ILI9488_CMD_DISPON);   // Turn on display
    usleep(500000);                    // Wait 500ms
}

// Set the address window for drawing (x0, y0, x1, y1)
void ILI9488::setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    // Column address set (x0, x1)
    sendCommand(ILI9488_CMD_CASET);
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (x1 >> 8) & 0xFF;
    data[3] = x1 & 0xFF;
    sendData(data, 4);

    // Row address set (y0, y1)
    sendCommand(ILI9488_CMD_RASET);
    data[0] = (y0 >> 8) & 0xFF;
    data[1] = y0 & 0xFF;
    data[2] = (y1 >> 8) & 0xFF;
    data[3] = y1 & 0xFF;
    sendData(data, 4);
}

// Send pixel data to the display
void ILI9488::sendFramebuffer(uint8_t* buffer, int length) {
    sendCommand(ILI9488_CMD_RAMWR); // Memory write
    sendData(buffer, length);       // Send pixel data
}