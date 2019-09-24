#include "cpu.h"
#include "Nes.h"
typedef void (*func_array)();
func_array CPU::addrtable[256] = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |     */
/* 0 */     imp, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm, abso, abso, abso, abso, /* 0 */
/* 1 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 1 */
/* 2 */    abso, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm, abso, abso, abso, abso, /* 2 */
/* 3 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 3 */
/* 4 */     imp, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm, abso, abso, abso, abso, /* 4 */
/* 5 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 5 */
/* 6 */     imp, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm,  ind, abso, abso, abso, /* 6 */
/* 7 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 7 */
/* 8 */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* 8 */
/* 9 */     rel, indy,  imp, indy,  zpx,  zpx,  zpy,  zpy,  imp, absy,  imp, absy, absx, absx, absy, absy, /* 9 */
/* A */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* A */
/* B */     rel, indy,  imp, indy,  zpx,  zpx,  zpy,  zpy,  imp, absy,  imp, absy, absx, absx, absy, absy, /* B */
/* C */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* C */
/* D */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* D */
/* E */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* E */
/* F */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx  /* F */
};
func_array CPU::optable[256] = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |      */
/* 0 */      brk,  ora,  nop,  slo,  nop,  ora,  asl,  slo,  php,  ora,  asl,  nop,  nop,  ora,  asl,  slo, /* 0 */
/* 1 */      bpl,  ora,  nop,  slo,  nop,  ora,  asl,  slo,  clc,  ora,  nop,  slo,  nop,  ora,  asl,  slo, /* 1 */
/* 2 */      jsr,  ant,  nop,  rla,  bit,  ant,  rol,  rla,  plp,  ant,  rol,  nop,  bit,  ant,  rol,  rla, /* 2 */
/* 3 */      bmi,  ant,  nop,  rla,  nop,  ant,  rol,  rla,  sec,  ant,  nop,  rla,  nop,  ant,  rol,  rla, /* 3 */
/* 4 */      rti,  eor,  nop,  sre,  nop,  eor,  lsr,  sre,  pha,  eor,  lsr,  nop,  jmp,  eor,  lsr,  sre, /* 4 */
/* 5 */      bvc,  eor,  nop,  sre,  nop,  eor,  lsr,  sre,  cli,  eor,  nop,  sre,  nop,  eor,  lsr,  sre, /* 5 */
/* 6 */      rts,  adc,  nop,  rra,  nop,  adc,  ror,  rra,  pla,  adc,  ror,  nop,  jmp,  adc,  ror,  rra, /* 6 */
/* 7 */      bvs,  adc,  nop,  rra,  nop,  adc,  ror,  rra,  sei,  adc,  nop,  rra,  nop,  adc,  ror,  rra, /* 7 */
/* 8 */      nop,  sta,  nop,  sax,  sty,  sta,  stx,  sax,  dey,  nop,  txa,  nop,  sty,  sta,  stx,  sax, /* 8 */
/* 9 */      bcc,  sta,  nop,  nop,  sty,  sta,  stx,  sax,  tya,  sta,  txs,  nop,  nop,  sta,  nop,  nop, /* 9 */
/* A */      ldy,  lda,  ldx,  lax,  ldy,  lda,  ldx,  lax,  tay,  lda,  tax,  nop,  ldy,  lda,  ldx,  lax, /* A */
/* B */      bcs,  lda,  nop,  lax,  ldy,  lda,  ldx,  lax,  clv,  lda,  tsx,  lax,  ldy,  lda,  ldx,  lax, /* B */
/* C */      cpy,  cmp,  nop,  dcp,  cpy,  cmp,  dec,  dcp,  iny,  cmp,  dex,  nop,  cpy,  cmp,  dec,  dcp, /* C */
/* D */      bne,  cmp,  nop,  dcp,  nop,  cmp,  dec,  dcp,  cld,  cmp,  nop,  dcp,  nop,  cmp,  dec,  dcp, /* D */
/* E */      cpx,  sbc,  nop,  isb,  cpx,  sbc,  inc,  isb,  inx,  sbc,  nop,  sbc,  cpx,  sbc,  inc,  isb, /* E */
/* F */      beq,  sbc,  nop,  isb,  nop,  sbc,  inc,  isb,  sed,  sbc,  nop,  isb,  nop,  sbc,  inc,  isb  /* F */
};
const uint32_t CPU::ticktable[256] = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |     */
/* 0 */      7,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    4,    4,    6,    6,  /* 0 */
/* 1 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 1 */
/* 2 */      6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    4,    4,    6,    6,  /* 2 */
/* 3 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 3 */
/* 4 */      6,    6,    2,    8,    3,    3,    5,    5,    3,    2,    2,    2,    3,    4,    6,    6,  /* 4 */
/* 5 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 5 */
/* 6 */      6,    6,    2,    8,    3,    3,    5,    5,    4,    2,    2,    2,    5,    4,    6,    6,  /* 6 */
/* 7 */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* 7 */
/* 8 */      2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* 8 */
/* 9 */      2,    6,    2,    6,    4,    4,    4,    4,    2,    5,    2,    5,    5,    5,    5,    5,  /* 9 */
/* A */      2,    6,    2,    6,    3,    3,    3,    3,    2,    2,    2,    2,    4,    4,    4,    4,  /* A */
/* B */      2,    5,    2,    5,    4,    4,    4,    4,    2,    4,    2,    4,    4,    4,    4,    4,  /* B */
/* C */      2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* C */
/* D */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7,  /* D */
/* E */      2,    6,    2,    8,    3,    3,    5,    5,    2,    2,    2,    2,    4,    4,    6,    6,  /* E */
/* F */      2,    5,    2,    8,    4,    4,    6,    6,    2,    4,    2,    7,    4,    4,    7,    7   /* F */
};

