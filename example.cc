#include <stdio.h>
#include <unistd.h>
#include "double_buf.h"

class Loader {
    public :
        bool Load(int &i) {
            if (count++ < 2) i=0;
            else i++;
            return true;
        }
    private:
        static int count ;
};
int Loader::count = 0;
/*
void *threadGet(void *t) {
    DoubleBuf<int, Loader> *d = (DoubleBuf<int, Loader> *)t;
    for (int i = 0; i < 500000000; i++) {
        d->GetData();
    }
    printf("%d \n", *d->GetData());
    return NULL;
}
*/

int main(){
    Loader l;
    DoubleBuf<int, Loader> d(1, &l);
    bool r = d.Run();
    printf("%d\n", *d.GetData());
/*
    pthread_t th[10];
    for (int i = 0; i< 10; i ++){
        pthread_create(th+i, NULL, threadGet, (void *)&d);
    }
    sleep(9);
    printf("%d\n", *d.GetData());
    */
}
