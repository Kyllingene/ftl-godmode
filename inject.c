#define _GNU_SOURCE

#include <ctype.h>

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <dirent.h>
#include <pthread.h>

typedef FILE *(*fopen_t)(const char *pathname, const char *mode);
fopen_t real_fopen;

pthread_t hack_thread;

void* ship;
int* fuel;
int* scrap;
int* hull;

void* weapons;
int* weapons_level;
int* weapons_health;
int* weapons_power_level;
int* missiles;

void* shields;
int* shields_level;
int* shields_health;
int* shields_power_level;
int* shields_online;
int* shields_max;

void* drones;
int* drones_level;
int* drones_power_level;
int* drones_health;
int* drone_parts;

void* power;
int* power_used;
int* power_max;

void* engines;
int* engines_level;
int* engines_health;
int* engines_power_level;

// void* oxygen;
// int* oxygen_level;
// int* oxygen_health;

int update_pointers() {
	ship = *(void**)(0xa3a660 + 0x190);
	if (!ship)
		return 0;
	hull = (int*)(ship + 0x188);
	if (!hull)
		return 0;
	fuel = (int*)(ship + 0x610);
	if (!fuel)
		return 0;
	scrap = (int*)(ship + 0x670);
	if (!scrap)
		return 0;

	weapons = *(void**)(ship + 0x88);
	if (!weapons)
		return 0;
	weapons_level = (int*)(weapons + 0x54);
	if (!weapons_level)
		return 0;
	weapons_power_level = (int*)(weapons + 0x50);
	if (!weapons_power_level)
		return 0;
	weapons_health = (int*)(weapons + 0x178);
	if (!weapons_health)
		return 0;
	missiles = (int*)(weapons + 0x288);
	if (!missiles)
		return 0;

	power = *(void**)0xa47c70;
	if (!power)
		return 0;
	power_used = (int*)power;
	if (!power_used)
		return 0;
	power_max = (int*)(power + 0x4);
	if (!power_max)
		return 0;

	shields = *(void**)(ship + 0x80);
	if (!shields)
		return 0;
	shields_level = (int*)(shields + 0x54);
	if (!shields_level)
		return 0;
	shields_power_level = (int*)(shields + 0x50);
	if (!shields_power_level)
		return 0;
	shields_health = (int*)(shields + 0x178);
	if (!shields_health)
		return 0;
	shields_online = (int*)(shields + 0x270);
	if (!shields_online)
		return 0;
	shields_max = (int*)(shields + 0x274);
	if (!shields_max)
		return 0;

	drones = *(void**)(ship + 0x90); // TODO: this isn't working
	if (!drones)
		return 0;
	drones_level = (int*)(drones + 0x54);
	if (!drones_level)
		return 0;
	drones_power_level = (int*)(drones + 0x50);
	if (!drones_power_level)
		return 0;
	drones_health = (int*)(drones + 0x178);
	if (!drones_health)
		return 0;
	drone_parts = (int*)(drones + 0x260);
	if (!drone_parts)
		return 0;

	engines = *(void**)(ship + 0x98);
	if (!engines)
		return 0;
	engines_level = (int*)(engines + 0x54);
	if (!engines_level)
		return 0;
	engines_power_level = (int*)(engines + 0x50);
	if (!engines_power_level)
		return 0;
	engines_health = (int*)(engines + 0x178);
	if (!engines_health)
		return 0;

	return 1;
}

pid_t findpid(const char* target) {
    DIR* dir;
    dir = opendir("/proc");
    if (dir == 0) {
        fprintf(stderr, "Failed to open /proc: %s\n", strerror(errno));
        return 0;
    }    

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char filename[512] = "/proc/";
        if (!isdigit(entry->d_name[0]))
            continue;

        if (entry->d_name[0] == 46) // skip . and ..
            continue;

        if (entry->d_type == 4) {
            strcat(filename, entry->d_name);
            strcat(filename, "/status");

            FILE* file;
            if ((file = fopen(filename, "r")) == NULL) {
                fprintf(stderr, "Failed to open %s: %s\n", filename, strerror(errno));
                return 0;
            }

            char name[512];
            fscanf(file, "Name:\t%[^\n]", name);
            fclose(file);

            if (!strcmp(name, target)) {
                int pid;
                sscanf(entry->d_name, "%d", &pid);

                return pid;
            }
        }
    }

    return 0;
}

void hack_ftl() {
	int pid = findpid("FTL.amd64");
	if (!pid) {
		fprintf(stderr, "\n\n=== Failed to find FTL ===\n\n");
		return;
	}

	while (!update_pointers())
		sleep(1);

	fprintf(stderr, "\n\n=== Hacked FTL ===\n\n");

	*weapons_level = 8;
	*shields_level = 8;
	*engines_level = 8;
	*drones_level = 8;

	int weapons_hp = *weapons_health;
	int shields_hp = *shields_health;
	int engines_hp = *engines_health;
	int drones_hp = *drones_health;

	while (1) {
		// close when FTL closes
		kill(pid, 0);
		if (errno == ESRCH)
			break;

		// make sure all pointers are valid and up-to-date
		while (!update_pointers())
			sleep(1);

		// fprintf(stderr, "\n\n=== hull: %d ===\n", *hull);
		// fprintf(stderr, "=== fuel: %d ===\n", *fuel);
		// fprintf(stderr, "=== scrap: %d ===\n", *scrap);
		// fprintf(stderr, "=== drones: %d ===\n", *drone_parts);
		// fprintf(stderr, "=== missiles: %d ===\n", *missiles);
		// fprintf(stderr, "=== power_used: %d ===\n", *power_used);
		// fprintf(stderr, "=== power_max: %d ===\n", *power_max);
		// fprintf(stderr, "=== shields_online: %d ===\n", *shields_online);
		// fprintf(stderr, "=== shields_max: %d ===\n\n", *shields_max);
		
		*hull = 30;
		*fuel = 999;
		*scrap = 9999;
		*drone_parts = 999; // TODO: this isn't working
		*missiles = 999;
		*power_used = 0;
		*power_max = 25;
		*shields_online = *shields_max;

		*weapons_power_level = 1;
		*shields_power_level = 8;
		*engines_power_level = 8;

		*weapons_health = weapons_hp;
		*shields_health = shields_hp;
		*engines_health = engines_hp;
		*drones_health = drones_hp;

		// *weapons_health = weapons_hp;
		// *shields_health = shields_hp;

		sleep(5);
	}

	fprintf(stderr, "\n\n=== Exiting hack function ===\n\n");
}

FILE *fopen(const char *pathname, const char *mode) {
	if (!real_fopen) {
		real_fopen = dlsym(RTLD_NEXT, "fopen");

		pthread_create(&hack_thread, NULL, (void*)&hack_ftl, NULL);
	}

	return real_fopen(pathname, mode);
}

__attribute__((constructor)) static void setup(void) {
	fprintf(stderr, "called setup()\n");
}
