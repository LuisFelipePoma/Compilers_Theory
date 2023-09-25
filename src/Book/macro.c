#include <stdio.h>
#define a (x + 1)

int x = 2;
void b()
{
    int x = 1;
    printf("%d\n", a);
}
void c() { printf("%d\n", a); }

int main(){
    b();
    c();
}