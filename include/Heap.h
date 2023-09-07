#ifndef HEAP_H
#define HEAP_H
#include <memory.h>
#include <sstream>
#include "IHeap.h"
#include "DLinkedList.h"

template<class T>
class Heap: public IHeap<T>{
public:
    class Iterator; //forward declaration
    
protected:
    T *elements;    //a dynamic array to contain user's data
    int capacity;   //size of the dynamic array
    int count;      //current count of elements stored in this heap
    int (*comparator)(T& lhs, T& rhs);      //see above
    void (*deleteUserData)(Heap<T>* pHeap); //see above 
    
public:
    Heap(   int (*comparator)(T& , T&)=0, 
            void (*deleteUserData)(Heap<T>*)=0 );
    
    Heap(const Heap<T>& heap); //copy constructor 
    Heap<T>& operator=(const Heap<T>& heap); //assignment operator
    
    ~Heap();
    
    //Inherit from IHeap: BEGIN
    void push(T item);
    T pop();
    const T peek();
    void remove(T item, void (*removeItemData)(T)=0);
    bool contains(T item);
    int size();
    void heapify(T array[], int size);
    void clear();
    bool empty();
    string toString(string (*item2str)(T&)=0 );
    //Inherit from IHeap: END

    void heapifySelf();
    DLinkedList<T> nlr();
    
    void println(string (*item2str)(T&)=0 ){
        cout << toString(item2str) << endl;
    }
    
