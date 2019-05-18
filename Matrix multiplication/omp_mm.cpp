#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <iostream>

using namespace std;

struct arg_struct {
    int row;
    int col;
};

int n;
int A[2500][2500], B[2500][2500], C[2500][2500];


void *thdwork(void* values){
	struct arg_struct *vals = (struct arg_struct*) values;
	int row = vals->row;
	long col = vals->col;
	

}

int main(){
	cin>>n;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j){
			A[i][j] = rand()%10;
			B[i][j] = rand()%10;
			
		}
	}

	double startTime, exitTime;
 //     startTime = omp_get_wtime();
	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < n; ++j){
	// 		C[i][j] = 0;
	// 		for (int k = 0; k < n; ++k){
	// 			C[i][j] += A[i][k]*B[k][j];
	// 		}
	// 	}
	// }

	//  exitTime = omp_get_wtime();
	// printf("timetaken: %lf\n", exitTime-startTime);


	startTime = omp_get_wtime();
	#pragma omp_set_num_threads(10) //private(i,j,k)shared(A,B,C)
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j){
			C[i][j] = 0;
			for (int k = 0; k < n; ++k){
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}

	 exitTime = omp_get_wtime();
	printf("timetaken: %lf\n", exitTime-startTime);

	// startTime = omp_get_wtime();
	// #pragma omp parallel for collapse(2) 
	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < n; ++j){
	// 		C[i][j] = 0;
	// 		for (int k = 0; k < n; ++k){
	// 			C[i][j] += A[i][k]*B[k][j];
	// 		}
	// 	}
	// }

	//  exitTime = omp_get_wtime();
	// printf("timetaken: %lf\n", exitTime-startTime);

	// startTime = omp_get_wtime();
	// #pragma omp parallel for schedule(static) collapse(2) 
	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < n; ++j){
	// 		C[i][j] = 0;
	// 		for (int k = 0; k < n; ++k){
	// 			C[i][j] += A[i][k]*B[k][j];
	// 		}
	// 	}
	// }

	//  exitTime = omp_get_wtime();
	// printf("timetaken: %lf\n", exitTime-startTime);
	
	// #pragma omp parallel for schedule(dynamic) collapse(2) //private(i,j,k)shared(A,B,C)
	// for (int i = 0; i < n; ++i) {
	// 	for (int j = 0; j < n; ++j){
	// 		C[i][j] = 0;
	// 		for (int k = 0; k < n; ++k){
	// 			C[i][j] += A[i][k]*B[k][j];
	// 		}
	// 	}
	// }

	//  exitTime = omp_get_wtime();
	// printf("timetaken: %lf\n", exitTime-startTime);

	// // for (int i = 0; i < n; ++i)
	// // {
	// // 	for (int j = 0; j < n; ++j)
	// // 		cout<<A[i][j]<<"  ";
	// // 	cout<<endl;
	// // }
	// // cout<<endl;

	// // for (int i = 0; i < n; ++i)
	// // {
	// // 	for (int j = 0; j < n; ++j)
	// // 		cout<<B[i][j]<<"  ";
	// // 	cout<<endl;
	// // }

	// // cout<<endl;

	// // for (int i = 0; i < n; ++i)
	// // {
	// // 	for (int j = 0; j < n; ++j)
	// // 		cout<<C[i][j]<<"  ";
	// // 	cout<<endl;
	// // }
	return 0;
}
