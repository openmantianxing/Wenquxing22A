# Programs for Wenquxing 22A
## File Structure
```
sw/
├── swSim-mnist                     # Python-version scripts for MNIST Recognition
│   └── scripts                     # Python scripts
└── swTest-mnist
    ├── fpga_apps                   # C program for MNIST recognition on FPGA
    │   ├── include
    │   │   ├── fpga_data
    │   │   ├── snn.h               # snn inline C code
    │   │   └── snn_portme.h        # platform parameters
    │   └── src
    │       └── standard_train.c    # Training program
    └── fpga_data                   # Generated spikes data
```

## Usage
### Python Version
The python scripts only need `Numpy` and `scipy` as thier dependences.Some important scripts:
- `standard_training.py` : Basic training for 10 Streamlined LIF neurons;
- `preprocessing.py` : To use OTSU method to achieve image enhancement, to deskew, and to generate the spikes depending on Poisson Spike Generator;
- `active_learning.py` : To enable active learning. 

### C Program
1. You need to modify the parameters in `snn_portme.h` depending on what your platform has;
2. You need to use riscv toolchain to compile the C program;
3. You can run your application according to [Nutshell-fpga-guidance](https://github.com/ssdfghhhhhhh/NutShell_U250) or by yourself.