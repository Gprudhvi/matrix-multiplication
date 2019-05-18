/*

*/

#include <iostream>			//C++ headers
#include <thread>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <queue>
#include <mutex>

#include <pthread.h>		//C headers
#include <stdio.h>
#include <omp.h>
#include <unistd.h>

using namespace std;
mutex mtx;
vector<vector<vector<int> > > partialsums;
		
class Matrix{				//implements the 
	public:
		int n1;
		vector<vector<int> > matA;
		vector<vector<int> > matB;
		vector<vector<int> > matC;
		int num_threads;
		
		
		Matrix(int size1){
			n1 = size1;						
			for (int i = 0; i < n1; i++){
				vector<int> temp;
				for (int j = 0; j < n1; j++){
					temp.push_back(rand()%10);
				}
				matA.push_back(temp);
			}
			for (int i = 0; i < n1; i++){
				vector<int> temp;
				for (int j = 0; j < n1; j++){
					temp.push_back(rand()%10);
				}
				matB.push_back(temp);
			}
			for (int i = 0; i < n1; i++){
				vector<int> temp(n1, 0);
				matC.push_back(temp);
			}		
		}


		vector<int> scalar_vector_product(int v1, vector<int> vec){
			for(int i=0; i< vec.size(); i++){
				vec[i] = v1*vec[i];
			}
			return vec; 
		}

		void summa_partial(int index){
			vector<vector<int> > tempMat;
			for (int i = 0; i < n1; i++){
				tempMat.push_back(scalar_vector_product(matA[index][i],matB[index]));
			}
			
			mtx.lock();
			partialsums.push_back(tempMat);
			mtx.unlock();
			return;
		}

		void summa_add(int i1, int i2){
			for (int i = i1; i < i2; i++){
				for (int j = 0; j < n1; j++){
					for (int k = 0; k < n1; k++){
						matC[i][j] += partialsums[k][i][j];
					}
				}
			}
			return;
		}
};


int main(){
	Matrix testMat(120)
	;
	
	// for(auto e : testMat.matA){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<"  ";
	// 	}
	// 	cout<<endl;
	// }

	// for(auto e : testMat.matB){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<"  ";
	// 	}
	// 	cout<<endl;
	// }

	vector<thread> workers(testMat.n1);
	double startTime, exitTime;
	startTime = omp_get_wtime();

	for (int i = 0; i < testMat.n1; i++){
		workers[i] = thread(&Matrix::summa_partial,testMat,i);
	}

	for (int i = 0; i < testMat.n1; i++){
		workers[i].join();
	}
		
	if (testMat.n1 > 3){
		int adder_threads = 4;
		thread adders[adder_threads];
		int s1 = 0;
		int s2 = testMat.n1/adder_threads;
		for (int i = 0; i < adder_threads; i++){
			adders[i] = thread(&Matrix::summa_add,testMat,s1,s2);
			s1 = s2;
			s2 += testMat.n1/adder_threads;
		}

		for (int i = 0; i < adder_threads; i++){
			adders[i].join();
		}
	}
	else
		testMat.summa_add(0,testMat.n1);

	exitTime = omp_get_wtime();
	printf("timetaken: %lf\n", exitTime-startTime);

	// for(auto e : testMat.matC){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<" ";
	// 	}
	// 	cout<<endl;
	// }

	

	return 0;
}