uint16_t CPU::oldpc, CPU::ea, CPU::reladdr, CPU::value, CPU::result;
uint32_t CPU::instructions, CPU::ticks;
uint8_t CPU::opcode;
uint16_t CPU::pc = 0xC000;
uint8_t CPU::sp = 0xFD, CPU::a, CPU::x, CPU::y, CPU::status = 0x34;
uint8_t CPU::penaltyop, CPU::penaltyaddr;
uint8_t CPU::ram[2048];
Nes* CPU::nes = nullptr;

CPU::CPU(){}
CPU::~CPU(){}
uint8_t CPU::read(uint16_t address)
{
        return ram[address];
}

void CPU::write(uint16_t address, uint8_t value)
{
        ram[address] = value;
}

uint16_t CPU::getvalue()
{
    if (addrtable[opcode] == acc) return((uint16_t)a);
        else return((uint16_t)nes->read(ea));
}

void CPU::putvalue(uint16_t saveval)
{
    if (addrtable[opcode] == acc) a = (uint8_t)(saveval & 0x00FF);
        else nes->write(ea, (saveval & 0x00FF));
}


void CPU::pushw(uint16_t pushval)
{
    nes->write(0x100 + sp, (pushval >> 8) & 0xFF);
    nes->write(0x100 + ((sp - 1) & 0xFF), pushval & 0xFF);
    sp -= 2;
}

void CPU::pushb(uint8_t pushval)
{
    nes->write(0x100 + sp--, pushval);
}

uint16_t CPU::popw()
{
    uint16_t temp16;
    temp16 = nes->read(0x100 + ((sp + 1) & 0xFF)) | ((uint16_t)nes->read(0x100 + ((sp + 2) & 0xFF)) << 8);
    sp += 2;
    return(temp16);
}

uint8_t CPU::popb()
{
    return (nes->read(0x100 + ++sp));
}

