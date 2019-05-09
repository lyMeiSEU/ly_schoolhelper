#include <Windows.h>
#include <stdio.h>
#define N 100
#define TRUE 1
typedef int Semaphore;
Semaphore full = 0, Empty = N;            //???????????????????????
int in = 0, out = 0;                      //???????????????????????
HANDLE mutex;
int ProducerThread[5];
int ConsumerThread[5];
int Buffer[N+4];                          //??????

int produce_item() {                      //????(?????)
    return (rand()%N + N)%N;
}

int insert_item(int item) {               //???????
    in %= N;
    printf("??????????????? %d\n",in);
    Buffer[in] = item;
    return Buffer[in++];
}

int remove_item() {                        //??????
    out %= N;
    printf("                       ?????????? %d ????\n",out);
    return Buffer[out++];
}

int consume_item(int item) {
    //consume it
}

void down(HANDLE handle) {                  //wait / P
    WaitForSingleObject(handle, INFINITE);
}

void up(HANDLE handle) {                    //signal / V
    ReleaseSemaphore(handle, 1, NULL);
}

DWORD WINAPI producer(LPVOID v) {

    int item;

    while(TRUE) {

        item = produce_item();
        if(Empty > 0) {           //down(empty)
            Empty--;
            down(mutex);          //down(mutex)
            insert_item(item);
            full++;               //up(full)
            up(mutex);            //up(mutex)
        }

        Sleep(2000);
    }
    return 1;
}

DWORD WINAPI consumer(LPVOID v) {

    int item;

    while(TRUE) {

        if(full > 0) {             //down(full)
            full--;
            down(mutex);           //down(mutex)
            item = remove_item();
            consume_item(item);
            Empty++;               //up(empty)
            up(mutex);             //up(mutex)
        }

        Sleep(2000);
    }
    return 1;
}

int main()
{
    DWORD Tid;

    mutex = CreateSemaphore(             //?????????????mutex
            NULL,
            1,
            1,
            NULL
        );

    for(int i=0;i<4;i++) {
        ProducerThread[i] = i+1;
        CreateThread(                    //?????????????
            NULL,                        //????????????
            0,                           //???????§³
            producer,                    //?????????
            &ProducerThread[i],          //????
            0,                           //?????????????
            &Tid                         //???ID
        );
        ConsumerThread[i] = i+1;
        CreateThread(NULL,0,consumer,&ConsumerThread[i],0,&Tid);   //?????????????
    }

    Sleep(20000);
    return 0;
}