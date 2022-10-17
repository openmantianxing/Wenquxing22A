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

class LTDIO(val len: Int) extends NutCoreBundle{
    val in =  Flipped(DecoupledIO(new Bundle{
        val prob = Input(UInt(len.W))
        val syn = Input(UInt(len.W))
    }))
    val out = DecoupledIO(new Bundle{
        val res = Output(UInt(len.W))
        val ret = Output(UInt(len.W))
        val cnt = Output(UInt(len.W))
    })
}

class LTD(val len: Int) extends NutCoreModule{
    val io = IO(new LTDIO(XLEN))

    io.out.bits.res := DontCare
    io.out.bits.ret := DontCare
    val prob = io.in.bits.prob
    val syn = RegInit(0.U(64.W))
    syn := io.in.bits.syn
    
    val r = RegInit(0.U(10.W))
    val syn_new = VecInit(syn.asBools)

    val cnt = Counter(len)

    val s_idle :: s_genr :: s_comp :: s_finish :: Nil = Enum(4)
    val state = RegInit(s_idle)
    val busy = (state === s_idle) && io.in.fire()

    when(busy){
        when(prob === 0.U){state := s_finish}
        state := s_genr
    }.elsewhen(state === s_genr){
        val rand = LFSR16()
        r := Cat(rand(14,10),rand(7,3))
        state := s_comp
    }.elsewhen(state === s_comp){
        when(syn(cnt.value) === 1.U){
            syn_new(cnt.value) := r >= prob
        }
        cnt.inc()
        when(cnt.value =/= (len - 1).U){ 
            state := s_genr
            io.out.bits.ret := syn_new.asUInt}
        when(cnt.value === (len - 1).U){ state := s_finish}
    }.elsewhen(state === s_finish){
        state := s_idle
        io.out.bits.res := syn_new.asUInt
    }
    
    io.in.ready := (state === s_idle)
    io.out.valid := (state === s_finish)
    io.out.bits.cnt := cnt.value
    when (io.in.valid && SNNDebug.enablePrint){
        printf("[stdp.LTD] random = 0x%x\n", r)                                                    
        printf("[stdp.LTD] syn = 0x%x\n", syn)                          
        printf("[stdp.LTD] prob = 0x%x\n", prob)                                            
        printf("[stdp.LTD] res = 0x%x\n", io.out.bits.res)
        printf("[stdp.LTD] valid = %d\n", io.out.valid)
        printf("[stdp.LTD] ready = %d\n", io.in.ready)
        printf("cnt: %d\n\n", cnt.value)                          
    }
    
}