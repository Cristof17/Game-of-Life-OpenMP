#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define DEAD 0
#define ALIVE 1

char *fisier_in; //initial state of the problem 
int N; //number of iterations
char *fisier_out; //final state of the problem

int L; //lines of the matrix
int C;  //columns of the matrix

char **matrix;
int **bools;
int **bools_normal;
int **bools_buffered;
int **current;
int **current_backup;

void extend_with_margins(int L, int C, int **begin, int **end){

	for (int i = 0; i < L; ++i){
		end[i + 1][0] = begin[i][C - 1];
		end[i + 1][C - 1 + 1] = begin[i][0];
	}

	for (int i = 0; i < C; ++i){
		end[0][i + 1] = begin[0][i];
		end[L - 1 + 1][0] = begin[0][i];
	}

	end[0][0] = begin[L-1][C-1];
	end[L- 1 + 1][0] = begin[0][C-1];
	end[L - 1 + 1][C -1 + 1] = begin[0][0];
	end[0][C - 1 + 1] = begin[L-1][C-1];

	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			end[i+1][j+1] = begin[i][j];
		}
	}

}

void transform_in_bool(int L, int C, char **chars, int** bools){
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

void simulate_matrix(int L,int C,int **start,int **end){
	int alive = 0;
	int dead = 0;

	for (int i = 1; i < L - 1 ; ++i){ //start from (1,1) because we have added borders
		for (int j = 1; j < C - 1; ++j){ //start from (1,1) because we have added borders
			//count alive and dead neighbors
			(start[i - 1][j - 1] == ALIVE) ? alive++ : dead++;
			(start[i - 1][j + 0] == ALIVE) ? alive++ : dead++;
			(start[i - 1][j + 1] == ALIVE) ? alive++ : dead++;
			(start[i + 0][j - 1] == ALIVE) ? alive++ : dead++;
			(start[i + 0][j + 1] == ALIVE) ? alive++ : dead++;
			(start[i + 1][j - 1] == ALIVE) ? alive++ : dead++;
			(start[i + 1][j + 0] == ALIVE) ? alive++ : dead++;
			(start[i + 1][j + 1] == ALIVE) ? alive++ : dead++;

			//check the values and put them in the buffered matrix
			if (alive < 2)
				end[i][j] = DEAD;
			if (start[i][j] == DEAD && alive == 3)
				end[i][j] = ALIVE;
			if (alive > 3)
				end[i][j] = DEAD;
			if (start[i][j] == ALIVE && (alive == 2 || alive == 3))
				end[i][j] = ALIVE;
			alive = 0;
			dead = 0;
		}
	}
}

void print(int **mat, int L, int C){
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}

void alloc_buffer_matrix(int L, int C, int ***bools){
	(*bools) = (int**)malloc(L * sizeof(int *));
	for (int i = 0; i < L; ++i){
		(*bools)[i] = (int*)malloc(C * sizeof(int));
	}
}

void save_to_file(FILE *file, int L, int C, int **matrix){
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			if (matrix[i][j] == DEAD)
				fprintf(file, ". ");
			else
				fprintf(file, "X ");
		}
		fprintf(file, "\n");
	}
	fflush(file);
}

void copy_matrix(int L, int C, int **start, int **stop){
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			stop[i][j] = start[i][j];
		}
	}
}

void reset_matrix(int L, int C, int **matrix){
	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			matrix[i][j] = DEAD;
		}
	}
}

void shrink_without_margins(int L, int C, int** start, int **end){
	for (int i = 1; i < L -1; ++i)
		for (int j = 1; j < C - 1; ++j)
			end[i-1][j-1] = start[i][j];
}



int main(int argc, char **argv){
	
	clock_t begin = clock();

	if (argc <= 1){
		printf("./g_serial <initial_file> <number_of_iterations> <final_file>");
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

	bools = (int **)malloc(L * sizeof(int *));
	for (int i = 0; i < L; ++i){
		bools[i] = (int *)malloc(C * sizeof(int));
	}

	bools_normal = (int **) malloc ((L + 2) * sizeof(int *));
	for (int i = 0; i < L + 2; ++i){
		bools_normal[i] = (int *)malloc((C + 2)* sizeof(int));
	}

	for (int i = 0; i < L; ++i){
		for (int j = 0; j < C; ++j){
			fscanf(f_in, "%c ", &matrix[i][j]);
		}
	}

	transform_in_bool(L, C, matrix, bools);
	//from here we will only work with bools_normal and bools_buffered
	alloc_buffer_matrix(L + 2, C + 2, &bools_buffered);
	printf("Bools = \n");
	print(bools, L, C);

	
	for (int i = 0; i < N ; ++i){
		printf("Rulez\n");
		if (i % 2 == 0){
			current = bools_normal;
		}else{
			current = bools_buffered;
		}
		extend_with_margins(L, C, bools, current); 
		if (i % 2 == 0){
			simulate_matrix(L + 2, C + 2, current, bools_buffered);
			reset_matrix(L, C, bools);
			shrink_without_margins(L + 2, C + 2, bools_buffered, bools);
		}else{ 
			simulate_matrix(L + 2, C + 2, current, bools_normal);
			reset_matrix(L, C, bools);
			shrink_without_margins(L + 2, C + 2, bools_normal, bools);
		}

		//copy_matrix(L + 2, C + 2, bools_buffered, bools_normal);
	}
	printf("Bools = \n");
	print(bools, L, C);

	save_to_file(f_out, L, C, bools);
	fclose(f_in);
	fclose(f_out);
	
	
	clock_t end = clock();
	double time_spent = (double)(end - begin)/CLOCKS_PER_SEC;
	printf("Executed in %lf\n", time_spent);
	
	return 0;
}
