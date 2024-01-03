#include "Vector.h"
#include <iostream>
#include <stdexcept>

template <typename T>
Vector<T>::Vector(): data(nullptr), size(0), capacity(0),rowSize(0), columnSize(0) {}

template <typename T>
Vector<T>::Vector(size_t rowSize, size_t columnSize):data(nullptr),size(0),capacity(rowSize*columnSize*2),rowSize(rowSize),columnSize(columnSize){
    data=new T[capacity];
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> elements):data(nullptr),size(0){
    rowSize=1;
    columnSize=elements.size();
    capacity=size*2;
    
    data=new T[capacity];
    for(auto values:elements){
        pushBack(values);
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<std::initializer_list<T>> elements):data(nullptr),size(0){
    rowSize=elements.size();
    columnSize=elements.begin()->size();
    capacity=rowSize*columnSize*2;
    data=new T[capacity];
    for(auto rows:elements){
        for(auto columns:rows){
            pushBack(columns);
        }
    }
}

template <typename T>
Vector<T>::~Vector(){
    delete[] data;
}

template <typename T>
void Vector<T>::pushBack(const T& value){
    if(size>=capacity){
        size_t newCapacity=(capacity==0)?1:2*capacity;
        reserve(newCapacity);
    }
    data[size++]=value;
}

template <typename T>
void Vector<T>::popBack(){
    if (size > 0) {
        --size;
        data[size].~T();
    } else {
        throw std::out_of_range("popBack(): Vector is empty");
    }
}

template <typename T>
void Vector<T>::insert(size_t index, const T& value){
    if (index > size) {
        throw std::out_of_range("insert(): Index out of range");
    }
    if (size == capacity) {
        reserve(capacity * 2); 
    }
    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

template <typename T>
void Vector<T>::erase(size_t index) {
    if (index >= size) {
        throw std::out_of_range("erase(): Index out of range");
    }

    for (size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
    if (size < capacity / 4) {
        reserve(capacity / 2); 
    }
}

template <typename T>
void Vector<T>::clear() {
    size = 0;
    reserve(0);
}

template <typename T>
void Vector<T>::setRowAndColumnSize(size_t row,size_t column){
    rowSize=size;
    columnSize=column;
}

template <typename T>
size_t Vector<T>::getRowSize() const{
    return rowSize;
}

template <typename T>
size_t Vector<T>::getColumnSize() const{
    return columnSize;
}

template <typename T>
T* Vector<T>::begin() const {
    return data;
}

template <typename T>
T* Vector<T>::end() const {
    return data + size;
}

template <typename T>
size_t Vector<T>::getSize() const{
    return size;
}

template <typename T>
bool Vector<T>::isEmpty() const{
    return size==0;
}

template <typename T>
T& Vector<T>::operator[](size_t index){
    if(index>=size){
        throw std::out_of_range("operator[]: Index out of range");
    }
    return data[index];
}

template <typename T>
T& Vector<T>::operator()(size_t rowIndex, size_t columnIndex){
    if(rowIndex==0){
        size_t index=columnIndex;
        if(index>=size){
            throw std::out_of_range("operator(): Index out of range");
        }
        return data[index];
    }else{
        size_t index=rowIndex*columnSize+columnIndex;
        if(index>=size){
            throw std::out_of_range("operator(): Index out of range");
        }
        return data[index];
    }
}

template <typename T>
void Vector<T>::reserve(size_t newCapacity){
    if(newCapacity>capacity){
        T* newData=new T[newCapacity];
        for(size_t i=0;i<size;++i){
            newData[i]=data[i];
        }
        delete[] data;
        data=newData;
        capacity=newCapacity;
    }
}




