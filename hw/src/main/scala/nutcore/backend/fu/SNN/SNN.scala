/**************************************************************************************
* Copyright (c) 2020 Institute of Computing Technology, CAS
* Copyright (c) 2020 University of Chinese Academy of Sciences
* 
* NutShell is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2. 
* You may obtain a copy of Mulan PSL v2 at:
*             http://license.coscl.org.cn/MulanPSL2 
* 
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER 
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR 
* FIT FOR A PARTICULAR PURPOSE.  
*
* See the Mulan PSL v2 for more details.  
***************************************************************************************/

package nutcore

import chisel3._
import chisel3.util._
import chisel3.util.experimental.BoringUtils

import utils._
import top.Settings

object SNNOpType{
    def ands  = "b00000_000".U 
    def sge   = "b100".U
    def rpop  = "b00000_001".U
    def sls   = "b00001_001".U
    def inf   = "b00010_001".U
    def sup   = "b00011_001".U
    def nadd  = "b010".U
    def sinit = "b1110".U  
    def vleak = "b100001".U
    def ltd   = "b11110".U

    //def isDOp(func: UInt): Bool = !func(1) && !func(2)
    def isWen(func: UInt):Bool  = (func === sge || func === sinit || func === sls || func === sup || func === inf || func === vleak || func === ltd)
}

object SNNRF{
    def num = 5
    def vinit = "b001".U
    def output = "b010".U
    def nr  = "b011".U
    def sr  = "b100".U
    def vleak = "b101".U
}

object  SNNCalcType{
    def ssp     = "b00".U
    def neuron  = "b01".U
    def stdp    = "b10".U 
    def ltd    = "b11".U
}

class SNNIO extends FunctionUnitIO{
    val imm = Input(UInt(XLEN.W))
    val srf = Input(Vec(SNNRF.num, UInt(XLEN.W)))
    val srfAddrGen = Output(UInt(3.W))
    val wen = Output(Bool())
}

object SNNDebug {
    def enablePrint = false.B
}

class SNN extends NutCoreModule{
    val io = IO(new SNNIO)

    val imm = io.imm
    val (valid, src1, src2, func) = (io.in.valid, io.in.bits.src1, io.in.bits.src2, io.in.bits.func)    
    def access(valid: Bool, src1: UInt, src2: UInt, func: UInt):UInt = {
        this.valid := valid
        this.src1 := src1
        this.src2 := src2
        this.func := func
        io.out.bits
    }

    val option = func//Mux(SNNOpType.isDOp(func), Cat(imm, func), func)

    val calcUnit = LookupTree(option, List(
        SNNOpType.ands        -> SNNCalcType.ssp,
        SNNOpType.sge         -> SNNCalcType.neuron,
        SNNOpType.rpop        -> SNNCalcType.ssp,
        SNNOpType.sls         -> SNNCalcType.neuron,
        SNNOpType.inf         -> SNNCalcType.stdp,
        SNNOpType.sup         -> SNNCalcType.stdp,
        SNNOpType.nadd        -> SNNCalcType.neuron,
        SNNOpType.sinit       -> SNNCalcType.stdp,
        SNNOpType.vleak       -> SNNCalcType.stdp,
        SNNOpType.ltd         -> SNNCalcType.ltd
    ))

    // spike process module
    val ssp = Module(new SpikeProc(XLEN))
    ssp.io.in.bits.src1 := src1
    ssp.io.in.bits.src2 := src2
    ssp.io.in.bits.op := option
    ssp.io.out.ready := io.out.ready
    ssp.io.in.valid := valid && (calcUnit === SNNCalcType.ssp)

    // neuron module
    val neuron = Module(new NeurModule(XLEN))
    neuron.io.in.bits.src1  := src1
    neuron.io.in.bits.src2  := src2
    neuron.io.in.bits.imm   := imm
    neuron.io.in.bits.vinit := io.srf(SNNRF.vinit)
    neuron.io.in.bits.spike := io.srf(SNNRF.nr)
    neuron.io.in.bits.vleak := io.srf(SNNRF.vleak)
    neuron.io.in.bits.option := option
    neuron.io.out.ready     := io.out.ready
    neuron.io.in.valid      := valid && (calcUnit === SNNCalcType.neuron)


