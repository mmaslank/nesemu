#ifndef _CPU_H_
#define _CPU_H_
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <map>
//flag defines
#define N  0x80
#define V  0x40
#define S  0x20
#define Bb  0x10
#define D  0x08
#define I  0x04
#define Z  0x02
#define C  0x01
//flag setting and resetting
#define SET_N(x) (x ? (status |= N) : (status &= (~N)) )
#define SET_V(x) (x ? (status |= V) : (status &= (~V)) )
#define SET_S(x) (x ? (status |= S) : (status &= (~S)) )
#define SET_Bb(x) (x ? (status |= Bb) : (status &= (~Bb)) )
#define SET_D(x) (x ? (status |= D) : (status &= (~D)) )
#define SET_I(x) (x ? (status |= I) : (status &= (~I)) )
#define SET_Z(x) (x ? (status |= Z) : (status &= (~Z)) )
#define SET_C(x) (x ? (status |= C) : (status &= (~C)) )
//zero, negative, carry and overflow checks
#define IS_Z(n) {\
    if ((n) & 0x00FF) SET_Z(0);\
        else SET_Z(1);\
}
#define IS_N(n) {\
    if ((n) & 0x0080) SET_N(1);\
        else SET_N(0);\
}
#define IS_C(n) {\
    if ((n) & 0xFF00) SET_C(1);\
        else SET_C(0);\
}
#define IS_V(n, m, o) { /* n = result, m = accumulator, o = memory */ \
    if (((n) ^ (uint16_t)(m)) & ((n) ^ (o)) & 0x0080) SET_V(1);\
        else SET_V(0);\
}
class Nes;
class CPU {
    //registers
    static uint8_t ram[2048];
    static uint16_t pc;
    static uint8_t sp, a, x, y, status;
    //supplementary variables for instruction handling
    static uint32_t instructions; //keep track of total instructions executed
    static uint32_t ticks; //keep track of total ticks
    static uint16_t oldpc, ea, reladdr, value, result;  //old program counter (to check for page boundary crossing), effective address, relative address
    static uint8_t opcode;
    static Nes* nes;
    typedef void (*func_array)();
    static func_array addrtable[256];
    static func_array optable[256];
    static const uint32_t ticktable[256];
    static uint8_t penaltyop, penaltyaddr;
    static uint16_t getvalue();
    static void putvalue(uint16_t saveval);
    static void pushw(uint16_t pushval);
    static void pushb(uint8_t pushval);
    static uint16_t popw();
    static uint8_t popb();
    //addressing functions
    static void imp(); //implied
    static void acc(); //accumulator
    static void imm(); //immediate
    static void zp(); //zero-page
    static void zpx(); //zero-page,X
    static void zpy(); //zero-page,Y
    static void rel(); //relative for branch ops (8-bit immediate value, sign-extended)
    static void abso(); //absolute
    static void absx(); //absolute,X
    static void absy(); //absolute,Y
    static void ind(); //indirect
    static void indx(); // (indirect,X)
    static void indy(); // (indirect),Y
    //instruction handling
    static void adc();
    static void ant();
    static void asl();
    static void bcc();
    static void bcs();
    static void beq();
    static void bit();
    static void bmi();
    static void bne();
    static void bpl();
    static void brk();
    static void bvc();
    static void bvs();
    static void clc();
    static void cld();
    static void cli();
    static void clv();
    static void cmp();
    static void cpx();
    static void cpy();
    static void dec();
    static void dex();
    static void dey();
    static void eor();
    static void inc();
    static void inx();
    static void iny();
    static void jmp();
    static void jsr();
    static void lda();
    static void ldx();
    static void ldy();
    static void lsr();
    static void nop();
    static void ora();
    static void pha();
    static void php();
    static void pla();
    static void plp();
    static void rol();
    static void ror();
    static void rti();
    static void rts();
    static void sbc();
    static void sec();
    static void sed();
    static void sei();
    static void sta();
    static void stx();
    static void sty();
    static void tax();
    static void tay();
    static void tsx();
    static void txa();
    static void txs();
    static void tya();
    static void lax();
    static void sax();
    static void dcp();
    static void isb();
    static void slo();
    static void rla();
    static void sre();
    static void rra();
public:
    CPU();
    ~CPU();
    static void connect_bus(Nes* _nes);
    static uint8_t read(uint16_t address);
    static void write(uint16_t address, uint8_t value);
    static void reset6502();
    static void nmi6502();
    static void irq6502();
    static uint8_t step(int printRegs);
};
#endif // _CPU_H_
