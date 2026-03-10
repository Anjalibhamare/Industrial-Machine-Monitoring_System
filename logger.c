#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

struct shmdata
{
    int machine_id;
    int production;
    int temperature;
    int vibration;
    int alert_flag;
};

int main()
{
    printf("Logger started\n");

    key_t key = ftok("shmfile", 75);
    int shmid = shmget(key, sizeof(struct shmdata), 0666);

    struct shmdata *data = (struct shmdata *)shmat(shmid, NULL, 0);

    int fd = open("logs/machine.log", O_WRONLY | O_CREAT | O_APPEND, 0644);

    while (1)
    {
        printf("Logger received data\n");
        printf("Machine%d | Production %d | Temp %d\n",
               data->machine_id,
               data->production,
               data->temperature);

        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        char timestamp[20];
        strftime(timestamp, 20, "%H:%M:%S", tm);

        char status[10];

        if (data->alert_flag == 1)
            strcpy(status, "ALERT");
        else
            strcpy(status, "OK");

        char log[200];

        sprintf(log, "%s | Machine%d | %d | %d | %d | %s\n",
                timestamp,
                data->machine_id,
                data->production,
                data->temperature,
                data->vibration,
                status);

        lseek(fd, 0, SEEK_END);
        write(fd, log, strlen(log));

        sleep(1);
    }

    close(fd);
}