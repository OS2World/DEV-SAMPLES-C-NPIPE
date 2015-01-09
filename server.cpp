#define INCL_DOSNMPIPES
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "isjpipe.h"

void os2error(APIRET rc, const char *api) {
        fprintf(stderr,"%s failed, rc=%ld\n",api,long(rc));
        exit(-1);
}

int main(void) {
        HPIPE hpipe;
        APIRET rc;

        printf("Creating pipe...\n");

        rc = DosCreateNPipe(PIPENAME,
                            &hpipe,
                            NP_ACCESS_DUPLEX,
                            NP_WAIT|NP_TYPE_MESSAGE|NP_READMODE_MESSAGE|1,
                            4096,
                            4096,
                            0);
        if(rc) os2error(rc,"DosCreateNPipe");

        printf("Pipe created.\n");


        int stop=0;
        while(!stop) {
                printf("Waiting for connect...\n");

                rc = DosConnectNPipe(hpipe);
                if(rc) os2error(rc,"DosConnectNPipe");

                printf("Connection made, reading...\n");

                static char inbuf[4096];  //if not static we need a larger stack
                ULONG bytesRead;
                rc = DosRead(hpipe, inbuf, 4096, &bytesRead);
                if(rc) os2error(rc,"DosRead");

                if(bytesRead==9 && strcmp(inbuf,"shutdown")==0) stop=1;

                printf("%ld bytes read, writing...\n",bytesRead);

                static char outbuf[4096]; //if not static we need a larger stack
                ULONG bytesWritten;
                strcpy(outbuf,"Ok");
                rc = DosWrite(hpipe, outbuf, strlen(outbuf), &bytesWritten);
                if(rc) os2error(rc,"DosWrite");

                printf("%ld bytes written, disconnecting....\n",bytesWritten);

                rc = DosDisConnectNPipe(hpipe);
                if(rc) os2error(rc,"DosDisConnectNPipe");

                printf("Pipe disconnected.\n");
        }

        printf("Closing pipe...\n");

        rc = DosClose(hpipe);
        if(rc) os2error(rc,"DosClose");

        printf("Pipe closed. Have a nice day!\n");

        return 0;
}

