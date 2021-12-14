
#ifndef myio_h
#define myio_h

extern int nError;

void openfile(char *s);
void closefile();

int read2bytes();
int readbyte();

void resetwritebuffer();
void writebyte(int x);
void write2bytes(int x);
void writestring(char *s);
void writetofile(char *filename);

void Error(char *s);

//using namespace std;

#endif
