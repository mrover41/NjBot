#ifndef DIS_H
#define DIS_H

#include <stdbool.h>
#include <concord/discord.h>

#define CONFIG_FILE "./config.json"

struct discord;

int disinit(void);
void disexit(void);
bool disget_client(struct discord **);

#endif
