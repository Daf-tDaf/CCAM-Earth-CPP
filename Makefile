# Library Locations
LIBDAISY_DIR = libDaisy
DAISYSP_DIR = DaisySP
CCAM_DIR = libCCAM

CCAM_ROOT=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

ifndef CCAM_PROJECT
$(error invalid CCAM_PROJECT)
else
include $(CCAM_PROJECT)/Makefile
endif

include $(CCAM_DIR)/Makefile

# Since this is the very first target
# it will be the default
ccam_main: $(LIB_CCAM)
	cd $(DAISYSP_DIR) && make
	cd $(LIBDAISY_DIR) && make
	$(MAKE) all


# Core location, and generic Makefile.
C_INCLUDES = -I$(CCAM_DIR)
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

.PHONY: deepclean
deepclean:
	cd $(DAISYSP_DIR) && $(MAKE) clean
	cd $(LIBDAISY_DIR) && $(MAKE) clean
	rm -rf build
	rm -f json/*.h