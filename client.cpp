#include <stdio.h>
#include <io.h>
#include <string.h>

#include "isjpipe.h"

int transactPipe(FILE *fp,
                 const void *outbuf, int outbuflen,
                 void *inbuf, int inbuflen, int *bytesRead)
{
        if(write(fileno(fp),outbuf,outbuflen)!=outbuflen)
                return -1;
        *bytesRead = read(fileno(fp), inbuf, inbuflen);
        if(*bytesRead==-1)
                return -1;
        return 0;
}

int callPipe(const char *pipename, 
             const void *outbuf, int outbuflen,
             void *inbuf, int inbuflen, int *bytesRead)
{
        FILE *fp=fopen(pipename, "r+");
        if(!fp) return -1;
        int r=transactPipe(fp, outbuf, outbuflen, inbuf, inbuflen, bytesRead);
        fclose(fp);
        return r;
}

int main(void) {
        char outbuf[]="shutdown";
        char inbuf[100];
        int inbytes;
        return callPipe(PIPENAME, outbuf,strlen(outbuf)+1, inbuf,100,&inbytes);
}

