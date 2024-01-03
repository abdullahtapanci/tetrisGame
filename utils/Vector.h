#pragma once

#include <iostream>
#include <initializer_list>


template <typename T>
class Vector{
private:
    T* data;
    size_t size;
    size_t capacity;
    //for nested vectors
    size_t rowSize;
    size_t columnSize;
public: 
    Vector();
    Vector(size_t rowSize, size_t columnSize);
    Vector(std::initializer_list<T> elements);
    Vector(std::initializer_list<std::initializer_list<T>> elements);
    ~Vector();
    void pushBack(const T& value);
    void popBack();
    void insert(size_t index, const T& value);
    void erase(size_t index);
    void clear();
    void setRowAndColumnSize(size_t row,size_t column);
    size_t getRowSize() const;
    size_t getColumnSize() const;
    T* begin() const;
    T* end() const;
    size_t getSize() const;
    bool isEmpty() const;
    T& operator[](size_t index);
    T& operator()(size_t rowIndex, size_t columnIndex);
    void reserve(size_t newCapacity);
};

#include "Vector.cpp"


