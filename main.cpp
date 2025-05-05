#include"CelluarAutomata.h"

int main()
{  
    CelluarAutomata a(200, 200);
    a.setup();
   a.fill(5000, 1,49,1,199);
   a.set_tau(1.0);
    a.display();

    return 0;
}