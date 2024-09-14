#include "FBFunctions.h"



FBFunctions::FBFunctions(const char* fb_device)
{
    fbdev = const_cast<char*>(fb_device);
    fbfd = open(fbdev, O_RDWR);
    if (fbfd < 0)
    {
        std::cerr << "Error opening framebuffer device: " << fbdev << std::endl;
        return;
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0)
    {
        std::cerr << "Error reading variable information from framebuffer." << std::endl;
        close(fbfd);
        return;
    }

    fb_width = vinfo.xres;
    fb_height = vinfo.yres;
    fb_bpp = vinfo.bits_per_pixel;
    fb_bytes = fb_bpp / 8;
    fb_data_size = fb_width * fb_height * fb_bytes;
}



void FBFunctions::saveBufferToMem()
{
    fbfd = open(fbdev, O_RDWR);
    fbdata = (char *)mmap(0, fb_data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
}



void FBFunctions::deleteBufferFromMem()
{
    munmap(fbdata, fb_data_size); 
}