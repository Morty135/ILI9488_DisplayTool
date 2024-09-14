#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdint>
#include <wiringPi.h>
#include "wiringPiSPI.h"

// Function to capture framebuffer
bool capture_framebuffer(const std::string &fb_path, char* &buffer, size_t &size) {
    int fb_fd = open(fb_path.c_str(), O_RDONLY);
    if (fb_fd == -1) {
        std::cerr << "Error opening framebuffer device: " << strerror(errno) << std::endl;
        return false;
    }

    fb_var_screeninfo vinfo;
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
        std::cerr << "Error reading variable information: " << strerror(errno) << std::endl;
        close(fb_fd);
        return false;
    }

    int width = vinfo.xres_virtual;
    int height = vinfo.yres_virtual;
    int bpp = vinfo.bits_per_pixel / 8; // bytes per pixel

    size = width * height * bpp;
    buffer = new char[size];
    if (read(fb_fd, buffer, size) != size) {
        std::cerr << "Error reading framebuffer: " << strerror(errno) << std::endl;
        delete[] buffer;
        close(fb_fd);
        return false;
    }

    close(fb_fd);
    return true;
}

// Function to send data via SPI
bool send_spi(const std::string &spi_path, const char* buffer, size_t size) {
    // Initialize WiringPi library
    if (wiringPiSetup() == -1) {
        std::cerr << "Error initializing WiringPi: " << strerror(errno) << std::endl;
        return false;
    }

    // Open SPI device
    int spi_fd = wiringPiSPISetup(0, 500000); // SPI channel 0, speed 500 kHz
    if (spi_fd < 0) {
        std::cerr << "Error opening SPI device: " << strerror(errno) << std::endl;
        return false;
    }

    // Send data in chunks
    const size_t MAX_CHUNK_SIZE = 4096; // Define a chunk size that fits within the limit
    size_t offset = 0;
    while (offset < size) {
        size_t chunk_size = std::min(size - offset, MAX_CHUNK_SIZE);
        
        // Send chunk of data
        if (wiringPiSPIDataRW(0, (unsigned char*)(buffer + offset), chunk_size) == -1) {
            std::cerr << "Error sending SPI data: " << strerror(errno) << std::endl;
            return false;
        }

        offset += chunk_size;
    }

    return true;
}


int main() {
    char* framebuffer = nullptr;
    size_t framebuffer_size = 0;

    if (!capture_framebuffer("/dev/fb0", framebuffer, framebuffer_size)) {
        return 1;
    }

    if (!send_spi("/dev/spidev0.0", framebuffer, framebuffer_size)) {
        delete[] framebuffer;
        return 1;
    }

    delete[] framebuffer;
    return 0;
}