#include <stdio.h>
#include <iostream>
#include <sys/stat.h>
#include "main.hpp"
#include "pointreader.hpp"
#include "vector3d.hpp"

PointReader::PointReader(const char* filepath, double *interval, unsigned int *length)
{
    m_hindex = 0;
    m_bufsize = 0;

    m_file = fopen(filepath, "rb");
    if(m_file == NULL)
    {
        std::cerr << "PointReader::PointReader : fileopen failed(" << filepath << ")." << std::endl;
        return;
    }

    // get file-size(number of point)
    struct stat statbuf;
    stat(filepath, &statbuf);
    *length = statbuf.st_size / 4 - 1;
    *length = *length / 3;

    // set interval
    float interval_f;
    fread(&interval_f, sizeof(float), 1, m_file);
    *interval = (double)interval_f;
}
PointReader::~PointReader()
{
    fclose(m_file);
}

Vector3D PointReader::operator[](int index)
{
    if(index < 0) return Vector3D::Zero;

    // if [index] is out of buffer, read the file
    if(index < m_hindex || index >= (m_hindex + m_bufsize))
    {
        m_hindex = index;
        fseek(m_file, (m_hindex*3 + 1)*sizeof(float), SEEK_SET);
        m_bufsize = fread(m_buffer, sizeof(float)*3, MAX_BUFSIZE, m_file);

        if(m_bufsize == 0) Vector3D::Zero;
    }

    float* p = m_buffer[index - m_hindex];

    return Vector3D(p[0], p[1], p[2]);
}