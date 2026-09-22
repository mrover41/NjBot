#ifndef MODULE_H
#define MODULE_H

#include <stdint.h>

typedef struct module_meta_t module_meta_t;

struct module_meta_t;

typedef struct {
	uint64_t id;
	const module_meta_t *mod_meta;
	void *handle;
} module;

struct module_meta_t {
	const char *name;
	const int (*init) (void);
	const void (*exit) (void);
};

#define REG_MODULE(mod_name, init_fn, exit_fn) \
    module_meta_t CURRENT_MODULE_INFO = {          \
        .name = mod_name,                          \
        .init = init_fn,                           \
        .exit = exit_fn                            \
    }

#endif
