// code for random string consist of A,T,G,C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));
    int i;
    char str[120];
    for(i=0;i<120;i++)
    {
        int r = rand()%4;
        if(r==0)
            str[i]='A';
        else if(r==1)
            str[i]='T';
        else if(r==2)
            str[i]='G';
        else
            str[i]='C';
    }
    printf("%s\n",str);
    return 0;
}

