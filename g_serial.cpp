#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

#define DEAD false
#define ALIVE true

char *fisier_in; //initial state of the problem 
int N; //number of iterations
char *fisier_out; //final state of the problem

int L; //lines of the matrix
int C;  //columns of the matrix

char **matrix;
bool **bools;
bool **bools2;

void extend_with_margins(int L, int C, bool **begin, bool ***end){

	(*end) =(bool **)malloc((L + 2) * sizeof(bool *));
	for (int i = 0; i < L; ++i){
		(*end)[i] = (bool *)malloc((C + 2) * sizeof(bool));
	}
	for (int i = 0; i < L; ++i){
		(*end)[i + 1][0] = begin[i][C - 1];
		(*end)[i + 1][C - 1 + 1] = begin[i][0];
	}

	cout << "C =" << C << "L =" << L << endl;
	
	/*

	for (int i = 0; i < C; ++i){
		(*end)[0][i + 1] = begin[0][i];
		(*end)[L - 1 + 1][0] = begin[0][i];
	}

	(*end)[0][0] = begin[L-1][C-1];
	(*end)[L- 1 + 1][0] = begin[0][C-1];
	(*end)[L - 1 + 1][C -1 + 1] = begin[0][0];
	(*end)[0][C - 1 + 1] = begin[L-1][C-1];

	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			(*end)[i+1][j+1] = begin[i][j];
		}
	}
	*/

}

void transform_in_bool(int L, int C, char **chars, bool** bools){
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			if (chars[i][j] == '.'){
				bools[i][j] = DEAD;
			} else if (chars[i][j] == 'X') {
				bools[i][j] = ALIVE;
			}
		}
	}
}

void simulate_matrix(int L,int C,bool **start,bool ** end){
	int alive_neigh = 0;
	int dead_neigh = 0;
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			for(int k = -1; k <= 1; ++j){
			}
		}
	}
			
}

void print(bool **mat, int L, int C){
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			cout << mat[i][j] << " ";
		}
	cout << endl;
	}
}

int main(int argc, char **argv){
	
	if (argc <= 1){
		cout << "./g_serial <initial_file> <number_of_iterations> <final_file>" << endl;
		return -1;
	}



	fisier_in = argv[1];
	N = atoi(argv[2]);
	fisier_out = argv[3];

	FILE *f_in = fopen(fisier_in, "rw+");
	FILE *f_out = fopen(fisier_out, "rw+");
	fscanf(f_in, "%d %d\n", &L, &C);	

	matrix =(char **)malloc(L * sizeof(char *));
	for (int i = 0; i < L; ++i){
		matrix[i] = (char *)malloc(C * sizeof(char));
	}

	bools = (bool **)malloc(L * sizeof(bool*));
	for (int i = 0; i < L; ++i){
		bools[i] = (bool*)malloc(C * sizeof(bool));
	}

	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			fscanf(f_in, "%c ", &matrix[i][j]);
		}
	}

	transform_in_bool(L, C, matrix, bools);
	print(bools, L, C);

	extend_with_margins(L, C, bools, &bools2); 
	/*
	print(bools2, L + 2, C + 2);
	for (int i = 0; i < N/2; ++i){
		simulate_matrix(L, C, bools2, bools);
		simulate_matrix(L, C, bools, bools2);
	}*/
	
	return 0;
}
