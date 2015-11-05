#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENUMBER 0xFF
#define BUFSIZE 10000

typedef int DFILE; // file ID

float _buffer[FILENUMBER][BUFSIZE][3];
int _index[FILENUMBER];
FILE *_file[FILENUMBER];
DFILE _findex = 1;


// void test(struct point p)
// {
// 	printf("%f,%f,%f\n", p.x, p.y, p.z);
// }

DFILE OpenFile(const char* filename)
{
	if(_findex == FILENUMBER) return -1;

	_file[_findex] = fopen(filename, "wb");
	if(_file[_findex] == NULL) return 0;

	_index[_findex] = 0;
	_findex++;
	return _findex - 1;
}

void WriteImmediate(float t, DFILE f)
{
	fwrite(&t, sizeof(float), 1, _file[f]);
	fflush(_file[f]);
}

void BWrite(float x, float y, float z, DFILE f)
{
	_buffer[f][_index[f]][0] = x;
	_buffer[f][_index[f]][1] = y;
	_buffer[f][_index[f]][2] = z;
	_index[f]++;

	if(_index[f] == BUFSIZE)
	{
		fwrite(_buffer[f], sizeof(float)*3, BUFSIZE, _file[f]);
		fflush(_file[f]);
		_index[f] = 0;
	}
}

void CloseFile(DFILE f)
{
	fwrite(_buffer[f], sizeof(float)*3, _index[f], _file[f]);
	fclose(_file[f]);
}

int main(int argc, char *argv[])
{
	// struct point p = {1, 2, 3};
	// test((struct point){1, 2, 3});
	// return 0;

	int count;
	for(count = 1; count < argc; count++)
	{
		// open input file
		char filename[0xFF];
		strcpy(filename, argv[count]);
		FILE *file = fopen(filename, "r");

		// open output file
		int slen = strlen(filename);
		filename[slen - 3] = 'b';
		filename[slen - 2] = 'p';
		filename[slen - 1] = 'a';
		DFILE bfile = OpenFile(filename);

		printf("start output : %s\n", filename);

		// read, write file
		char line[0xFF];
		double t, x, y, z;

		fgets(line, 0xFF, file); // skip label

		fgets(line, 0xFF, file);
		sscanf(line, "%lf,%lf,%lf,%lf", &t, &x, &y, &z);
		BWrite(x, y, z, bfile);

		fgets(line, 0xFF, file);
		sscanf(line, "%lf,%lf,%lf,%lf", &t, &x, &y, &z);
		BWrite(x, y, z, bfile);
		WriteImmediate((float)t, bfile);

		while(fgets(line, 0xFF, file) != NULL)
		{
			if(sscanf(line, "%lf,%lf,%lf,%lf", &t, &x, &y, &z) == EOF) break;
			BWrite(x, y, z, bfile);
		}

		fclose(file);
		CloseFile(bfile);
	}

	return 0;
}