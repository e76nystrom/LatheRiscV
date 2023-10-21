# Modify this variable to fit your NEORV32 setup (neorv32 home folder)
NEORV32_HOME ?= ../../..
$(info $(NEORV32_HOME))

PLATFORM := $(shell uname)
$(info $(PLATFORM))

ifeq ($(PLATFORM), Linux)
include $(NEORV32_HOME)/sw/common/common.mk
else
include $(NEORV32_HOME)/sw/common/common1.mk
endif
