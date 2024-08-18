#include "fb.h"
#include "../utils/colors.h"
#include "mbox.h"
#include "uart.h"

/* Scalable Screen Font (https://gitlab.com/bztsrc/scalable-font2) */
typedef struct
{
  unsigned char magic[4];
  unsigned int size;
  unsigned char type;
  unsigned char features;
  unsigned char width;
  unsigned char height;
  unsigned char baseline;
  unsigned char underline;
  unsigned short fragments_offs;
  unsigned int characters_offs;
  unsigned int ligature_offs;
  unsigned int kerning_offs;
  unsigned int cmap_offs;
} __attribute__((packed)) sfn_t;
extern volatile unsigned char _binary_font_sfn_start;

unsigned int width, height, pitch;
unsigned char *fb;

/**
 * Set screen resolution to 800x480
 */
void fb_init()
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
  mbox[24] = 1; // RGB, not BGR preferably

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

  if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0)
  {
    mbox[28] &= 0x3FFFFFFF;
    width = mbox[5];
    height = mbox[6];
    pitch = mbox[33];
    fb = (void *)((unsigned long)mbox[28]);
  }
  else
  {
    uart_puts("Unable to set screen resolution to 800x480x32\n");
  }
}

/**
 * Display a string using proportional SSFN
 */
void fb_text(int x, int y, char *s, color_t col, unsigned short int fs)
{
  sfn_t *font = (sfn_t *)&_binary_font_sfn_start;
  unsigned char *ptr, *chr, *frg;
  unsigned int c;
  unsigned long o, p;
  int i, j, k, l, m, n;

  while (*s)
  {
    if ((*s & 128) != 0)
    {
      if (!(*s & 32))
      {
        c = ((*s & 0x1F) << 6) | (*(s + 1) & 0x3F);
        s += 1;
      }
      else if (!(*s & 16))
      {
        c = ((*s & 0xF) << 12) | ((*(s + 1) & 0x3F) << 6) | (*(s + 2) & 0x3F);
        s += 2;
      }
      else if (!(*s & 8))
      {
        c = ((*s & 0x7) << 18) | ((*(s + 1) & 0x3F) << 12) | ((*(s + 2) & 0x3F) << 6) | (*(s + 3) & 0x3F);
        s += 3;
      }
      else
        c = 0;
    }
    else
      c = *s;
    s++;

    if (c == '\r')
    {
      x = 0;
      continue;
    }
    else if (c == '\n')
    {
      x = 0;
      y += font->height * fs;
      continue;
    }

    // Find glyph, look up "c" in Character Table.
    for (ptr = (unsigned char *)font + font->characters_offs, chr = 0, i = 0; i < 0x110000; i++)
    {
      if (ptr[0] == 0xFF)
      {
        i += 65535;
        ptr++;
      }
      else if ((ptr[0] & 0xC0) == 0xC0)
      {
        j = (((ptr[0] & 0x3F) << 8) | ptr[1]);
        i += j;
        ptr += 2;
      }
      else if ((ptr[0] & 0xC0) == 0x80)
      {
        j = (ptr[0] & 0x3F);
        i += j;
        ptr++;
      }
      else
      {
        if ((unsigned int)i == c)
        {
          chr = ptr;
          break;
        }
        ptr += 6 + ptr[1] * (ptr[0] & 0x40 ? 6 : 5);
      }
    }

    if (!chr)
      continue;

    // Uncompress and display fragments.
    ptr = chr + 6;
    o = (unsigned long)fb + (y * fs) * pitch + x * (4 * fs);

    for (i = n = 0; i < chr[1]; i++, ptr += chr[0] & 0x40 ? 6 : 5)
    {
      if (ptr[0] == 255 && ptr[1] == 255)
        continue;

      frg = (unsigned char *)font + (chr[0] & 0x40 ? ((ptr[5] << 24) | (ptr[4] << 16) | (ptr[3] << 8) | ptr[2]) : ((ptr[4] << 16) | (ptr[3] << 8) | ptr[2]));

      if ((frg[0] & 0xE0) != 0x80)
        continue;

      o += (int)(ptr[1] - n) * pitch * fs;
      n = ptr[1];
      k = ((frg[0] & 0x1F) + 1) << 3;
      j = frg[1] + 1;
      frg += 2;

      for (m = 1; j; j--, n++, o += pitch * fs)
        for (p = o, l = 0; l < k; l++, p += (4 * fs), m <<= 1)
        {
          if (m > 0x80)
          {
            frg++;
            m = 1;
          }

          if (*frg & m)
          {
            for (unsigned short int yy = 0; yy < fs * 800; yy += 800)
            {
              for (unsigned short int xx = 0; xx < fs; xx++)
              {
                *((unsigned int *)p + xx + yy) = ((col.r & 0xff) << 16) + ((col.g & 0xff) << 8) + (col.b & 0xff);
              }
            }
          }
        }
    }

    // Add advances.
    x += chr[4] + 1;
    y += chr[5];
  }
}

void fb_rect(int x, int y, unsigned int w, unsigned int h, color_t col)
{
  unsigned char *ptr = fb;
  int p = x * 4 + (y * 800 * 4);

  for (unsigned int i = 0; i < w * 4; i += 4)
  {
    for (unsigned int j = 0; j < h * 4; j += 4)
    {
      ptr[p + i + (j * 800)] = col.r;
      ptr[p + i + (j * 800) + 1] = col.g;
      ptr[p + i + (j * 800) + 2] = col.b;
      ptr[p + i + (j * 800) + 3] = 255;
    }
  }
}
