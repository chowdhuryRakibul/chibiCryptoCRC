//https://github.com/pfalcon/uzlib

#include <chtypes.h>

uint32_t crc32_update(const void *data, unsigned int length, uint32_t crc);
uint32_t crc32_final(const void *data, unsigned int length, uint32_t crc);
