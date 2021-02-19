## Small obect allocator
### Disclaimer
I stole the idea and technology from Andrei Alexandrescu [book](http://index-of.co.uk/C++/C++%20Design%20Generic%20Programming%20and%20Design%20Patterns%20Applied.pdf)
But the realization is a result of my own mind efforts
### How to use
It is easy to start use small object allocator in your project, all you need is inherit your class or struct from ``hope::memory::small_object``.
[See](https://github.com/glensand/hope/blob/master/test/memory/sm_objects.h) for more examples.
It works due to ``operator new`` and ``operator delete`` overloading, thus you do not have to change your code after *sm* inheritance;
