# Usage

In hardware directory of Wenquxing 22A(`Wenquxing22A/hw/`):
```
$ make IMAGE=verify/snn/xxx.bin emu
```
where `xxx.bin` is the file to test all snn instruction extentions.

We provide 6 test programs:
- `calcOpTest.bin` : It tests basic snn calculation operations including ANDS, RPOP, INF, SGE, SLS, and VLEAK.
- `neuronTest.bin` : It tests instructions related to neuron updating: SINIT, VLEAK, and NADD.
- `synTest.bin`  :   It tests instructions related to synapses updating: SINIT and SUP.
- `LTDTest.bin` :    It tests LTD instruction.
- `neuronUpdateExample.bin` : It simulates the updating of 5 lif neurons in the inference process.
- `snnLoadStore.bin` : It tests NST and NLD instructions.