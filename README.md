# nex

[![Build Status](https://travis-ci.org/Statoil/nex.svg?branch=master)](https://travis-ci.org/Statoil/nex)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/20683e5e1e1b4196972ab68d1f6154a3)](https://www.codacy.com/app/ReedOnly/nex?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Statoil/nex&amp;utm_campaign=Badge_Grade)


## Introduction ##
Nex2ecl is an application for converting Nexus plot files to Eclipse
summaries. In the current version nex is supported on Linux and OSX. After
installation the application can be launched from terminal with the command
`nex2ecl`.

## Installation ##

To build nex you need:

* CMake version 2.8.12 or greater
* Python 2.7
* pandas version 0.21.1 or greater
* setuptools version 28 or greater



To install nex2ecl from source execute the following:

```sh
git clone https://github.com/Statoil/nex
mkdir nex/build
cd nex/build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install
```


## History ##
Nex was initially written and is maintained by [Statoil
ASA](http://www.statoil.com/) as a free, simple, easy-to-use way of converting
Nexus summary files to similar Eclipse files tailored to our needs, and as
contribution to the free software community.