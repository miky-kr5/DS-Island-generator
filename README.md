DS-Island-generator
===================

A procedural island generator based on the diamond-square mid point displacement and the particle deposition algorithms.

Compile with:

    make

Execute as:

    ./ds N PREFIX

Where N is a positive integer greater than zero and PREFIX is a string to append to the output file names. The generated island
will be of size (2^n + 1)x(2^n + 1).

The diamond-square mid point displacement implementation is a port of the code outlined in [this post] [1] at Stack Overflow.

 [1]: http://stackoverflow.com/a/2773032
