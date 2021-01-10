//https://github.com/pfalcon/uzlib
#include "crc.h"

static const uint32_t crc32tab[16] = {
   0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190,
   0x6b6b51f4, 0x4db26158, 0x5005713c, 0xedb88320, 0xf00f9344,
   0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278,
   0xbdbdf21c
};

/* crc is previous value for incremental computation, 0xffffffff (-1) initially */
uint32_t crc32_update(const void *data, unsigned int length, uint32_t crc)
{
   const unsigned char *buf = (const unsigned char *)data;
   uint16_t i;

   for (i = 0; i < length; ++i)
   {
      crc ^= buf[i];
      crc = crc32tab[crc & 0x0f] ^ (crc >> 4);
      crc = crc32tab[crc & 0x0f] ^ (crc >> 4);
   }

   // return value suitable for passing in next time, for final value invert it
   return crc;
}


/* calculates final crc value, crc is 0xffffffff initially if no prev crc exist */
uint32_t crc32_final(const void *data, unsigned int length, uint32_t crc)
{
   return crc32_update(data, length, crc) ^ 0xffffffff;
}
