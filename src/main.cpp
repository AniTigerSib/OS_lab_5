// Изменения в программе должны осуществляться только в главном модуле и
// только путем добавления кода, позволяющего синхронизировать действия:
// -  внутри "производителей" и "потребителей";
// -  в теле главной функции (создание объектов синхронизации и пр.);
// -  в глобальном пространстве (объявление переменных и пр.).

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <iostream>

#include "buffer/buffer.h"

#define C_PRODUCERS 3 // 3
#define C_CONSUMERS 5 // 5
#define BUFFER_SIZE 5

int g_cOperations = 1000;

sem_t sem_free_slots; // Семафор для свободных ячеек в буфере
sem_t sem_used_slots; // Семафор для занятых ячеек в буфере
sem_t sem_buffer_lock;

void *GetKey(void*) {
    cin.get();
    cout << endl << g_cOperations << endl;
    g_cOperations = 0;
    return NULL;
}

// Готовый вариант потока-производителя
void *Producer(void *pArg) {
    while (g_cOperations-- > 0) {
        if (sem_wait(&sem_free_slots) != -1 && sem_wait(&sem_buffer_lock) != -1) {
            std::cout << "Producer working\n";
            int item = rand();

            ((Buffer *)pArg)->PutItem(item);

            sem_post(&sem_used_slots);
            sem_post(&sem_buffer_lock);
        }
        // std::cout << "Producer waiting\n";
        // sleep(500 + rand() % 100);
        sleep((500 + rand() % 100) / 1000);
    }
    return NULL;
}

// Готовый вариант потока-потребителя
void *Consumer(void *pArg) {
    while (g_cOperations-- > 0) {
        if (sem_wait(&sem_used_slots) != -1 && sem_wait(&sem_buffer_lock) != -1) {
            std::cout << "Consumer working\n";

            cout << ((Buffer *)pArg)->GetItem() << endl;

            sem_post(&sem_free_slots);
            sem_post(&sem_buffer_lock);
        }
        // std::cout << "Consumer waiting\n";
        // sleep(500 + rand() % 100);
        sleep((500 + rand() % 100) / 1000);
    }
    return NULL;
}

int main() {
    Buffer *pBuf = Buffer::CreateBuffer(BUFFER_SIZE);  // Создание буфера

    sem_init(&sem_free_slots, 0, BUFFER_SIZE);            // BUFFER_SIZE - изначальное количество свободных ячеек
    sem_init(&sem_used_slots, 0, 0);           // Изначально нет занятых ячеек
    sem_init(&sem_buffer_lock, 0, 1);


    pthread_t keyWaiter;
    pthread_t rgProducers[C_PRODUCERS];
    pthread_t rgConsumers[C_CONSUMERS];
    int hThreads[C_PRODUCERS + C_CONSUMERS];

    cout << "Process started";
    
    // Вспомогательный поток, ожидающий нажатие клавиши
    pthread_create(&keyWaiter, NULL, GetKey, NULL);

    for (int i = 0; i < C_PRODUCERS; i++)  // Создание потоков-производителей
        hThreads[i] = pthread_create(&rgProducers[i], NULL, Producer, pBuf);

    for (int i = C_PRODUCERS; i < C_PRODUCERS + C_CONSUMERS; i++)  // Создание потоков-потребителей
        hThreads[i] = pthread_create(&rgConsumers[i - C_PRODUCERS], NULL, Consumer, pBuf);

    pthread_exit(0);

    sem_destroy(&sem_free_slots);
    sem_destroy(&sem_used_slots);

    cin.get();
    return 0;
}