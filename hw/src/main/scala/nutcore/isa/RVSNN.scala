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

object RVSNNInstr extends  HasInstrType{
    def ANDS    = BitPat("b0000000_?????_?????_000_?????_0001011")
    def SGE     = BitPat("b0000000_?????_?????_100_?????_0001011")
    def RPOP    = BitPat("b0000000_00000_?????_001_?????_0001011")
    def SLS     = BitPat("b0000001_00000_?????_001_?????_0001011")
    def INF     = BitPat("b0000010_00000_?????_001_?????_0001011")
    def SUP     = BitPat("b0000011_?????_?????_001_?????_0001011")
    def VLEAK   = BitPat("b0000100_00000_?????_001_?????_0001011")
    def NADD    = BitPat("b0000000_?????_?????_010_?????_0001011")
    def NST     = BitPat("b???????_?????_?????_011_?????_0001011")// neuron store
    //def SST     = BitPat("b???????_?????_?????_100_?????_0001011")// synapses store
    def NLD     = BitPat("b????????????_?????_101_?????_0001011")// neuron load
    def LTD     = BitPat("b0000000_?????_?????_110_?????_0001011")// LTD
    def SINIT   = BitPat("b????????????????_?_111_?????_0001011")
                         
    val table = Array(
        ANDS        -> List(InstrSNNR,   FuType.snn,   SNNOpType.ands),
        SGE         -> List(InstrSNNR,   FuType.snn,   SNNOpType.sge),
        RPOP        -> List(InstrSNNR,   FuType.snn,   SNNOpType.rpop),
        SLS         -> List(InstrSNNR,   FuType.snn,   SNNOpType.sls),
        INF         -> List(InstrSNNsp,   FuType.snn,   SNNOpType.inf),
        SUP         -> List(InstrSNNR,   FuType.snn,   SNNOpType.sup),
        VLEAK       -> List(InstrSNNR,   FuType.snn,   SNNOpType.vleak),
        NADD        -> List(InstrSNNR,   FuType.snn,   SNNOpType.nadd),
        NST         -> List(InstrSNNS,   FuType.lsu,   LSUOpType.nst),
        //SST         -> List(InstrSNNS,   FuType.lsu,   LSUOpType.sst),
        NLD         -> List(InstrSNNL,   FuType.lsu,   LSUOpType.nld),
        LTD         -> List(InstrSNNR,   FuType.snn,   SNNOpType.ltd),
        SINIT       -> List(InstrSNNU,   FuType.snn,   SNNOpType.sinit)
        )
}