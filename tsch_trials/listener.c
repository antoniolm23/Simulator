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
	
	random_init();
	
	//we can use only 16 channel offsets
	unsigned int short channelOffset = random_rand();
	channelOffset = random_rand() % 16;
	
	//busy wait
	int i = 0;
	for(i = 0; i < 65536; i++);
	
	/*
	 * can't use any of the receiving functions defined in tsch.c
	 * since all those function assume a synchronization that I
	 * can't assume
	 */
	
	hop_channel(&current_asn, channelOffset);
	
	//turn radio on
	on();
	
	//wait for an EB packet to arrive
	
	uint8_t eb_arrived = 0;
	while(!NETSTACK_RADIO.pending_packet() && eb_arrived) {
		eb_arrived = tsch_packet_parse_frame( (uint8_t*) 
			current_input->payload, current_input->len, &seqno)
			& IS_EB;
		if(eb_arrived == 1)
			PROCEES_END();
	}
	
	off();
	
	PROCESS_END();
}
