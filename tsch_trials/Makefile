ifndef TARGET
TARGET = sky
endif
#DEVICE_NAME ?= DR1198
APPS=servreg-hack
UIP_CONF_IPV6=1

CONTIKI_PROJECT = advertiser 
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
SMALL = 1

all: $(CONTIKI_PROJECT)

CONTIKI = ../..
include $(CONTIKI)/Makefile.include

