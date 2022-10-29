#include "main.h"

int main()
{

    losInit();

    //should not get here bec losInit() starts the scheduler
    while(1){}

    return 0;
}
