#include "img.h"
#include "mbox.h"
#include "uart.h"

unsigned int w, h, pitch, isrgb; /* dimensions and channel order */
unsigned char *lfb;              /* raw frame buffer address */

/**
 * Set screen resolution to 800x480
 */
void lfb_init()
{
  mbox[0] = 35 * 4;
  mbox[1] = MBOX_REQUEST;

  mbox[2] = 0x48003; // set phy wh
  mbox[3] = 8;
  mbox[4] = 8;
  mbox[5] = 800; // FrameBufferInfo.width
  mbox[6] = 480; // FrameBufferInfo.height

  mbox[7] = 0x48004; // set virt wh
  mbox[8] = 8;
  mbox[9] = 8;
  mbox[10] = 800; // FrameBufferInfo.virtual_width
  mbox[11] = 480; // FrameBufferInfo.virtual_height

  mbox[12] = 0x48009; // set virt offset
  mbox[13] = 8;
  mbox[14] = 8;
  mbox[15] = 0; // FrameBufferInfo.x_offset
  mbox[16] = 0; // FrameBufferInfo.y.offset

  mbox[17] = 0x48005; // set depth
  mbox[18] = 4;
  mbox[19] = 4;
  mbox[20] = 32; // FrameBufferInfo.depth

  mbox[21] = 0x48006; // set pixel order
  mbox[22] = 4;
  mbox[23] = 4;
  mbox[24] = 0; // RGB, not BGR preferably

  mbox[25] = 0x40001; // get framebuffer, gets alignment on request
  mbox[26] = 8;
  mbox[27] = 8;
  mbox[28] = 4096; // FrameBufferInfo.pointer
  mbox[29] = 0;    // FrameBufferInfo.size

  mbox[30] = 0x40008; // get pitch
  mbox[31] = 4;
  mbox[32] = 4;
  mbox[33] = 0; // FrameBufferInfo.pitch

  mbox[34] = MBOX_TAG_LAST;

  // this might not return exactly what we asked for, could be
  // the closest supported resolution instead
  if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0)
  {
    mbox[28] &= 0x3FFFFFFF; // convert GPU address to ARM address
    w = mbox[5];
    h = mbox[6];
    pitch = mbox[33];
    isrgb = mbox[24];
    lfb = (void *)((unsigned long)mbox[28]);
  }
  else
  {
    uart_puts("Unable to set screen resolution to 800x480x32\n");
  }
}

/**
 * Show a picture
 */
void lfb_showpicture()
{
  int x, y;
  unsigned char *ptr = lfb;
  char *data = header_data, pixel[4];

  ptr += (h - height) / 2 * pitch + (w - width) * 2;

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      HEADER_PIXEL(data, pixel);
      // the image is in RGB. So if we have an RGB framebuffer, we can copy the pixels
      // directly, but for BGR we must swap R (pixel[0]) and B (pixel[2]) channels.
      *((unsigned int *)ptr) = isrgb ? *((unsigned int *)&pixel) : (unsigned int)(pixel[0] << 16 | pixel[1] << 8 | pixel[2]);
      ptr += 4;
    }
    ptr += pitch - width * 4;
  }
}
