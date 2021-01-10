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

  const unsigned char  test[] = "123456789";
  const unsigned char newTest[] = "abcdefghi";

  myPrintf("0x%X\r\n",uzlib_crc32(test,strlen(test),-1));



  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