    // STDP module
    val stdp = Module(new STDP(XLEN))
    stdp.io.in.bits.src1 := src1
    stdp.io.in.bits.src2 := src2
    stdp.io.in.bits.imm := imm
    stdp.io.in.bits.op := option
    stdp.io.in.bits.output := io.srf(SNNRF.output)
    stdp.io.in.valid := (valid && (calcUnit === SNNCalcType.stdp))
    stdp.io.out.ready := io.out.ready
    stdp.io.in.bits.vinit := io.srf(SNNRF.vinit)
    //stdp.io.in.bits.spike := Mux(option === SNNOpType.sup, neuron.io.out.bits(0), DontCare)
    
    val ltd = Module(new LTD(XLEN))
    ltd.io.in.bits.prob := src1
    ltd.io.in.valid := ((calcUnit === SNNCalcType.ltd) && valid)
    ltd.io.out.ready := io.out.ready
    val cnt = ltd.io.out.bits.cnt
    ltd.io.in.bits.syn := Mux(cnt =/= 0.U, ltd.io.out.bits.ret, src2)
    // SNN calcutation result
    val res = LookupTree(option, List(
        SNNOpType.ands  ->  ssp.io.out.bits,
        SNNOpType.sge   ->  neuron.io.out.bits,
        SNNOpType.rpop  ->  ssp.io.out.bits,
        SNNOpType.sls   ->  neuron.io.out.bits,
        SNNOpType.sup   ->  stdp.io.out.bits.res,
        SNNOpType.nadd  ->  neuron.io.out.bits,
        SNNOpType.sinit ->  stdp.io.out.bits.res,
        SNNOpType.inf   ->  stdp.io.out.bits.res,
        SNNOpType.vleak ->  stdp.io.out.bits.res,
        SNNOpType.ltd   ->  ltd.io.out.bits.res
    ))

    // generating address of SNN regfile 
    val srfAddrGen = LookupTree(option, List(
        SNNOpType.sinit     -> SNNRF.vinit,
        SNNOpType.sls       -> SNNRF.output,
        SNNOpType.sge       -> SNNRF.nr,
        SNNOpType.sup       -> SNNRF.sr,
        SNNOpType.ltd       -> SNNRF.sr,
        SNNOpType.inf       -> SNNRF.output,
        SNNOpType.vleak     -> SNNRF.vleak
    ))

    io.in.ready := LookupTree(calcUnit, List(
        SNNCalcType.ssp         -> ssp.io.in.ready,
        SNNCalcType.neuron      -> neuron.io.in.ready,
        SNNCalcType.stdp        -> stdp.io.in.ready,
        SNNCalcType.ltd         -> ltd.io.in.ready
    ))

    io.out.bits := res
    io.srfAddrGen := srfAddrGen
    io.wen  := SNNOpType.isWen(option)
    io.out.valid := LookupTree(calcUnit, List(
        SNNCalcType.ssp         -> ssp.io.out.valid,
        SNNCalcType.neuron      -> neuron.io.out.valid,
        SNNCalcType.stdp        -> stdp.io.out.valid,
        SNNCalcType.ltd         -> ltd.io.out.valid
    ))

    when (valid === true.B && SNNDebug.enablePrint){
        printf("[SNN]option = 0x%x\n", option)
        printf("[SNN]src1 = 0x%x\n", src1)
        printf("[SNN]src2 = 0x%x\n", src2)
        printf("[SNN]imm = 0x%x\n", imm)
        printf("[SNN]res = 0x%x\n", res)
        printf("[SNN]ltdret = 0x%x\n", ltd.io.out.bits.ret)
        printf("[SNN]dest = 0x%x\n", io.srfAddrGen)
        printf("[SNN]io.in.ready = %d\n", io.in.ready)
        printf("\n")
    }
}