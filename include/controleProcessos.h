#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

#define MAX_JOBS 100

typedef struct {
    int id;
    pid_t pid;
    int ativo; 
} job_t;

void init_jobs();
void add_job(pid_t pid);
void check_jobs();
int fg_job(int id);

#endif