uint8_t CPU::step(int printRegs) {
    ticks = 0;
    opcode = nes->read(pc++);
    FILE* out = fopen("out.txt", "a");
    if (printRegs)
    {
        //printf("op %02x pc %04x sp %02x a %02x x %02x y %02x status %02x code %02x\n",
               //opcode, pc-1, sp, a, x, y, status, nes->read(0x0002));
        fprintf(out, "op %02x pc %04x sp %02x a %02x x %02x y %02x status %02x code %02x\n", opcode, pc-1, sp, a, x, y, status, nes->read(0x0002));
    }
    fclose(out);
    //status |= S;
    penaltyop = 0;
    penaltyaddr = 0;
    (*addrtable[opcode])();
    (*optable[opcode])();
    ticks += ticktable[opcode];
    if (penaltyop && penaltyaddr) ticks++;
    instructions++;
    return ticks;
}

void CPU::imp()
{

}

void CPU::acc()
{

}

void CPU::imm()
{
    ea = pc++;
}

void CPU::zp()
{
    ea = (uint16_t)nes->read((uint16_t)pc++);
}

void CPU::zpx()
{
    ea = ((uint16_t)nes->read((uint16_t)pc++) + (uint16_t)x) & 0xFF; //zero-page wraparound
}

void CPU::zpy()
{
    ea = ((uint16_t)nes->read((uint16_t)pc++) + (uint16_t)y) & 0xFF; //zero-page wraparound
}

void CPU::rel()
{
    reladdr = (uint16_t)nes->read(pc++);
    if (reladdr & 0x80) reladdr |= 0xFF00;
}

void CPU::abso()
{
    ea = (uint16_t)nes->read(pc) | ((uint16_t)nes->read(pc+1) << 8);
    pc += 2;
}

void CPU::absx()
{
    uint16_t startpage;
    ea = ((uint16_t)nes->read(pc) | ((uint16_t)nes->read(pc+1) << 8));
    startpage = ea & 0xFF00;
    ea += (uint16_t)x;
    if (startpage != (ea & 0xFF00)) { //one cycle penalty for page-crossing on some opcodes
        penaltyaddr = 1;
    }
    pc += 2;
}

void CPU::absy()
{
    uint16_t startpage;
    ea = ((uint16_t)nes->read(pc) | ((uint16_t)nes->read(pc+1) << 8));
    startpage = ea & 0xFF00;
    ea += (uint16_t)y;
    if (startpage != (ea & 0xFF00)) { //one cycle penalty for page-crossing on some opcodes
        penaltyaddr = 1;
    }
    pc += 2;
}

void CPU::ind()
{
    uint16_t eahelp, eahelp2;
    eahelp = (uint16_t)nes->read(pc) | (uint16_t)((uint16_t)nes->read(pc+1) << 8);
    eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); //replicate 6502 page-boundary wraparound bug
    ea = (uint16_t)nes->read(eahelp) | ((uint16_t)nes->read(eahelp2) << 8);
    pc += 2;
}

void CPU::indx()
{
    uint16_t eahelp;
    eahelp = (uint16_t)(((uint16_t)nes->read(pc++) + (uint16_t)x) & 0xFF); //zero-page wraparound for table pointer
    ea = (uint16_t)nes->read(eahelp & 0x00FF) | ((uint16_t)nes->read((eahelp+1) & 0x00FF) << 8);
}

void CPU::indy()
{
    uint16_t eahelp, eahelp2, startpage;
    eahelp = (uint16_t)nes->read(pc++);
    eahelp2 = (eahelp & 0xFF00) | ((eahelp + 1) & 0x00FF); //zero-page wraparound
    ea = (uint16_t)nes->read(eahelp) | ((uint16_t)nes->read(eahelp2) << 8);
    startpage = ea & 0xFF00;
    ea += (uint16_t)y;
    if (startpage != (ea & 0xFF00)) { //one cycle penalty for page-crossing on some opcodes
        penaltyaddr = 1;
    }
}

void CPU::adc()
{
    penaltyop = 1;
    value = getvalue();
    result = (uint16_t)a + value + (uint16_t)(status & C);
    IS_C(result);
    IS_Z(result);
    IS_V(result, a, value);
    IS_N(result);
    a = (uint8_t)(result & 0xFF);
}

void CPU::ant()
{
    penaltyop = 1;
    value = getvalue();
    result = (uint16_t)a & value;
    IS_Z(result);
    IS_N(result);
    a = (uint8_t)(result & 0xFF);
}

