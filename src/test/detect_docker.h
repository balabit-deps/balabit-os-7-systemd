#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool
is_docker(void)
{
        char *docker, *line, *tokenize;
        bool docker_detected = false;
        size_t len;
        int ret;

        FILE *cgroup = fopen("/proc/self/cgroup", "r");
        if (cgroup == NULL)
                goto out_not_docker;

        len = 0;
        ret = getline(&line, &len, cgroup);
        if (ret <= 0)
                goto out_not_docker;

        tokenize = line;
        /* split out the cgroup */
        strsep(&tokenize, "/");
        /* try to detect the name */
        docker = strsep(&tokenize, "/");
        if (docker != NULL && strcasecmp(docker, "docker") == 0) {
                docker_detected = true;
                goto out;
        }

out_not_docker:
        docker_detected = false;
out:
        fclose(cgroup);
        free(line);

        return docker_detected;
}


#define EXIT_TEST_SKIP_IF_DOCKER()	\
({ \
 if (is_docker()) { \
     puts("testing systemd not supported in docker, skipping"); \
     return EXIT_TEST_SKIP; \
 } \
 })
