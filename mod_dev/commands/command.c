#include <stdlib.h>
#include <stdint.h>

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
		.name = name,
		.access_roles = access_roles,
		.roles_count = rcnt,
		.vtable = ops,
	};

	//TODO: reg ops->execute and ops->on_msg_delete and add command to discord

	return lastId;
}

/*void urgcommand(uint32_t id) {
	
}*/
