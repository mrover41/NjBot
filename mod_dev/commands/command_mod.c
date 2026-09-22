#include <stdio.h>

#include "module.h"

static int on_init(void) {
	puts("[COMMAND_MOD] Loaded");
}

static void on_exit(void) {
	puts("[COMMAND_MOD] Unloaded");
}

REG_MODULE("test_module", on_init, on_exit);
