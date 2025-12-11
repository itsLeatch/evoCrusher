# evoCrusher
## Install
### PyTorch
To install Torch, download the package from here: https://pytorch.org/get-started/locally/ .
Extract the files somewhere on your device. 
In the CMake file, set the path to the PyTorch config (path/to/libtorch/share/cmake/Torch) under Torch_ROOT, or set the environment variable directly in CMAKE_PREFIX_PATH.

### VCPKG
If you havend allready installed vcpkg, then follow the tutorial on this page: https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell . You can skip step 3. If you the vcpkg toolchain is the standard one, than you can also skip step 4.

Then run:
```
mkdir build
cd build
cmake --build .
```