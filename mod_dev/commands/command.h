#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>
#include <stddef.h>
#include <concord/discord.h>

#define ARG_BUFFER 32

typedef struct icommand icommand;

typedef struct {
	uint64_t message_id;
    uint64_t channel_id;
    uint64_t guild_id;
    uint64_t author_id;
    
    const uint64_t *author_roles;
    size_t author_roles_count;

    const char **args;
    size_t args_count;

    struct discord *discord_client;
} command_context;


typedef struct {
	int (*execute)(const icommand *, const command_context *);
	void (*on_msg_delete)(const icommand *, const struct discord_message *msg);
} command_ops;

struct icommand {
	uint32_t id;
	uint32_t access_level;

	size_t roles_count;
	uint64_t *access_roles;


	char *name;

	void *owner;
	const command_ops *vtable;
};

int rgcommand(const char *, uint32_t, uint64_t *, size_t, const command_ops *);
//void urgcommand(uint32_t);


void on_msg_create(struct discord *client, const struct discord_message *msg);

#endif
