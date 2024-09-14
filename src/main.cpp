#include <iostream>

#include "FBFunctions.h"
#include "ILI9488Functions.h"



int main()
{
    // Create an instance of FBFunctions, specifying the framebuffer device (e.g., "/dev/fb0")
    FBFunctions fbFunctions("/dev/fb0");

    // Optional: check if fb_width and fb_height are correctly initialized
    std::cout << "Framebuffer Width: " << fbFunctions.fb_width << std::endl;
    std::cout << "Framebuffer Height: " << fbFunctions.fb_height << std::endl;

    // Save framebuffer content to memory
    fbFunctions.saveBufferToMem();

    ILI9488 display;

    // Initialize the ILI9488 display
    display.initDisplay();

    // Set the drawing area (entire screen, for example, 320x480)
    display.setAddressWindow(0, 0, 319, 479);

    // Create a framebuffer (for example, a solid color or image data)
    uint8_t framebuffer[320 * 480 * 3];  // Replace with actual framebuffer data

    // Send the framebuffer to the display
    display.sendFramebuffer(framebuffer, sizeof(framebuffer));

    fbFunctions.deleteBufferFromMem();

    return 0;
}