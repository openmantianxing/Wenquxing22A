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

class SspIO(val len: Int) extends NutCoreBundle{
    val in = Flipped(DecoupledIO(new Bundle{
        val src1   = Input(UInt(len.W))
        val src2   = Input(UInt(len.W))
        val op     = Input(UInt(len.W))
        }))
    val out = DecoupledIO(Output(UInt(len.W)))
}

class SpikeProc(val len: Int) extends NutCoreModule{
    val io = IO(new SspIO(len))

    val (valid, src1, src2, op) = (io.in.valid, io.in.bits.src1, io.in.bits.src2, io.in.bits.op)
    
    def isAnds(op:UInt): Bool = op === SNNOpType.ands
    val andsRes = src1 & src2
    val regPopRes = PopCount(src1)

    io.out.bits := Mux(isAnds(op), andsRes, regPopRes)
    
    io.in.ready := io.out.ready
    io.out.valid := valid
}
