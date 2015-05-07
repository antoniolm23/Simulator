/*
 * First example of an advertiser that sends EB 
 * on timeslot 0 at channel 0 and on timeslot 2
 * at channel 1 on a frameslot of 5 timeslots
 */
//FIXME commented line 1471 of tsch.c plus other modifications in tsch.c

//#undef WITH_COMPOWER
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "net/netstack.h"
#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-schedule.h"
#include <string.h>

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

PROCESS(advertising, "Advertising process");
AUTOSTART_PROCESSES(&advertising);

PROCESS_THREAD(advertising, ev, data)
{
	PROCESS_BEGIN();
	PROCESS_PAUSE();
	
	//create a new frameslot
	sf_eb = tsch_schedule_add_slotframe(0, 3);
	
	//add an advertising link at timeslot 0, channel offset 0
	tsch_schedule_add_link(sf_eb, LINK_OPTION_TX | LINK_OPTION_SHARED, 
		LINK_TYPE_ADVERTISING_ONLY, &tsch_broadcast_address, 0, 0);
#ifdef TS1
#ifdef CH1	
  //DUMMY links to print ASN
	tsch_schedule_add_link(sf_eb, LINK_OPTION_TX | LINK_OPTION_SHARED, 
		LINK_TYPE_ADVERTISING_ONLY, &tsch_broadcast_address, TS1, CH1);
#endif
#else
	tsch_schedule_add_link(sf_eb, LINK_OPTION_RX | LINK_OPTION_SHARED,
		LINK_TYPE_ADVERTISING_ONLY, &tsch_broadcast_address, 1, 2);
#endif

#ifdef TS2
#ifdef CH2
	tsch_schedule_add_link(sf_eb, LINK_OPTION_TX | LINK_OPTION_SHARED, 
		LINK_TYPE_ADVERTISING_ONLY, &tsch_broadcast_address, TS2, CH2);
#endif
#else
  
  tsch_schedule_add_link(sf_eb, LINK_OPTION_RX | LINK_OPTION_SHARED, 
		LINK_TYPE_ADVERTISING_ONLY, &tsch_broadcast_address, 2, 1);
#endif	
	//tsch_schedule_print();
	
	//tsch_is_coordinator = 1;
	
  
	while(1)
	{
		PROCESS_YIELD();		
	}
	
	PROCESS_END();
}
