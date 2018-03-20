#include "simpletime.h"
#include "chrono"
#include "thread"

SimpleTime::SimpleTime(QObject *parent) : QObject(parent), is_terminated(false)
{

}

void SimpleTime::run(){
    int j = 0;
    while (!is_terminated) {
        if(j%9==0)
            emit cicle(j/9);
        j = (j+1)%27;
        std::this_thread::sleep_for((std::chrono::milliseconds)100);
    }
}
