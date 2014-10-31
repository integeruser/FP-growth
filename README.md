# FP-growth
This repository contains my C++ implementation of the well-known **FP-growth** algorithm, published in the hope that it will be useful. I carefully tested the code on three different samples and results were checked against [this other implementation](http://www.borgelt.net/fpgrowth.html) of the algorithm, but please don't assume the code is completely bug-free. Feel free to use the [issue tracker](https://github.com/integeruser/FP-growth/issues) to report any problem.  

The files `fptree.hpp` and `fptree.cpp` contain the data structures and the algorithm, while `main.cpp` contains only a few tests.  

Compile the code using the appropriate options for C++11 (e.g. `-std=c++11` using g++).  

Happy coding! :v:  


## License
The MIT License (MIT)

Copyright (c) 2014 Francesco Cagnin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
