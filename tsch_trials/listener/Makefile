ifndef TARGET
TARGET = sky
endif
#DEVICE_NAME ?= DR1198
APPS=servreg-hack
UIP_CONF_IPV6=1

ifndef SEED
SEED = 12
endif

CONTIKI_PROJECT = listener 
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\" -D CONF_SEED=$(SEED)
SMALL = 1



all: $(CONTIKI_PROJECT)

CONTIKI = ../../..
include $(CONTIKI)/Makefile.include

