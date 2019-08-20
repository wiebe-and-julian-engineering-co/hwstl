// This file should be used by the linker to determine the correct target to use.

#ifdef TARGET_CHIP_ARM_SAM3X8E
#include "arm-sam3x8e/core.hpp"
#endif

#error "No target chip has been selected!"