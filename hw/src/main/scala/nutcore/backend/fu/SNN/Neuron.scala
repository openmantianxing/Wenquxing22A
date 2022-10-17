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

import utils._

class NeuronIO(val len: Int) extends NutCoreBundle{
    val in = Flipped(DecoupledIO(new Bundle{
        val src1        = Input(UInt(len.W))
        val src2        = Input(UInt(len.W))
        val imm         = Input(UInt(len.W))
        val vinit       = Input(UInt(len.W))
        val vleak       = Input(UInt(len.W))
        val spike       = Input(UInt(len.W))
        val option      = Input(UInt(len.W))
    }))
    val out = DecoupledIO(Output(UInt(len.W)))
}

class NeurModule(len: Int) extends NutCoreModule{
    val io = IO(new NeuronIO(len))

    val valid = io.in.valid
    val src1 = io.in.bits.src1
    val src2 = io.in.bits.src2
    val imm = io.in.bits.imm
    val option = io.in.bits.option
    //val wlt = Module(new WallaceTree)
    //wlt.io.in(0) := src1(7,0)
    //wlt.io.in(1) := src2(7,0)
    //wlt.io.in(2) := (imm(7,0) ^ Fill(8, 1.U)) + 1.U
    //val sum = RegInit(0.U(len.W))
    //sum := wlt.io.sum
    val nadd = src1(63,1) + src2 + ((io.in.bits.vleak ^ Fill(len, 1.U)) + 1.U)
    val naddRes = nadd << 1
    //val overflow = WireInit(false.B)
    //overflow := wlt.io.overf
    val spike  = src1(63,1) >= src2
    val slsRes = Mux(io.in.bits.vinit(0) === 1.U, ZeroExt(src1 >> 1, len), (src1 << 1) + io.in.bits.spike(0))
    val sgeRes = Mux(spike && option === SNNOpType.sge && valid, Cat(io.in.bits.vinit(63, 1), 1.U), Cat(src1(63,1), 0.U))

    io.out.bits := LookupTree(option, List(
        SNNOpType.nadd      ->  naddRes,
        SNNOpType.sls       ->  slsRes,
        SNNOpType.sge       ->  sgeRes
    ))
    when (valid && SNNDebug.enablePrint){
        printf("[neuron]option = 0x%x\n",option)
        printf("[neuron]src1 = 0x%x\n", src1)
        printf("[neuron]src2 = 0x%x\n", src2)
        printf("[neuron]imm = 0x%x\n", imm)
        printf("[neuron]io.in.bits.vinit = 0x%x\n", io.in.bits.vinit)
        when (option === SNNOpType.nadd) { printf("[neuron]naddRes = 0x%x\n", naddRes) }
        when (option === SNNOpType.sls) { printf("[neuron]slsRes = 0x%x\n", slsRes) }
        when (option === SNNOpType.sge) { printf("[neuron]sgeRes = 0x%x\n", sgeRes) }
        printf("\n")
    }

    io.in.ready := io.out.ready
    io.out.valid := valid
}