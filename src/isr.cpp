
#include "isr.hpp"
using namespace Interrupts;

void Default_Handler(void)
{
    while(1);
}
inline void Def_Handl(int) { Default_Handler(); }

ISR_t Manager::ISRs[NUM_INTERRUPTS] = {
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl,
    Def_Handl
};