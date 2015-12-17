/*
 * startup.hpp
 *
 *  Created on: Mar 4, 2013
 *      Author: penguin
 */

void SystemInit();
int main();

/* start address for the initialization values of the .data section.
 * defined in linker script */
extern uint32_t _sidata;
/* start address for the .data section. defined in linker script */
extern uint32_t _sdata;
/* end address for the .data section. defined in linker script */
extern uint32_t _edata;
/* start address for the .bss section. defined in linker script */
extern uint32_t _sbss;
/* end address for the .bss section. defined in linker script */
extern uint32_t _ebss;


namespace Startup
{

inline void CopyDataInit()
{
    uint32_t *source, *destination;
    source = &_sidata;
    destination = &_sdata;
    while(destination <= &_edata)
    {
        *destination++ = *source++;
    }
}

inline void FillZeroBss()
{
    uint32_t *p;
    for(p = &_sbss;p <=&_ebss; *p++=0);
}


} // namespace Startup

extern "C" void Reset_Handler(void)
{
    Startup::CopyDataInit();
    Startup::FillZeroBss();
    SystemInit();
    main();
}

