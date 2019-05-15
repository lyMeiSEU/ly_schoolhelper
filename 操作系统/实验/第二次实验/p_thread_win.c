#include <Windows.h>
#include <stdio.h>
#define N 100
#define TRUE 1
typedef int Semaphore;
Semaphore full = 0, Empty = N;            //共享资源区满槽数目和空槽数目
int in = 0, out = 0;                      //缓冲区生产，消费数据指针
HANDLE mutex;
int ProducerThread[5];
int ConsumerThread[5];
int Buffer[N+4];                          //缓冲区

int produce_item() {                      //生产(随机数)
    return (rand()%N + N)%N;
}

int insert_item(int item) {               //插入资源
    in %= N;
    printf("生产到缓冲区槽： %d\n",in);
    Buffer[in] = item;
    return Buffer[in++];
}

int remove_item() {                        //移出资源
    out %= N;
    printf("                       取走缓冲区槽 %d 的数\n",out);
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

    mutex = CreateSemaphore(             //创建互斥信号量mutex
            NULL,
            1,
            1,
            NULL
        );

    for(int i=0;i<4;i++) {
        ProducerThread[i] = i+1;
        CreateThread(                    //创建生产者线程
            NULL,                        //不能被子线程继承
            0,                           //默认堆栈大小
            producer,                    //生产者函数
            &ProducerThread[i],          //传参
            0,                           //创建后立即执行
            &Tid                         //线程ID
        );
        ConsumerThread[i] = i+1;
        CreateThread(NULL,0,consumer,&ConsumerThread[i],0,&Tid);   //创建消费者线程
    }

    Sleep(20000);
    return 0;
}