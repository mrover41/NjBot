#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "command.h"

static icommand *commands;
static size_t buff_count = 0;

int rgcommand(const char *name, uint32_t access_level, uint64_t *access_roles, size_t rcnt, const command_ops *ops) { //command_ops must be static const, but i`m can`t check ths.
																													  //all that remains for me is to trust the module.
																													  //TODO: make macros
	static uint64_t lastId = 0;

	icommand *current_cmd;
	if (commands == NULL) {
		if ((commands = malloc(sizeof(icommand))) == NULL) return -1;
		current_cmd = commands;
		buff_count = 1;
	} else {
		buff_count++;

		void *tmp_c;
		if((tmp_c = realloc(commands, sizeof(icommand) * buff_count)) == NULL) return -1;
		commands = tmp_c;

		current_cmd = commands;
		current_cmd = commands + (buff_count - 1);
	}

	lastId++;
	*current_cmd = (icommand) {
		.id = lastId,
		.access_level = access_level,
		.name = strdup(name),
		.access_roles = access_roles,
		.roles_count = rcnt,
		.vtable = ops,
	};

	return lastId;
}

/*void urgcommand(uint32_t id) {
	
}*/

void on_msg_create(struct discord *client, const struct discord_message *msg) { //TODO: check mod
	if (msg->author->bot) return;

	char *content = msg->content;
	if(*content != '!') return;

	char *content_cpy = strdup(content);
	char *cmd_name = strtok(content_cpy + 1, " \t\r\n");

	icommand *current = commands;
	for (size_t i = 0; i < buff_count; i++) {
		if (strcmp(cmd_name, current->name) == 0) {
			char *content_copy = strdup(msg->content);
			char *args[ARG_BUFFER];
			size_t arg_cnt = 0;

			char *tokn = strtok(content_copy, " ");
			while (tokn != NULL && arg_cnt < ARG_BUFFER) {
				args[arg_cnt] = tokn;
				arg_cnt++;
				tokn = strtok(NULL, " ");
			}


			command_context context = {
				.message_id = msg->id,
				.channel_id = msg->channel_id,
				.guild_id = msg->guild_id,
				.author_id = msg->author ? msg->author->id : 0,
				.author_roles = (const uint64_t *)msg->member->roles->array,
				.author_roles_count = (size_t)msg->member->roles->size,
				.args = (const char **)args,
				.args_count = arg_cnt,
				.discord_client = client,
			};
			free(content_cpy);
			free(content_copy);
			current->vtable->execute(current, &context);
			return;
		} else {
			current ++;
		}
	}
}