    Iterator begin(){
        return Iterator(this, true);
    }
    Iterator end(){
        return Iterator(this, false);
    }
    
public:
    /* if T is pointer type:
     *     pass the address of method "free" to Heap<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  Heap<Point*> heap(&Heap<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(Heap<T> *pHeap){
        for(int idx=0; idx < pHeap->count; idx++) delete pHeap->elements[idx];
    }
    
    
private:
    bool aLTb(T& a, T& b){
        return compare(a, b) < 0;
    }
    bool aLEb(T& a, T& b) {
        return compare(a, b) <= 0; // ???? <=
    }
    int compare(T& a, T& b){
        if(comparator != 0) return comparator(a, b);
        else{
            if (a < b) return -1;
            else if(a > b) return 1;
            else return 0;
        }
    }
    
    void ensureCapacity(int minCapacity); 
    void swap(int a, int b);
    void reheapUp(int position);
    void reheapDown(int position);
    int getItem(T item);
    void nlr(int position, DLinkedList<T>& list);
    
    void removeInternalData();
    void copyFrom(const Heap<T>& heap);
    
    
//////////////////////////////////////////////////////////////////////
////////////////////////  INNER CLASSES DEFNITION ////////////////////
//////////////////////////////////////////////////////////////////////
    
public:
    
    //Iterator: BEGIN
    class Iterator{
    private:
        Heap<T>* heap;
        int cursor;
    public:
        Iterator(Heap<T>* heap=0, bool begin=0){
            this->heap = heap;
            if(begin && (heap !=0)) cursor = 0;
            if(!begin && (heap !=0)) cursor = heap->size();
        }
        Iterator& operator=(const Iterator& iterator){
            this->heap = iterator.heap;
            this->cursor = iterator.cursor;
            return *this;
        }
        
        T& operator*(){
            return this->heap->elements[cursor];
        }
        bool operator!=(const Iterator& iterator){
            return this->cursor != iterator.cursor;
        }
        // Prefix ++ overload 
        Iterator& operator++(){
            cursor++;
            return *this; 
        }
        // Postfix ++ overload 
        Iterator operator++(int){
            Iterator iterator = *this; 
            ++*this; 
            return iterator; 
        }
        void remove(void (*removeItemData)(T)=0){
            this->heap->remove(this->heap->elements[cursor], removeItemData);
        }
    };
    //Iterator: END
};


//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
Heap<T>::Heap(
        int (*comparator)(T&, T&), 
        void (*deleteUserData)(Heap<T>* ) ){
    capacity = 10;
    count = 0;
    elements = new T[capacity];
    this->comparator = comparator;
    this->deleteUserData = deleteUserData;
}
template<class T>
Heap<T>::Heap(const Heap<T>& heap){
    copyFrom(heap);
}

template<class T>
Heap<T>& Heap<T>::operator=(const Heap<T>& heap){
    removeInternalData();
    copyFrom(heap);
    return *this;
}


template<class T>
Heap<T>::~Heap(){
    removeInternalData();
}

template<class T>
void Heap<T>::push(T item){ //item  = 25
    ensureCapacity(count + 1); //[18, 15, 13, 25 , , ]
    elements[count] = item;
    count += 1; //count = 
    reheapUp(count - 1); // [18, 15, 13, 25 , , ]
}

template<class T>
T Heap<T>::pop(){
    if(count == 0) 
        throw std::underflow_error("Calling to peek with the empty heap.");
    
    T item = elements[0]; //item =25
    elements[0] = elements[count - 1]; //[15, 18, 13, , , ]
    count -= 1;
    reheapDown(0);
    return item;
}

template<class T>
const T Heap<T>::peek(){
    if(count == 0) 
        throw std::underflow_error("Calling to peek with the empty heap.");
    return elements[0];
}

template<class T>
void Heap<T>::remove(T item, void (*removeItemData)(T)){
    int foundIdx = this->getItem(item);
    
    //CASE 1: not found
    if(foundIdx == -1) return;

    //CASE 2: found at 0 => use pop
    if(foundIdx == 0){
        T removed = this->pop();
        if(removeItemData != 0) removeItemData(removed);
        return;
    }
    
    //CASE 3: found at anywhere else
    //Remove item's data
    if(removeItemData != 0) removeItemData(elements[foundIdx]);
    
    //Remove item: shift left
    int copyCount = (count - 1) - (foundIdx + 1) + 1;
    memcpy(&elements[foundIdx], &elements[foundIdx + 1], copyCount*sizeof(T));
    count -= 1;

    //Determine valid heap [0-> (new size -1)]
    int startOldData = foundIdx;
    int lastOldData = count - 1;
    count = foundIdx;

    //reheapify from startOldData
    for(int idx=startOldData; idx <= lastOldData; idx++)
        push(elements[idx]);
}

template<class T>
bool Heap<T>::contains(T item){
    bool found = false;
    for(int idx=0; idx < count; idx++){
        if(compare(elements[idx], item) == 0){
            found = true;
            break;
        }
    }
    return found;
}

template<class T>
int Heap<T>::size(){
    return count;
}

template<class T>
void Heap<T>::heapify(T array[], int size){
    for(int idx=0; idx < size; idx++) push(array[idx]);
}

template<class T>
void Heap<T>::heapifySelf() {
    Heap<T> newHeap(*this);
    this->clear();

    Iterator it = newHeap.begin();
    for (it; it != newHeap.end(); it++)
        this->push(*it);
    
}

template<class T>
DLinkedList<T> Heap<T>::nlr() {
    DLinkedList<T> list;
    nlr(0, list);
    return list;
}

template<class T>
void Heap<T>::nlr(int position, DLinkedList<T>& list) {
    if (position >= count) return;
    list.add(elements[position]);
    nlr(position*2 + 1, list);
    nlr(position*2 + 2, list);
}

template<class T>
void Heap<T>::clear(){
    removeInternalData();
    
    capacity = 10;
    count = 0;
    elements = new T[capacity];
}

template<class T>
bool Heap<T>::empty(){
    return count == 0;
}

template<class T>
string Heap<T>::toString(string (*item2str)(T&)){
    stringstream os;
    if(item2str != 0){
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << item2str(elements[idx]) << ",";
        if(count > 0) os << item2str(elements[count - 1]);
        os << "]";
    }
    else{
        os << "[";
        for(int idx=0; idx < count -1; idx++)
            os << elements[idx] << ",";
        if(count > 0) os << elements[count - 1];
        os << "]";
    }
    return os.str();
}


//////////////////////////////////////////////////////////////////////
//////////////////////// (private) METHOD DEFNITION //////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
void Heap<T>::ensureCapacity(int minCapacity){
    if(minCapacity >= capacity){
        //re-allocate 
        int old_capacity = capacity;
        capacity = old_capacity + (old_capacity >> 2);
        try{
            T* new_data = new T[capacity];
            //OLD: memcpy(new_data, elements, capacity*sizeof(T));
            memcpy(new_data, elements, old_capacity*sizeof(T));
            delete []elements;
            elements = new_data;
        }
        catch(std::bad_alloc e){
            e.what();
        }
    }
}

template<class T>
void Heap<T>::swap(int a, int b){
    T temp = this->elements[a];
    this->elements[a] = this->elements[b];
    this->elements[b] = temp;
}

template<class T>
void Heap<T>::reheapUp(int position){
    if(position <= 0) return;
    int parent = (position-1)/2;
    if(aLTb(this->elements[position], this->elements[parent])){
        this->swap(position, parent);
        reheapUp(parent);
    }
}

template<class T>
void Heap<T>::reheapDown(int position){
    int leftChild = position*2 + 1;
    int rightChild = position*2 + 2;
    int lastPosition = this->count - 1;

    if(leftChild <= lastPosition){
        int smallChild = leftChild; //by default => leftChild valid but rightChild invalid
        if(rightChild <= lastPosition){
            if(aLEb(this->elements[leftChild], this->elements[rightChild])) 
                smallChild = leftChild;
            else smallChild = rightChild;
        }

        if(aLTb(this->elements[smallChild], this->elements[position])){
            this->swap(smallChild, position);
            reheapDown(smallChild);
        }
    }
}

template<class T>
int Heap<T>::getItem(T item){
    int foundIdx = -1;
    for(int idx=0; idx < this->count; idx++){
        if(compare(elements[idx], item) == 0){
            foundIdx = idx;
            break;
        }
    }
    return foundIdx;
}

template<class T>
void Heap<T>::removeInternalData(){
    if(this->deleteUserData != 0) deleteUserData(this); //clear users's data if they want
    delete []elements;
}

template<class T>
void Heap<T>::copyFrom(const Heap<T>& heap){
    capacity = heap.capacity;
    count = heap.count;
    elements = new T[capacity];
    this->comparator = heap.comparator;
    this->deleteUserData = heap.deleteUserData;
    
    //Copy items from heap:
    for(int idx=0; idx < heap.count; idx++){
        this->elements[idx] = heap.elements[idx];
    }
}

#endif