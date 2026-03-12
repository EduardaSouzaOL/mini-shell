#include "controleProcessos.h"
#include <stdio.h>
#include <sys/wait.h>


job_t jobs[MAX_JOBS];
int next_id = 1;

void init_jobs() {
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs[i].ativo = 0;
    }
}

void add_job(pid_t pid) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs[i].ativo) {
            jobs[i].id = next_id++;
            jobs[i].pid = pid;
            jobs[i].ativo = 1;
            printf("[%d] %d\n", jobs[i].id, pid);
            return;
        }
    }
    printf("mesoShell: limite de jobs atingido.\n");
}

void check_jobs() {
    int status;
    pid_t pid;
    
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < MAX_JOBS; i++) {
            if (jobs[i].ativo && jobs[i].pid == pid) {
                jobs[i].ativo = 0;
                printf("[%d]+ Concluído\t%d\n", jobs[i].id, pid);
                break;
            }
        }
    }
}

int fg_job(int id) {
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].ativo && jobs[i].id == id) {
            printf("fg: [%d] %d\n", id, jobs[i].pid);
            int status;
            waitpid(jobs[i].pid, &status, 0);
            jobs[i].ativo = 0;
            return 1;
        }
    }
    return 0;
}