void CPU::asl()
{
    value = getvalue();
    result = value << 1;
    IS_C(result);
    IS_Z(result);
    IS_N(result);
    putvalue(result);
}

void CPU::bcc()
{
    if ((status & C) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2; //check if jump crossed a page boundary
            else ticks++;
    }
}

void CPU::bcs()
{
    if ((status & C) == C) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::beq()
{
    if ((status & Z) == Z) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::bit()
{
    value = getvalue();
    result = (uint16_t)a & value;

    IS_Z(result);
    status = (status & 0x3F) | (uint8_t)(value & 0xC0);
}

void CPU::bmi()
{
    if ((status & N) == N) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::bne()
{
    if ((status & Z) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::bpl()
{
    if ((status & N) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::brk()
{
    pc++;
    pushw(pc);
    pushb(status | Bb);
    SET_I(1);
    pc = (uint16_t)nes->read(0xFFFE) | ((uint16_t)nes->read(0xFFFF) << 8);
}

void CPU::bvc()
{
    if ((status & V) == 0) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::bvs()
{
    if ((status & V) == V) {
        oldpc = pc;
        pc += reladdr;
        if ((oldpc & 0xFF00) != (pc & 0xFF00)) ticks += 2;
            else ticks++;
    }
}

void CPU::clc()
{
    SET_C(0);
}

void CPU::cld()
{
    SET_D(0);
}

void CPU::cli()
{
    SET_I(0);
}

void CPU::clv()
{
    SET_V(0);
}

void CPU::cmp()
{
    penaltyop = 1;
    value = getvalue();
    result = (uint16_t)a - value;
    if (a >= (uint8_t)(value & 0x00FF)) SET_C(1);
        else SET_C(0);
    if (a == (uint8_t)(value & 0x00FF)) SET_Z(1);
        else SET_Z(0);
    IS_N(result);
}

void CPU::cpx()
{
    value = getvalue();
    result = (uint16_t)x - value;
    if (x >= (uint8_t)(value & 0x00FF)) SET_C(1);
        else SET_C(0);
    if (x == (uint8_t)(value & 0x00FF)) SET_Z(1);
        else SET_Z(0);
    IS_N(result);
}

void CPU::cpy()
{
    value = getvalue();
    result = (uint16_t)y - value;
    if (y >= (uint8_t)(value & 0x00FF)) SET_C(1);
        else SET_C(0);
    if (y == (uint8_t)(value & 0x00FF)) SET_Z(1);
        else SET_Z(0);
    IS_N(result);
}

void CPU::dec()
{
    value = getvalue();
    result = value - 1;
    IS_Z(result);
    IS_N(result);
    putvalue(result);
}

void CPU::dex()
{
    x--;
    IS_Z(x);
    IS_N(x);
}

void CPU::dey()
{
    y--;
    IS_Z(y);
    IS_N(y);
}

void CPU::eor()
{
    penaltyop = 1;
    value = getvalue();
    result = (uint16_t)a ^ value;
    IS_Z(result);
    IS_N(result);
    a = (uint8_t)(result & 0xFF);
}

void CPU::inc()
{
    value = getvalue();
    result = value + 1;
    IS_Z(result);
    IS_N(result);
    putvalue(result);
}

void CPU::inx()
{
    x++;
    IS_Z(x);
    IS_N(x);
}

void CPU::iny()
{
    y++;
    IS_Z(y);
    IS_N(y);
}

void CPU::jmp()
{
    pc = ea;
}

void CPU::jsr()
{
    pushw(pc - 1);
    pc = ea;
}

void CPU::lda()
{
    penaltyop = 1;
    value = getvalue();
    a = (uint8_t)(value & 0x00FF);
    IS_Z(a);
    IS_N(a);
}

void CPU::ldx()
{
    penaltyop = 1;
    value = getvalue();
    x = (uint8_t)(value & 0x00FF);
    IS_Z(x);
    IS_N(x);
}

void CPU::ldy()
{
    penaltyop = 1;
    value = getvalue();
    y = (uint8_t)(value & 0x00FF);
    IS_Z(y);
    IS_N(y);
}

void CPU::lsr()
{
    value = getvalue();
    result = value >> 1;
    if (value & 1) SET_C(1);
        else SET_C(0);
    IS_Z(result);
    IS_N(result);
    putvalue(result);
}

void CPU::nop()
{
    switch (opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
            penaltyop = 1;
            break;
    }
}

void CPU::ora()
{
    penaltyop = 1;
    value = getvalue();
    result = (uint16_t)a | value;
    IS_Z(result);
    IS_N(result);
    a = (uint8_t)(result & 0xFF);
}

void CPU::pha()
{
    pushb(a);
}

void CPU::php()
{
    pushb(status | Bb);
}

void CPU::pla()
{
    a = popb();

    IS_Z(a);
    IS_N(a);
}

void CPU::plp()
{
    status = popb() | S;
}

void CPU::rol()
{
    value = getvalue();
    result = (value << 1) | (status & C);
    IS_C(result);
    IS_Z(result);
    IS_N(result);
    putvalue(result);
}

void CPU::ror()
{
    value = getvalue();
    result = (value >> 1) | ((status & C) << 7);
    if (value & 1) SET_C(1);
        else SET_C(0);
    IS_Z(result);
    IS_N(result);
    putvalue(result);
}

void CPU::rti()
{
    status = popb();
    value = popw();
    pc = value;
}

void CPU::rts()
{
    value = popw();
    pc = value + 1;
}

void CPU::sbc()
{
    penaltyop = 1;
    value = getvalue() ^ 0x00FF;
    result = (uint16_t)a + value + (uint16_t)(status & C);
    IS_C(result);
    IS_Z(result);
    IS_V(result, a, value);
    IS_N(result);
    a = (uint8_t)(result & 0xFF);
}

void CPU::sec()
{
    SET_C(1);
}

void CPU::sed()
{
    SET_D(1);
}

void CPU::sei()
{
    SET_I(1);
}

void CPU::sta()
{
    putvalue(a);
}

void CPU::stx()
{
    putvalue(x);
}

void CPU::sty()
{
    putvalue(y);
}

void CPU::tax()
{
    x = a;
    IS_Z(x);
    IS_N(x);
}

void CPU::tay()
{
    y = a;
    IS_Z(y);
    IS_N(y);
}

void CPU::tsx()
{
    x = sp;
    IS_Z(x);
    IS_N(x);
}

void CPU::txa()
{
    a = x;
    IS_Z(a);
    IS_N(a);
}

void CPU::txs()
{
    sp = x;
}

void CPU::tya()
{
    a = y;
    IS_Z(a);
    IS_N(a);
}

void CPU::lax()
{
    lda();
    ldx();
}

void CPU::sax()
{
    sta();
    stx();
    putvalue(a & x);
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::dcp()
{
    dec();
    cmp();
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::isb()
{
    inc();
    sbc();
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::slo()
{
    asl();
    ora();
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::rla()
{
    rol();
    ant();
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::sre()
{
    lsr();
    eor();
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::rra()
{
    ror();
    adc();
    if (penaltyop && penaltyaddr) ticks--;
}

void CPU::nmi6502()
{
    pushw(pc);
    pushb(status);
    status |= I;
    pc = (uint16_t)nes->read(0xFFFA) | ((uint16_t)nes->read(0xFFFB) << 8);
}

void CPU::irq6502()
{
    if(!(status & I)) {
        pushw(pc);
        pushb(status);
        status |= I;
        pc = (uint16_t)nes->read(0xFFFE) | ((uint16_t)nes->read(0xFFFF) << 8);
    }
}


void CPU::reset6502()
{
    pc = (uint16_t)nes->read(0xFFFC) | ((uint16_t)nes->read(0xFFFD) << 8);
    a = 0;
    x = 0;
    y = 0;
    sp = 0xFD;
    status = 0x00 | Bb;
    ticks = 8;
}

void CPU::connect_bus(Nes* _nes) {
    nes = _nes;
}

