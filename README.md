connect4-mpi
============

Connect4 game implemented in C++ with MPI. Done for Parallel programming course on FER, Croatia (http://www.fer.unizg.hr/en/course/parpro).

Everything is implemented just in order to show how computation time can be reduced by using MPI and parallel computing.

No pruning or similar techniques are implemented.

## <a name="install"></a> Install
Have to be done

## <a name="running"></a> Running
Have to be done

## <a name="performance"></a> Performance

Testing is done on computer with 64GB of RAM and 2 Intel Xeon E5646 processors (24 cores total).

### depth = 8 (without -O2)
| n | time (msec) | efficiency | speedup |
|:-:|-------------|------------|---------|
| 1 |       11695 |     1.0000 |  1.0000 |
| 2 |       14515 |     0.4029 |  0.8057 |
| 3 |        8313 |     0.4689 |  1.4068 |
| 4 |        6266 |     0.4666 |  1.8664 |
| 5 |        5279 |     0.4431 |  2.2154 |
| 6 |        4713 |     0.4134 |  2.4814 |
| 7 |        4272 |     0.3910 |  2.7376 |
| 8 |        3781 |     0.3866 |  3.0931 |

### depth = 9 (without -O2)
| n | time (msec) | efficiency | speedup |
|:-:|-------------|------------|---------|
| 1 |       80023 |     1.0000 |  1.0000 |
| 2 |       84716 |     0.4723 |  0.9946 |
| 3 |       42578 |     0.6265 |  1.8795 |
| 4 |       29604 |     0.6758 |  2.7031 |
| 5 |       22963 |     0.6970 |  3.4849 |
| 6 |       19263 |     0.6924 |  4.1542 |
| 7 |       16305 |     0.7012 |  4.9079 |
| 8 |       14205 |     0.7042 |  5.6334 |

### depth = 10 (with -O2)
| n | time (msec) | efficiency | speedup |
|:-:|-------------|------------|---------|
| 1 |      112179 |     1.0000 |  1.0000 |
| 2 |      123446 |     0.4543 |  0.9086 |
| 3 |      062084 |     0.6023 |  1.8069 |
| 4 |      042971 |     0.6526 |  2.6104 |
| 5 |      033724 |     0.6653 |  3.3265 |
| 6 |      028707 |     0.6513 |  3.9078 |
| 7 |      025085 |     0.6389 |  4.4723 |
| 8 |      021641 |     0.6480 |  5.1840 |

## Efficiency chart
![Efficiency chart](https://raw.githubusercontent.com/mariokostelac/connect4-mpi/master/img/efficiency.png)

## Speedup chart
![Speedup chart](https://raw.githubusercontent.com/mariokostelac/connect4-mpi/master/img/speedup.png)
