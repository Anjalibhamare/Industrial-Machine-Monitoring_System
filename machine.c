#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message
{
    long msg_type;
    int machine_id;
    int production;
    int temperature;
    int vibration;
};

pthread_mutex_t lock;

int production_value;
int temperature_value;
int vibration_value;

void *production_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        production_value = rand() % 100;
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

void *temperature_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        temperature_value = rand() % 100;
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

void *vibration_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&lock);
        vibration_value = rand() % 10;
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&lock, NULL);

    key_t key = ftok("msgqueue", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    pthread_t production, temperature, vibration;

    srand(time(NULL));

    int machine_id = atoi(argv[1]);

    struct message msg;
    msg.msg_type = 1;
    msg.machine_id = machine_id;

    pthread_create(&production, NULL, production_thread, NULL);
    pthread_create(&temperature, NULL, temperature_thread, NULL);
    pthread_create(&vibration, NULL, vibration_thread, NULL);

    while (1)
    {
        pthread_mutex_lock(&lock);

        msg.production = production_value;
        msg.temperature = temperature_value;
        msg.vibration = vibration_value;

        msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

        printf("Machine %d sent data\n", machine_id);

        pthread_mutex_unlock(&lock);

        sleep(1);
    }
}