#ifndef SNN_H
#define SNN_H

#include "snn_portme.h"

#if HAS_KLIB
#include <klib.h>
#endif

#if HAS_STDIO
#include <stdio.h>
#endif

#if HAS_STDLIB
#include<stdlib.h>
#endif

typedef	unsigned long long	snn64_t;

/*ANDS*/

inline snn64_t ands(snn64_t rs1, snn64_t rs2){
    register snn64_t src1  asm("a1") = rs1;
    register snn64_t src2  asm("a2") = rs2;
    register snn64_t dest  asm("a0");
    asm volatile (
        ".word 0xC5850B"
        :[dest]"=r"(dest)
        :[src1]"r"(src1), [src2]"r"(src2)
    );
    return dest;
}

/*RPOP*/

inline snn64_t rpop(snn64_t rs){
    register snn64_t src asm("a1") = rs;
    register snn64_t dest asm("a0");
    asm volatile (
        ".word 0x5950B"
        :[dest]"=r"(dest)
        :[src]"r"(src)
    );
    return dest;
}

/*SINIT*/

inline void sinit(snn64_t vinit/*initial voltage*/, snn64_t enable/*online learning enable*/){
    vinit = vinit << 16;
    enable = enable << 15;
    register snn64_t rd asm("a0");
    int a0 = 10 << 7;
    int func = 7 << 12;
    int opcode = 0xb;
    int instr = vinit + enable + func + a0 + opcode;
    asm volatile (
            ".word %[instr]"
            :[rd]"=r"(rd)
            :[instr]"i"(instr)
            );
}

/*SGE*/

inline snn64_t sge(snn64_t vneuron, snn64_t vth){
    register snn64_t src1 asm("a2") = vneuron;
    register snn64_t src2 asm("a1") = vth;
    register snn64_t res asm("a0");
    asm volatile (
            ".word 0xB6450B"
            :[res]"=r"(res)
            :[src1]"r"(src1), [src2]"r"(src2)
            );
    return res;
}

/*NADD*/

inline snn64_t nadd(snn64_t vneuron, snn64_t vin){
    register snn64_t src1 asm("a2") = vneuron;
    register snn64_t src2 asm("a1") = vin;
    register snn64_t res asm ("a0");
    int a1 = 11 << 20;
    int a2 = 12 << 15;
    int func = 2 << 12;
    int a0 = 10 << 7;
    int opcode = 0xb;
    int instr =a1 + a2 + func + a0 + opcode;
    asm volatile (
            ".word %[instr]"
            :[res]"=r"(res)
            :[src1]"r"(src1), [src2]"r"(src2), [instr]"i"(instr)
            );
    return res;
}

/*SLS*/

inline snn64_t sls(snn64_t output){
    register snn64_t src1 asm("a1") = output;
    register snn64_t res asm("a0");
    asm volatile (
        ".word 0x205950B"
        :[res]"=r"(res)
        :[src1]"r"(src1)
    );
    return res;
}

/*SUP*/

inline snn64_t sup(snn64_t syn, snn64_t input){
    register snn64_t res asm("a0");
    register snn64_t src1 asm("a1") = syn;
    register snn64_t src2 asm("a2") = input;
    asm volatile (
        ".word 0x6C5950B"
        :[res]"=r"(res)
        :[src1]"r"(src1), [src2]"r"(src2)
    );
    return res;
}

/*NLD*/

inline snn64_t nld(snn64_t* base, snn64_t offset){
    register snn64_t* src1 asm("a1") = base;
    register snn64_t res asm("a0");
    int a0 = 10 << 7;
    int a1 = 11 << 15;
    offset = offset << 25;
    int func = 5 << 12;
    int opcode = 0xb;
    int instr = offset + a1 + func + a0 + opcode;
    asm volatile (
        ".word %[instr]"
        :[res]"=r"(res)
        :[src1]"r"(src1), [instr]"i"(instr)
    );
    return res;
}

/*NST*/

inline void nst(snn64_t* base, snn64_t rs,  snn64_t offset){
    register snn64_t src1 asm("a1") = rs;
    register snn64_t* src2 asm("s0") = base;
    int offset_11_5 = offset;
    int offset_4_0 = offset & 0x1f;
    offset_11_5 = offset_11_5 >> 5;
    int s0 = 8 << 15;
    int a1 = 11 << 20;
    int func = 3 << 12;
    int opcode = 0xb;
    offset_11_5 = offset_11_5 << 25;
    offset_4_0 = offset_4_0 << 7;
    int instr = offset_11_5 + a1 + s0 + func + offset_4_0 + opcode;
    asm volatile (
        ".word %[instr]"
        :
        :[src1]"r"(src1), [src2]"r"(src2), [instr]"i"(instr)
    );
}

/*INF*/

inline snn64_t inf(snn64_t rs) {
    register snn64_t src   asm("a1") = rs;
    register snn64_t dest  asm("a0");
    asm volatile (
        ".word  0x405950B"
        :[dest]"=r"(dest)
        :[src]"r"(src)
    );
    return dest;
}

/*VLEAK*/

inline snn64_t vleak(snn64_t src1){
    register snn64_t  rs1 asm("a1") = src1;
    register snn64_t  rd  asm("a0");
    asm volatile (
        ".word  0x805950B"
        :[rd]"=r"(rd)
        :[rs1]"r"(rs1)
    );
    return rd;
}

/*LTD*/

inline snn64_t ltd(snn64_t src1, snn64_t src2){
    register snn64_t  rs1 asm("a1") = src1;
    register snn64_t  rs2 asm("a3") = src2;
    register snn64_t  rd  asm("a0");
    asm volatile (
        ".word  0xD5E50B"
        :[rd]"=r"(rd)
        :[rs1]"r"(rs1), [rs2]"r"(rs2)
    );
    return rd;
}

#endif  /*  SNN_H   */