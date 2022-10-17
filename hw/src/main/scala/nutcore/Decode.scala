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

trait HasInstrType {
  def InstrN    = "b00000".U
  def InstrI    = "b00100".U
  def InstrR    = "b00101".U
  def InstrS    = "b00010".U
  def InstrB    = "b00001".U
  def InstrU    = "b00110".U
  def InstrJ    = "b00111".U
  def InstrA    = "b01110".U
  def InstrSA   = "b01111".U // Atom Inst: SC
  def InstrSNNS  = "b10000".U  // SNN store
  def InstrSNNL  = "b10101".U // SNN load
  def InstrSNNR  = "b10110".U // SNN R-Type
  def InstrSNNU  = "b10111".U // SNN U-type
  def InstrSNNsp = "b11111".U 


  def isrfWen(instrType : UInt): Bool = instrType(2)
}

// trait CompInstConst {
//   val RVCRegNumTable = Array(
//     BitPat("b000") -> 8.U,
//     BitPat("b001") -> 9.U,
//     BitPat("b010") -> 10.U,
//     BitPat("b011") -> 11.U,
//     BitPat("b100") -> 12.U,
//     BitPat("b101") -> 13.U,
//     BitPat("b110") -> 14.U,
//     BitPat("b111") -> 15.U
//   )
// }

object SrcType {// add sreg
  def reg = "b00".U
  def pc  = "b01".U
  def imm = "b01".U
  def sreg = "b10".U
  def apply() = UInt(2.W)
}

object FuType extends HasNutCoreConst {
  def num = 6
  def alu = "b000".U
  def lsu = "b001".U
  def mdu = "b010".U
  def csr = "b011".U
  def mou = "b100".U
  def snn = "b101".U
  def bru = if(IndependentBru) "b110".U
            else               alu
  def apply() = UInt(log2Up(num).W)
}

object FuOpType {
  def apply() = UInt(7.W)
}

object Instructions extends HasInstrType with HasNutCoreParameter {
  def NOP = 0x00000013.U
  val DecodeDefault = List(InstrN, FuType.csr, CSROpType.jmp)
  def DecodeTable = RVIInstr.table ++ NutCoreTrap.table ++
    (if (HasMExtension) RVMInstr.table else Nil) ++
    (if (HasCExtension) RVCInstr.table else Nil) ++
    Priviledged.table ++
    RVAInstr.table ++
    RVZicsrInstr.table ++ RVZifenceiInstr.table ++
    RVSNNInstr.table
}

object CInstructions extends HasInstrType with HasNutCoreParameter{
  def NOP = 0x00000013.U
  val DecodeDefault = List(RVCInstr.ImmNone, RVCInstr.DtCare, RVCInstr.DtCare, RVCInstr.DtCare)
  // val DecodeDefault = List(InstrN, FuType.csr, CSROpType.jmp)
  def CExtraDecodeTable = RVCInstr.cExtraTable
}
