#Makefile at top of application tree

TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure

DIRS += test
test_DEPEND_DIRS = configure database

DIRS += database
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), database)

include $(TOP)/configure/RULES_TOP

