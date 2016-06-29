/*
 *  implement double buffer access.
 *
 */
#ifndef _DOUBLE_BUF_H
#define _DOUBLE_BUF_H

#include <unistd.h>
#include <pthread.h>

/*
 * class P {} //real access data
 * class Loader { bool Load(P &p);}
 */
template<class P, class Loader> class DoubleBuf {
    public :
        DoubleBuf(int i, Loader *l) :
            checkInterval_(i),
            l_(l),
            dataValid_(0)
        {}
        bool Run();
        P* GetData() {
            return &datas_[dataValid_];
        }
    private:
        static void* threadRun( void *arg);
        Loader *l_;
        P datas_[2];
        int dataValid_;
        int checkInterval_;
        pthread_t checkThread_;
};

template<class P, class Loader>
void* DoubleBuf<P, Loader>::threadRun(void *m) {
    DoubleBuf<P, Loader> *p = (DoubleBuf<P, Loader> *)m;
    while (1) {
        sleep(p->checkInterval_);
        int x = (p->dataValid_ +1)%2;
        bool ret = p->l_->Load(p->datas_[x]);
        if (ret) {
            p->dataValid_ = x;
        }
    }
    return NULL;
}

template<class P, class Loader>
bool DoubleBuf<P, Loader>::Run() {
    bool ret = this->l_->Load(datas_[dataValid_]);
    if (ret == false) return false;
    int i = pthread_create(&this->checkThread_, NULL, DoubleBuf<P, Loader>::threadRun, (void *)this);
    if (i == 0) {
        return true; 
    } else return false;
}

#endif
