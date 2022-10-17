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

class AdderIO extends Bundle{
    val a = Input(UInt(1.W))
    val b = Input(UInt(1.W))
    val cin = Input(UInt(1.W))
    val sum = Output(UInt(1.W))
    val cout = Output(UInt(1.W))
}

class HalfAdder extends Module{
    val io = IO(new AdderIO)

    io.sum := io.a ^ io.b ^ io.cin
}

class FullAdder extends Module{
    val io = IO(new AdderIO)
        
    io.sum := io.a ^ io.b ^ io.cin
    io.cout := io.a & io.b | (io.a | io.b) & io.cin
}

// Wallace tree adder
class WallaceTree(len: Int = 8)extends Module{
    val io = IO(new Bundle{
        val in = Input(Vec(3,UInt(len.W)))
        val sum = Output(UInt(len.W))
        val overf = Output(Bool())
    })

    val (a, b, d) = (io.in(0), io.in(1), io.in(2))

    val adder0 = Module(new FullAdder)
    val adder1 = Module(new FullAdder)
    val adder2 = Module(new FullAdder)
    val adder3 = Module(new FullAdder)
    val adder4 = Module(new FullAdder)
    val adder5 = Module(new FullAdder)
    val adder6 = Module(new FullAdder)
    val adder7 = Module(new FullAdder)

    // level 1
    adder0.io.a := a(0)
    adder0.io.b := b(0)
    adder0.io.cin := d(0)

    adder1.io.a := a(1)
    adder1.io.b := b(1)
    adder1.io.cin := d(1)

    adder2.io.a := a(2)
    adder2.io.b := b(2)
    adder2.io.cin := d(2)

    adder3.io.a := a(3)
    adder3.io.b := b(3)
    adder3.io.cin := d(3)

    adder4.io.a := a(4)
    adder4.io.b := b(4)
    adder4.io.cin := d(4)

    adder5.io.a := a(5)
    adder5.io.b := b(5)
    adder5.io.cin := d(5)

    adder6.io.a := a(6)
    adder6.io.b := b(6)
    adder6.io.cin := d(6)

    adder7.io.a := a(7)
    adder7.io.b := b(7)
    adder7.io.cin := d(7)

    // level 2
    val sum = Cat(adder7.io.sum, adder6.io.sum, adder5.io.sum, adder4.io.sum, adder3.io.sum, adder2.io.sum, adder1.io.sum, adder0.io.sum)
    val cout = Cat(adder7.io.cout, adder6.io.cout, adder5.io.cout, adder4.io.cout, adder3.io.cout, adder2.io.cout, adder1.io.cout, adder0.io.cout)
    val res = Wire(UInt(9.W))
    res := sum + cout << 1
    io.overf := (res(8) === 1.U)
    io.sum := res(7,0)
}