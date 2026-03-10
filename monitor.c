#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

struct message
{
    long msg_type;
    int machine_id;
    int production;
    int temperature;
    int vibration;
};

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
    printf("Monitoring system started\n");

    key_t msgkey = ftok("msgqueue", 65);
    int msgid = msgget(msgkey, 0666 | IPC_CREAT);

    key_t shmkey = ftok("shmfile", 75);
    int shmid = shmget(shmkey, sizeof(struct shmdata), 0666 | IPC_CREAT);

    struct shmdata *data = (struct shmdata *)shmat(shmid, NULL, 0);

    struct message msg;

    while (1)
    {
        msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0);

        printf("\nMonitor received data\n");
        printf("Machine %d\n", msg.machine_id);
        printf("Production: %d\n", msg.production);
        printf("Temperature: %d\n", msg.temperature);
        printf("Vibration: %d\n", msg.vibration);

        data->machine_id = msg.machine_id;
        data->production = msg.production;
        data->temperature = msg.temperature;
        data->vibration = msg.vibration;

        data->alert_flag = 0;

        if (msg.temperature > 80)
        {
            printf("ALERT: Machine %d overheating\n", msg.machine_id);
            data->alert_flag = 1;
        }

        if (msg.vibration > 5)
        {
            printf("ALERT: Machine %d vibration fault\n", msg.machine_id);
            data->alert_flag = 1;
        }
    }
}