#ifndef _INC_PointReader
#define _INC_PointReader

#include <stdio.h>
#define MAX_BUFSIZE 10000

class Vector3D;

class PointReader
{
private:
    FILE *m_file;
    float m_buffer[MAX_BUFSIZE][3];
    int m_bufsize;
    int m_hindex;  // absolute index of head of buffer

public:
    PointReader(const char* filepath, double *interval, unsigned int *length);
    ~PointReader();
    Vector3D operator[](int index);
};

#endif
