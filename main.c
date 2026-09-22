#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "module.h"
#include "discord.h"

static const char *path = "./modules/";

int main() {
	disinit();

	DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;

	uint64_t x = 0;
	while ((entry = readdir(dir)) != NULL) {
		char *ext = strrchr(entry->d_name, '.') + 1;
		if (ext == NULL) continue;
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(ext, "so") != 0) {
            continue;
        }
		x++;
	}

	rewinddir(dir);

	module *arr = calloc(x, sizeof(module));
	
	x = 0;
	module *current = arr;
    while ((entry = readdir(dir)) != NULL) {
		char full_path[1024];
		
		char *ext = strrchr(entry->d_name, '.') + 1;
		if (ext == NULL) continue;
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(ext, "so") != 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s%s", path, entry->d_name);

		void *handle = dlopen(full_path, RTLD_NOW);
		if (!handle) {
			perror("module open error");

			free(arr);
		    closedir(dir);
			return -1;
		};
		
		dlerror();
		
		module_meta_t *meta = (module_meta_t *)dlsym(handle, "CURRENT_MODULE_INFO");
		

		const char *err = dlerror();

		if (err) {
			dlclose(handle);
			perror("module load error");
			
			free(arr);
		    closedir(dir);
			return -1;
		}

		*current = (module) {
			.id = x,
			.mod_meta = meta,
			.handle = handle,
		};
		current++;
		x++;

		fprintf(stdout, "Module %s registered\n", meta->name);
    }

	current = arr;

	for (uint64_t i = 0; i < x; i++) {
		if (current->mod_meta->init() < 0) {
			fprintf(stderr, "Module %s not loaded", current->mod_meta->name);
			current->mod_meta->exit();
			continue;
		};
		fprintf(stdout, "Module %s loaded\n", current->mod_meta->name);
		current++;
	}

	for (;;) {
		char buf[256];
		ssize_t nread = read(0, buf, sizeof(buf) - 1);

		if (nread <= 0) break;
		buf[nread] = '\0';
		buf[strcspn(buf, "\r\n")] = '\0';

		if (strcmp(buf, "exit") == 0 || strcmp(buf, "q") == 0) break;
	}

	current = arr;
	for (uint64_t i = 0; i < x; i++) {
		current->mod_meta->exit();
		fprintf(stdout, "Module %s unloaded\n", current->mod_meta->name);
		dlclose(current->handle);
		current++;
	}

	disexit();

	free(arr);
    closedir(dir);
	return 0;
}
