#include<math.h>
#include<time.h>

int nrand(int a,int b,unsigned seed)
{
    double temp;
    int len=b-a;
    int ta=a,tb=b;
    time_t t;
    a-=len/3;
    b+=len/3;
    do
    {
        seed++;                     //time unit is second, in order to accelerate.
        t=time(NULL)*seed;
        temp=(sin(128*t)+1)/2;

        temp=(b-a)*temp+a;
        if(temp-(int)temp>=0.5)
            temp++;
        if(temp-(int)temp<=-0.5)
            temp--;
    }while((int)temp<ta||(int)temp>tb);

    return (int)temp;
}
