#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "Heap.h"

template<class T>
class PriorityQueue: public Heap<T>{
public:
    class Iterator;
    
    PriorityQueue(  int (*comparator)(T& , T&)=0, 
                    void (*removeData)(Heap<T>*)=0):
        Heap<T>(comparator, removeData){
    }
};

#endif