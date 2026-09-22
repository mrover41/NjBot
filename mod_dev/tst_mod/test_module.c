#include <stdio.h>

#include "module.h"

static int on_init(void) {
	puts("[TEST_MOD] Loaded");
	return 0;
}

static void on_exit(void) {
	puts("[TEST_MOD] Unloaded");
}

REG_MODULE("test_module", on_init, on_exit);
