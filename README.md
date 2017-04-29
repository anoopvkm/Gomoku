# Gomoku
Anoop Ranaganthan Santhoh
Aman Preet Singh Bhatia

Different files - explanation of each and instructions to run them

1 . gomoku_serial.c - Serial implementation og gomoku with alpha beta pruning and human user
	To run : gcc -o test gomoku_serial.c
	         ./test

2 . gomoku_serial_random.c - Serial implementation og gomoku with alpha beta pruning and random player - computer selects a move randomly against AI player
	To run : gcc -o test gomoku_serial_random.c
	         ./test

3 . gomoku_parallel.c - Parallel implementation (master slave)of gomoku with alpha beta pruning and human user
	To run : mpicc gomoku_parallel.c
	         mpirun -n 8 a.out

4 . gomoku_parallel_random.c - parallel implementation(master slave) of gomoku with alpha beta pruning and random player - computer selects a move randomly against AI player
	To run : mpicc gomoku_parallel_random.c
	         mpirun -n 8 a.out


5 . gomoku_parallel_evensplit.c - Parallel implementation (even split)of gomoku with alpha beta pruning and human user
	To run : mpicc gomoku_parallel_evensplit.c
	         mpirun -n 8 a.out

6 . gomoku_parallel_evensplit_random.c - parallel implementation(even split) of gomoku with alpha beta pruning and random player - computer selects a move randomly against AI player
	To run : mpicc gomoku_parallel_evensplit_random.c
	         mpirun -n 8 a.out


7 . gomoku_serial_pvsplit.c - Serial implementation of gomoku with PV split and human user
	To run : gcc -o test gomoku_serial_pvsplit.c
	         ./test

8 . gomoku_serial_pvsplit_random.c - Serial implementation og gomoku with PV split and random player - computer selects a move randomly against AI player
	To run : gcc -o test gomoku_serial_pvsplit_random.c
	         ./test


9 . gomoku_parallel_pvsplit.c - Parallel implementation of gomoku with PV split and human user
	To run : mpicc gomoku_parallel_pvsplit.c
	         mpirun -n 8 a.out

10 . gomoku_parallel_pvsplit_random.c - parallel implementation(even split) of gomoku with PV split and random player - computer selects a move randomly against AI player
	To run : mpicc gomoku_parallel_evensplit_random.c
	         mpirun -n 8 a.out
__________________________________________________________________________________________

MORE DETAILS
1 . Message passing is implemented using MPI 
2 . After a run, it prints average time taken
3 . Use _random files for getting run times (Random moves against AI DFS search)
4 . Use other files to play against Human
