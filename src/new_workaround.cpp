#include <cstdlib>
#include <cstddef>

void * operator new(std::size_t n)
{
    void * const p = std::malloc(n);
    // handle p == 0
    return p;
}

void operator delete(void * p, std::size_t) // or delete(void *, std::size_t)
{
    std::free(p);
}
