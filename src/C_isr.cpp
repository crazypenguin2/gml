#include "isr.hpp"

extern "C" void Reset_Handler(void);

using namespace Interrupts;

typedef void (*c_isr_t)();

template <int N> void c_isr()
{
    Manager::isrDispatcher(N);
}

/*template<int N, int... Rest>
struct Array_impl {
    static constexpr auto& value = Array_impl<N - 1, N, Rest...>::value;
};

template<int... Rest>
struct Array_impl<0, Rest...> {
    static constexpr int value[] = { 0, Rest... };
};

template<int... Rest>
constexpr int Array_impl<0, Rest...>::value[] __attribute__ ((section (".cpp_isr")));

template<int N>
struct Array {
    static_assert(N >= 0, "N must be at least 0");

    static constexpr auto& value = Array_impl<N>::value;

    Array() = delete;
    Array(const Array&) = delete;
    Array(Array&&) = delete;
};

template <> struct Array<NUM_INTERRUPTS>;*/

#include <array>

template <int i> constexpr c_isr_t f() { return c_isr<i>; }

template<> constexpr c_isr_t f<1>() { return ::Reset_Handler; }

template<> constexpr c_isr_t f<0>() { return reinterpret_cast<c_isr_t>(0x20002000); }

typedef std::array<c_isr_t, NUM_INTERRUPTS> A;

template<int... i> constexpr A fs() { return A{{ f<i>()... }}; }

template<int...> struct S;

template<int... i> struct S<0,i...>
{ static constexpr A gs() { return fs<0,i...>(); } };

template<int i, int... j> struct S<i,j...>
{ static constexpr A gs() { return S<i-1,i,j...>::gs(); } };

auto isr __attribute__ ((section (".isr_vector"))) = S<NUM_INTERRUPTS-1>::gs();

/*
struct Array {
    //static_assert(N >= 0, "N must be at least 0");

    static constexpr auto& value = Array_impl<NUM_INTERRUPTS>::value;

    Array() = delete;
    Array(const Array&) = delete;
    Array(Array&&) = delete;
};*/

/*c_isr_t x[] __attribute__ ((section (".cpp_isr")))= {
    &c_isr<0>,
    &c_isr<1>,
    &c_isr<2>,
    &c_isr<3>};*/
//int x[] __attribute__ ((section (".cpp_isr")))= {0, 1, 2, 3, 4, 5, 6, 7};
//char stack[10000] __attribute__ ((section ("STACK"))) = { 0 };

// Reset handle directly
/*void isr1(void)
{ Manager::isrDispatcher(1); }*/

