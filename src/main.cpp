// Изменения в программе должны осуществляться только в главном модуле и 
// только путем добавления кода, позволяющего синхронизировать действия:
// -  внутри "производителей" и "потребителей";
// -  в теле главной функции (создание объектов синхронизации и пр.);
// -  в глобальном пространстве (объявление переменных и пр.).

#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include "buffer/buffer.h"

#define C_PRODUCERS 3
#define C_CONSUMERS 5
#define BUFFER_SIZE 5

int g_cOperations = 1000;

void *GetKey(void *ptr) {
    cin.get();
    cout << endl << g_cOperations << endl;
    g_cOperations = 0;
    return NULL;
}

// Исходный вариант потока-производителя
// DWORD __stdcall producer(void * b) {
//     while (g_cOperations-- > 0) {
//         int item = rand();
//         ((Buffer*)b)->PutItem(item);
//         sleep(500 + rand()%100);
//     }
//     return 0;
// }

// Готовый вариант потока-производителя
void *Producer(void *pArg) {
    while (g_cOperations-- > 0) {
        int item = rand();
        ((Buffer*)pArg)->PutItem(item);
        sleep(500 + rand()%100);
    }
    return NULL;
}

// Исходный вариант потока-потребителя
// DWORD __stdcall consumer(void * b) {
//     while (g_cOperations-- > 0)    	{
//         cout << ((Buffer*)b)->GetItem() << endl;
//         sleep(500 + rand()%100);
//     }
//     return 0;
// }

// Готовый вариант потока-потребителя
void *Consumer(void *pArg) {
    while (g_cOperations-- > 0)    	{
        cout << ((Buffer*)pArg)->GetItem() << endl;
        sleep(500 + rand()%100);
    }
    return NULL;
}

int main() {
    Buffer *pBuf = Buffer::CreateBuffer(5); // Создание буфера

    pthread_t keyWaiter;
    pthread_t rgProducers[C_PRODUCERS];
    pthread_t rgConsumers[C_CONSUMERS];
    int hThreads[C_PRODUCERS + C_CONSUMERS];

    cout << "Process started";
    // HANDLE hThreads[C_PRODUCERS + C_CONSUMERS];

    // Вспомогательный поток, ожидающий нажатие клавиши
    // CreateThread(0,0,getkey,0,0,0); 
    pthread_create(&keyWaiter, NULL, GetKey, NULL);
    
    for(int i = 0; i < C_PRODUCERS; i++) // Создание потоков-производителей
        hThreads[i] = pthread_create(&rgProducers[i], NULL, Producer, pBuf);
        // hThreads[i] = CreateThread(0,0,producer,pBuf,0,0);

    for(int i = C_PRODUCERS; i < C_PRODUCERS + C_CONSUMERS; i++) // Создание потоков-потребителей
        hThreads[i] = pthread_create(&rgConsumers[i - C_PRODUCERS], NULL, Consumer, pBuf);
        // hThreads[i] = CreateThread(0,0,consumer,pBuf,0,0);
    
    
    // WaitForMultipleObjects(C_PRODUCERS + C_CONSUMERS, hThreads, true, INFINITE);
    pthread_exit(0);

    // cin.get();
    return 0;
}