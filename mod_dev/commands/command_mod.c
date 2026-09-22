#include <stdio.h>

#include "module.h"
#include "command.h"
#include "discord.h"

static struct discord *client;

static int on_init(void) {
	puts("[COMMAND_MODULE] Loading...");
	if (!disget_client(&client)) return -1;
	discord_set_on_message_create(client, &on_msg_create);

	puts("[COMMAND_MODULE] Loaded");
	return 0;
}

static void on_exit(void) {
	puts("[COMMAND_MODULE] Unloading...");
}

REG_MODULE("command_module", on_init, on_exit);
