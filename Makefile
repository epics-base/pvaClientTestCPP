#Makefile at top of application tree

TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure

DIRS += test
test_DEPEND_DIRS = configure

DIRS += database

include $(TOP)/configure/RULES_TOP

