/*

*/

#include <iostream>			//C++ headers
#include <thread>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

#include <pthread.h>		//C headers
#include <stdio.h>
#include <omp.h>
#include <unistd.h>


using namespace std;


class Matrix{				//implements the bakery lock
	public:
		int n1, n2, n3;
		vector<vector<int> > matA;
		vector<vector<int> > matB;
		vector<vector<int> > matC;


		vector<vector<int> > transpose(vector<vector<int> > mat){ 
		    int rows = mat.size();
		    int cols = mat[0].size();
		    vector<vector<int> > transMat(rows, vector<int>(cols));

		    for (int i = 0; i < rows; i++){ 
		        for (int j = 0; j < cols; j++){ 
		            transMat[i][j] = mat[j][i]; 
		        }
		    }
		    return transMat;
		} 

		Matrix(int size1, int size2, int size3){
			n1 = size1;
			n2 = size2;
			n3 = size3;
			
			for (int i = 0; i < n1; i++){
				vector<int> temp;
				for (int j = 0; j < n2; j++){
					temp.push_back(rand()%10);
				}
				matA.push_back(temp);
			}

			for (int i = 0; i < n3; i++){
				vector<int> temp;
				for (int j = 0; j < n2; j++){
					temp.push_back(rand()%10);
				}
				matB.push_back(temp);
			}


			for (int i = 0; i < n1; i++){
				vector<int> temp(n3, 0);
				matC.push_back(temp);
			}
			
		}

		int dot_product(vector<int> v1, vector<int> v2){
			int val = 0;
			if (v1.size() != v2.size()){
				return -1;
			}
			for(int i=0; i< v1.size(); i++){
				val += v1[i]*v2[i];
			}
			return val; 
		}

		void seqMultiply(){
			cout<<matC[0][0];
			for (int i = 0; i < n1; i++){
				for (int j = 0; j < n3; j++){
					matC[i][j] = dot_product(matA[i],matB[j]);
				}
			}
			return;
		}

};


void thdworkparallel1(vector<int> v1, vector<int> v2, int* val ){
	val = 0;
	for(int i=0; i< v1.size(); i++){
		val += v1[i]*v2[i];
	}
	return;
}
	

int main(){
	close();
  
    Matrix testMat(50,50,50);

// printing all the matrices
 //    for(auto e : testMat.matA){
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

	// for(auto e : testMat.matC){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<"  ";
	// 	}
	// 	cout<<endl;
	// }


/* Sequential matrix multplication*/
    double startTime, exitTime;
    startTime = omp_get_wtime();

    testMat.seqMultiply();
    
    exitTime = omp_get_wtime();
	printf("timetaken: %lf\n", exitTime-startTime);

	/*for(auto e : testMat.matC){
		for(auto e_sub : e){
			cout<<e_sub<<" ";
		}
		cout<<endl;
	}
*/

/* one element per thread parallel multiplication */
	thread workers[testMat.n1*testMat.n3];
	startTime = omp_get_wtime();
    
	for (int i = 0; i < testMat.n1; i++){	
		for (int j = 0; j < testMat.n3; j++){			
			workers[i*testMat.n1+j] = thread(thdworkparallel1, testMat.matA[i], testMat.matB[j], &testMat.matC[i][j]);		//assigning work to threads 
		}
	}
		
	for (int i = 0; i < testMat.n1; i++){	
		for (int j = 0; j < testMat.n3; j++){			
			workers[i*testMat.n1+j].join();
		}
	}

    exitTime = omp_get_wtime();
	printf("timetaken for thread: %lf\n", exitTime-startTime);

	// for(auto e : testMat.matC){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<" ";
	// 	}
	// 	cout<<endl;
	// }

	

	return 0;
}