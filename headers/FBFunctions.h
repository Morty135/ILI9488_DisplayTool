#ifndef FBFunctions_H
#define FBFunctions_H

#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h> 
#include <unistd.h>
#include <iostream>

class FBFunctions
{
    public:
        int fb_width;
        int fb_height;
        int fb_bpp;
        int fb_bytes;
        int fb_data_size;

        char *fbdev;

        char *fbdata;

        FBFunctions(const char* fb_device);
        void saveBufferToMem();

        void deleteBufferFromMem();
    private:
        int fbfd;
};



#endif