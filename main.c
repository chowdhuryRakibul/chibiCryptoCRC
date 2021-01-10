/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "crc.h"
#include <string.h>
#include "ff.h"


#define FILE_NAME "aXXXXXX.BIN"

char readBuff[1024];

FIL currentFile;

FATFS SDC_FS;

//SDIO configuration
static const SDCConfig sdccfg = {
  SDC_MODE_4BIT
};

void initUART(void)
{
  sdStart(&SD6, NULL);//Baud rate = 57600
  palSetPadMode(GPIOC,6U,PAL_MODE_ALTERNATE(8));
  palSetPadMode(GPIOC,7U,PAL_MODE_ALTERNATE(8));
}


int myPrintf(const char *fmt, ...) {
  va_list ap;
  int formatted_bytes = 0;

  va_start(ap, fmt);
  formatted_bytes = chvprintf((BaseSequentialStream*)&SD6, fmt, ap);
  va_end(ap);

  return formatted_bytes;
}


void initSdCard(const SDCConfig *config, FATFS *SDC_FS)
{
  sdcInit();
  sdcStart(&SDCD1, config);

  sdcConnect(&SDCD1);

  myPrintf("OK\r\n\r\nCard Info\r\n");
  myPrintf("CSD      : %08X %8X %08X %08X \r\n",
           SDCD1.csd[3], SDCD1.csd[2], SDCD1.csd[1], SDCD1.csd[0]);
  myPrintf("CID      : %08X %8X %08X %08X \r\n",
           SDCD1.cid[3], SDCD1.cid[2], SDCD1.cid[1], SDCD1.cid[0]);
  myPrintf("Capacity : %DMB\r\n", SDCD1.capacity / 2048);

  f_mount(SDC_FS, "/", 1);
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  initUART();
  myPrintf("Bismillah\r\n");

  uint16_t buffSize = 1024;
  uint16_t bytesRead;

  initSdCard(&sdccfg, &SDC_FS);

  f_open(&currentFile,FILE_NAME,FA_READ);

  uint32_t crc = -1;

  while(1)
  {
    f_read(&currentFile, readBuff, buffSize, &bytesRead);
    if(!f_eof(&currentFile))
    {
      crc = crc32_update(readBuff,bytesRead,crc);
    }
    else
    {
      crc = crc32_final(readBuff,bytesRead,crc);
      break;
    }
  }

  myPrintf("0x%X\r\n",crc);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
