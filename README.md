# nex

## Installation

To build nex you need:

* CMake version 2.8.12 or greater
* Python 2.7
* pandas version 0.21.1 or greater
* setuptools version 28 or greater

To install nex from source execute the following:

```sh
git clone https://github.com/Statoil/nex
mkdir nex/build
cd nex/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install
```
