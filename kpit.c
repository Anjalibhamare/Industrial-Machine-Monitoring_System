#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    printf("Factory Controller Started\n");
    printf("PID is %d PPID is %d\n", getpid(), getppid());

    pid_t pid1, pid2, pid3, pid4;

    pid1 = fork();
    if (pid1 == 0)
    {
        printf("PID of Machine 1 is %d\n", getpid());
        return 0;
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        printf("PID of Machine 2 is %d\n", getpid());
        return 0;
    }

    pid3 = fork();
    if (pid3 == 0)
    {
        printf("PID of Monitor is %d\n", getpid());
        return 0;
    }

    pid4 = fork();
    if (pid4 == 0)
    {
        printf("PID of Logger is %d\n", getpid());
        return 0;
    }

    waitpid(pid1, NULL, 0);
    printf("Child process terminated\n");

    waitpid(pid2, NULL, 0);
    printf("Child process terminated\n");

    waitpid(pid3, NULL, 0);
    printf("Child process terminated\n");

    waitpid(pid4, NULL, 0);
    printf("Child process terminated\n");

    return 0;
}
