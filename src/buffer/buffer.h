#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
// #include <windows.h>

using namespace std;

class Buffer {
protected:
    Buffer(){}
    static Buffer *s_pBuf;
public:
    static Buffer *CreateBuffer(int);
    virtual int GetItem(void) = 0;
    virtual void PutItem(int) = 0;
    virtual ~Buffer(){}
};
#endif