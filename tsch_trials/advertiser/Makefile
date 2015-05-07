ifndef TARGET
TARGET = sky
endif
#DEVICE_NAME ?= DR1198
APPS=servreg-hack
UIP_CONF_IPV6=1

#ifndef CELL
#CELL = 1
#endif

#ifndef P_COLLISIONS
#P_COLLISIONS = 0
#endif

ifdef T1
ifdef C1

CFLAGS += -D TS1=$(T1) -D CH1=$(C1)

endif
endif

ifdef T2
ifdef C2

CFLAGS += -D TS2=$(T2) -D CH2=$(C2)

endif
endif

CONTIKI_PROJECT = advertiser 
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\" #-D CELLS=$(CELL) -D CONF_PROB_COLLISIONS=$(P_COLLISIONS)
SMALL = 1

all: $(CONTIKI_PROJECT)

CONTIKI = ../../..
include $(CONTIKI)/Makefile.include