/*void isr2(void)
{ Manager::isrDispatcher(2); }

void isr3(void)
{ Manager::isrDispatcher(3); }

void isr4(void)
{ Manager::isrDispatcher(4); }

void isr5(void)
{ Manager::isrDispatcher(5); }

void isr6(void)
{ Manager::isrDispatcher(6); }

void isr7(void)
{ Manager::isrDispatcher(7); }

void isr8(void)
{ Manager::isrDispatcher(8); }

void isr9(void)
{ Manager::isrDispatcher(9); }

void isr10(void)
{ Manager::isrDispatcher(10); }

void isr11(void)
{ Manager::isrDispatcher(11); }

void isr12(void)
{ Manager::isrDispatcher(12); }

void isr13(void)
{ Manager::isrDispatcher(13); }

void isr14(void)
{ Manager::isrDispatcher(14); }

void isr15(void)
{ Manager::isrDispatcher(15); }

void isr16(void)
{ Manager::isrDispatcher(16); }

void isr17(void)
{ Manager::isrDispatcher(17); }

void isr18(void)
{ Manager::isrDispatcher(18); }

void isr19(void)
{ Manager::isrDispatcher(19); }

void isr20(void)
{ Manager::isrDispatcher(20); }

void isr21(void)
{ Manager::isrDispatcher(21); }

void isr22(void)
{ Manager::isrDispatcher(22); }

void isr23(void)
{ Manager::isrDispatcher(23); }

void isr24(void)
{ Manager::isrDispatcher(24); }

void isr25(void)
{ Manager::isrDispatcher(25); }

void isr26(void)
{ Manager::isrDispatcher(26); }

void isr27(void)
{ Manager::isrDispatcher(27); }

void isr28(void)
{ Manager::isrDispatcher(28); }

void isr29(void)
{ Manager::isrDispatcher(29); }

void isr30(void)
{ Manager::isrDispatcher(30); }

void isr31(void)
{ Manager::isrDispatcher(31); }

void isr32(void)
{ Manager::isrDispatcher(32); }

void isr33(void)
{ Manager::isrDispatcher(33); }

void isr34(void)
{ Manager::isrDispatcher(34); }

void isr35(void)
{ Manager::isrDispatcher(35); }

void isr36(void)
{ Manager::isrDispatcher(36); }

void isr37(void)
{ Manager::isrDispatcher(37); }

void isr38(void)
{ Manager::isrDispatcher(38); }

void isr39(void)
{ Manager::isrDispatcher(39); }

void isr40(void)
{ Manager::isrDispatcher(40); }

void isr41(void)
{ Manager::isrDispatcher(41); }

void isr42(void)
{ Manager::isrDispatcher(42); }

void isr43(void)
{ Manager::isrDispatcher(43); }

void isr44(void)
{ Manager::isrDispatcher(44); }

void isr45(void)
{ Manager::isrDispatcher(45); }

void isr46(void)
{ Manager::isrDispatcher(46); }

void isr47(void)
{ Manager::isrDispatcher(47); }

void isr48(void)
{ Manager::isrDispatcher(48); }

void isr49(void)
{ Manager::isrDispatcher(49); }

void isr50(void)
{ Manager::isrDispatcher(50); }

void isr51(void)
{ Manager::isrDispatcher(51); }

void isr52(void)
{ Manager::isrDispatcher(52); }

void isr53(void)
{ Manager::isrDispatcher(53); }

void isr54(void)
{ Manager::isrDispatcher(54); }

void isr55(void)
{ Manager::isrDispatcher(55); }

void isr56(void)
{ Manager::isrDispatcher(56); }

void isr57(void)
{ Manager::isrDispatcher(57); }

void isr58(void)
{ Manager::isrDispatcher(58); }

void isr59(void)
{ Manager::isrDispatcher(59); }

void isr60(void)
{ Manager::isrDispatcher(60); }

void isr61(void)
{ Manager::isrDispatcher(61); }

void isr62(void)
{ Manager::isrDispatcher(62); }

void isr63(void)
{ Manager::isrDispatcher(63); }

void isr64(void)
{ Manager::isrDispatcher(64); }

void isr65(void)
{ Manager::isrDispatcher(65); }

void isr66(void)
{ Manager::isrDispatcher(66); }

void isr67(void)
{ Manager::isrDispatcher(67); }

void isr68(void)
{ Manager::isrDispatcher(68); }

void isr69(void)
{ Manager::isrDispatcher(69); }

void isr70(void)
{ Manager::isrDispatcher(70); }

void isr71(void)
{ Manager::isrDispatcher(71); }

void isr72(void)
{ Manager::isrDispatcher(72); }

void isr73(void)
{ Manager::isrDispatcher(73); }

void isr74(void)
{ Manager::isrDispatcher(74); }

void isr75(void)
{ Manager::isrDispatcher(75); }

void isr76(void)
{ Manager::isrDispatcher(76); }

void isr77(void)
{ Manager::isrDispatcher(77); }

void isr78(void)
{ Manager::isrDispatcher(78); }

void isr79(void)
{ Manager::isrDispatcher(79); }

void isr80(void)
{ Manager::isrDispatcher(80); }
*/
/*
void isr81(void)
{ Manager::isrDispatcher(81); }

void isr82(void)
{ Manager::isrDispatcher(82); }

void isr83(void)
{ Manager::isrDispatcher(83); }

void isr84(void)
{ Manager::isrDispatcher(84); }

void isr85(void)
{ Manager::isrDispatcher(85); }

void isr86(void)
{ Manager::isrDispatcher(86); }

void isr87(void)
{ Manager::isrDispatcher(87); }

void isr88(void)
{ Manager::isrDispatcher(88); }

void isr89(void)
{ Manager::isrDispatcher(89); }

void isr90(void)
{ Manager::isrDispatcher(90); }

void isr91(void)
{ Manager::isrDispatcher(91); }

void isr92(void)
{ Manager::isrDispatcher(92); }

void isr93(void)
{ Manager::isrDispatcher(93); }

void isr94(void)
{ Manager::isrDispatcher(94); }

void isr95(void)
{ Manager::isrDispatcher(95); }

void isr96(void)
{ Manager::isrDispatcher(96); }

void isr97(void)
{ Manager::isrDispatcher(97); }

void isr98(void)
{ Manager::isrDispatcher(98); }

void isr99(void)
{ Manager::isrDispatcher(99); }

void isr100(void)
{ Manager::isrDispatcher(100); }

void isr101(void)
{ Manager::isrDispatcher(101); }

void isr102(void)
{ Manager::isrDispatcher(102); }

void isr103(void)
{ Manager::isrDispatcher(103); }

void isr104(void)
{ Manager::isrDispatcher(104); }

void isr105(void)
{ Manager::isrDispatcher(105); }

void isr106(void)
{ Manager::isrDispatcher(106); }

void isr107(void)
{ Manager::isrDispatcher(107); }

void isr108(void)
{ Manager::isrDispatcher(108); }

void isr109(void)
{ Manager::isrDispatcher(109); }

void isr110(void)
{ Manager::isrDispatcher(110); }

void isr111(void)
{ Manager::isrDispatcher(111); }

void isr112(void)
{ Manager::isrDispatcher(112); }

void isr113(void)
{ Manager::isrDispatcher(113); }
*/
