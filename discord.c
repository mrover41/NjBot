#include <stddef.h>
#include <concord/discord.h>

#include "discord.h"

struct discord *client = NULL;

int disinit() {
	client = discord_config_init(CONFIG_FILE);
	if (!client) return -1;
	discord_run(client);

	return 0;
}

void disexit() {
	discord_cleanup(client);
}

bool disget_client(struct discord **out) {
	return (*out = client) != NULL;
}
