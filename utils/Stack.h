#pragma once

template <typename T>
class Stack{
private:
    struct StackNode {
        T item;
        StackNode *next;
    };

    StackNode *topPtr;





public:
    Stack();
    Stack(const Stack<T>& aStack);
    ~Stack();

    bool isEmpty() const;
    void push(T newItem);
    bool pop();
    bool pop(T& stackTop);
    bool getTop(T& stackTop) const;
    T getTop() const;



};


#include "Stack.cpp"