#include <stdint.h>

#include "module.h"
#include "command.h"
#include "discord.h"

int commandHandler(const icommand *, const command_context *);

static int on_init() {
	puts("[PING_MODULE] Loading...");
	static const command_ops cops = {
		.execute = commandHandler,
	};

	int id = 0;
	if((id = rgcommand("ping", 0, 0, 0, &cops)) < 0) return -1;
	printf("[PING_MODULE] Command retistered, id: %i", id);

	puts("[PING_MODULE] Loaded");
	return 0;
}

static void on_exit() {
	puts("[PING_MODULE] Unloading...");
}

int commandHandler(const icommand *icmd, const command_context *cmd_context) {
	struct discord_create_message params = {
		.content = "pong"
	};

	discord_create_message(cmd_context->discord_client, cmd_context->channel_id, &params, NULL);
}

REG_MODULE("ping_module", on_init, on_exit);
