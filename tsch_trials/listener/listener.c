/*
 * This is the listener, by definition, the listener 
 * wakes up randomly, then it pics a random channel and 
 * waits for a message to arrive
 */

#undef WITH_COMPOWER
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "lib/random.h"

#include "net/netstack.h"
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-schedule.h"
#include <string.h>
#include "cooja-debug.h"
#define DEBUG DEBUG_PRINT

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"
#if DEBUG
#if ENABLE_COOJA_DEBUG
#include "cooja-debug.h"
#undef PRINTF
#undef PRINT6ADDR
#define PRINTF COOJA_DEBUG_PRINTF
#define PRINT6ADDR COOJA_DEBUG_ADDR16
#endif /* ENABLE_COOJA_DEBUG */
#else
#undef PRINTF
#undef PRINT6ADDR
#define PRINTF(...)
#define PRINT6ADDR(...)
#endif /* DEBUG */

PROCESS(listener, "LISTENER process");
AUTOSTART_PROCESSES(&listener);

PROCESS_THREAD(listener, ev, data)
{ 
  
  PROCESS_BEGIN();
  
  
  
	PROCESS_PAUSE();
	
	while(1) 
  {
    PROCESS_YIELD();
  }
	
	PROCESS_END();
}
