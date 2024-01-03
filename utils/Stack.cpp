#include "Stack.h"

template <typename T>
Stack<T>::Stack():topPtr(nullptr){

}

template <typename T>
Stack<T>::Stack(const Stack<T>& aStack){
    if(aStack.topPtr==nullptr)
        topPtr=nullptr;
    else{
        topPtr=new StackNode;
        topPtr->item=aStack.topPtr->item;

        StackNode *newPtr= topPtr;
        for(StackNode *origPtr=aStack.topPtr->next; origPtr!=nullptr; origPtr=origPtr->next){
            newPtr->next=new StackNode;
            newPtr=newPtr->next;
            newPtr->item=origPtr->item;
        }
        newPtr->next=nullptr;
    }
}

template <typename T>
Stack<T>::~Stack(){
    while(!isEmpty()){
        pop();
    }
}

template <typename T>
bool Stack<T>::isEmpty() const{
    return topPtr==nullptr;
}

template <typename T>
void Stack<T>::push(T newItem){
    StackNode *newPtr= new StackNode;
    newPtr->item=newItem;
    newPtr->next=topPtr;
    topPtr=newPtr;
}

template <typename T>
bool Stack<T>::pop(){
    if(isEmpty()){
        return false;
    }else{
        StackNode *temp=topPtr;
        topPtr=topPtr->next;

        temp->next=nullptr;
        delete temp;
        return true;
    }
}

template <typename T>
bool Stack<T>::pop(T& stackTop){
    if(isEmpty()){
        return false;
    }else{
        stackTop=topPtr->item;
        StackNode *temp=topPtr;
        topPtr=topPtr->next;

        temp->next=nullptr;
        delete temp;
        return true;
    }
}

template <typename T>
bool Stack<T>::getTop(T& stackTop) const{
    if(isEmpty()){
        return false;
    }else{
        stackTop=topPtr->item;
        return true;
    }
}

template <typename T>
T Stack<T>::getTop() const{
    if(isEmpty()){
        return T();
    }else{
        return topPtr->item;
    }
}