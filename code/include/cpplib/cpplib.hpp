#pragma once

#include "stdint.h"
#include "cppl.hpp"
extern "C"
{
#include "lib.h"
}


#define _OS_API


namespace ker_lib
{

template <class T>
class kNode
{
    public:
        kNode(T ele);
        ~kNode();
    private:
        T ele;
        kNode<T> *next;
        kNode<T> *pred;
};

template <class T>
class kList
{
    public:
        kList();
        ~kList();
        uint32_t getLength();
        _OS_API T getElement(uint32_t n);
        bool push_back(T ele);
        bool push_front(T ele);
        _OS_API T pop_back();
        _OS_API T pop_front();
        uint32_t size();
        const T& operator[] (uint32_t index);
    private:
        kNode<T> *first;
        kNode<T> *end;
        uint32_t length;
};

template <class T>
class qdeque
{
    public:
        qdeque(uint32_t size);
        ~qdeque();
        bool push(T ele);
        _OS_API T pop();
    private:
        T *buff;
        uint32_t size;
        uint32_t front;
        uint32_t end;
};

}



namespace ker_lib
{
    template <class T>
    kNode<T>::kNode(T ele)
    {
        this->next = nullptr;
        this->pred = nullptr;
        this->ele = ele;
    }

    template <class T>
    kList<T>::kList()
    {
        this->length = 0;
        this->first = nullptr;
        this->end = nullptr;
    }

    template <class T>
    kList<T>::~kList()
    {
        kNode<T> *now = this->first;
        kNode<T> *next;
        while(now != nullptr)
        {
            next = now->next;
            delete now;
            now = next;
        }
    }

    template <class T>
    bool kList<T>::push_back(T ele)
    {
        kNode<T> *new_ele = new kNode<T>(ele);
        if(new_ele == nullptr)
        {
            return false;
        }
        if(this->end = nullptr)
        {
            this->first = new_ele;
            this->end = new_ele;
            this->length = 1;
        }
        else
        {
            this->end->next = new_ele;
            new_ele->pred = this->end;
            this->end = new_ele;
            (this->length) += 1;
        }
        return true;
    }

    template <class T>
    bool kList<T>::push_front(T ele)
    {
        kNode<T> *new_ele = new kNode<T>(ele);
        if(new_ele == nullptr)
        {
            return false;
        }
        if(this->first = nullptr)
        {
            this->first = new_ele;
            this->end = new_ele;
            this->length = 1;
        }
        else
        {
            this->first->pred = new_ele;
            new_ele->next = this->first;
            this->first = new_ele;
            (this->length) += 1;
        }
        return true;
    }

    template <class T>
    uint32_t kList<T>::size()
    {
        return this->length;
    }

    template <class T>
    _OS_API T kList<T>::pop_back()
    {
        if (this->end == nullptr)
        {
            T hhh;
            ERR_CODE[get_pid()] = OS_FAILED;
            return hhh;
        }
        else
        {
            ERR_CODE[get_pid()] = OS_SUCCES;
            T now = this->end->ele;
            kNode<T> *now_end = this->end->pred;
            now_end->next = nullptr;
            delete this->end;
            this->end = now_end;
            return now;
        }
    }

    template <class T>
    _OS_API T kList<T>::pop_front()
    {
        if (this->first == nullptr)
        {
            T hhh;
            ERR_CODE[get_pid()] = OS_FAILED;
            return hhh;
        }
        else
        {
            ERR_CODE[get_pid()] = OS_SUCCES;
            T now = this->first->ele;
            kNode<T> *now_first = this->first->next;
            now_first->pred = nullptr;
            delete this->first;
            this->first = now_first;
            return now;
        }
    }

    template <class T>
    const T& kList<T>::operator[] (uint32_t index)
    {
        if(index >= this->length)
        {
            return nullptr;
        }
        kNode<T> *now;
        if((2 * index) > length)
        {
            for(now = this->end; index != (length - 1); index++)
            {
                now = now->pred;
            }
            if(now == nullptr)
            {
                return nullptr;
            }
            return now->ele;
        }
        else
        {
            for(now = this->first; index != 0; index--)
            {
                now = now->next;
            }
            if(now == nullptr)
            {
                return nullptr;
            }
            return now->ele;
        }
    }

    template <class T>
    qdeque<T>::qdeque(uint32_t size)
    {
        this->buff = new T[size];
        if (this->buff == nullptr)
        {
            this->size = 0;
            return;
        }
        this->size = size;
        this->front = 0;
        this->end = 0;
        return;
    }

    template <class T>
    qdeque<T>::~qdeque()
    {
        if(this->buff != nullptr)
        {
            delete this->buff;
        }
    }

    template <class T>
    bool qdeque<T>::push(T ele)
    {
        if(this->end == (this->size - 1))
        {
            if(this->front == 0)
            {
                return false;
            }
            else
            {
                this->buff[this->end] = ele;
                this->end = 0;
                return true;
            }
        }
        else
        {
            if((this->end + 1) == this->front)
            {
                return false;
            }
            else
            {
                this->buff[this->end] = ele;
                this->end += 1;
                return true;
            }
        }
    }

    template <class T>
    _OS_API T qdeque<T>::pop()
    {
        if(this->front == this->end)
        {
            T hhh;
            ERR_CODE[get_pid()] = OS_FAILED;
            return hhh;
        }
        else
        {
            ERR_CODE[get_pid()] = OS_SUCCES;
            if(this->front == (this->size - 1))
            {
                this->front = 0;
                return this->buff[this->size - 1];
            }
            else
            {
                this->front += 1;
                return this->buff[this->front - 1];
            }
        }
    }
}