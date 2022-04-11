#pragma once

class Partition{
    int w;
    int h;
    int* s = new int[w*h];
public:
    Partition();
    Partition(int K); //
    ~Partition();
};
