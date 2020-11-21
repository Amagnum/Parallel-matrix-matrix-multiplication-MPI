# Matrix-matrix-multiplication-MPI
A simple MPI program to compute the matrix matrix multiplication. Splitting the matrix A rowwise, and distribute it to different processes. Comparing the runtime using 1, 2 and 4 processors.

### C++ code for matrix multiplication
```cpp
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            res[i][j] = 0;
            for (k = 0; k < N; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
```

### Results:
Intel(R) Core(TM) i5-8250U CPU @ 1.60GHz <br>
CPU(s):              8 <br>
Thread(s) per core:  2 <br>
![alt approach](https://github.com/Amagnum/Dot-Product-of-2-vectors-MPI/blob/main/img/Dot%20Product%20of%202%20Vectors.png)

### Compile & run the code:
```
$ mpic++ main.cpp -o exc
$ mpirun -np 4 exc
```

## REFERENCES
1. Chandresh Kumar Maurya, Assisant professor, IIT Indore [link](https://chandu8542.github.io/)
2. Advanced Message Passing in MPI, Using MPI Datatypes with Opaque C++ Types, Paul Preney [pdf link](https://www.sharcnet.ca/help/images/f/fa/MPI_Datatypes_with_Opaque_Types_seminar_2014.pdf)
