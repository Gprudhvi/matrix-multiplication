#include <iostream>
#include <thread>
#include <ctime>
#include <atomic>
#include <bits/stdc++.h>
#include <mutex>
#include <time.h>
#include <chrono>

#include <vector>
#include <fstream>
#include <random>
#include <algorithm>

#include <pthread.h>		//C headers
#include <stdio.h>
#include <omp.h>
#include <unistd.h>


using namespace std;


struct my_id
{
	int a[4];
};

my_id nul;

class Matrix{				//implements the bakery lock
	public:
		int n1;
		vector<vector<int> > matA;
		vector<vector<int> > matB;
		vector<vector<int> > matC;
		
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

		void compute_row(int row){
			for (int j = 0; j < n1; j++){
				for (int k = 0; k < n1; k++){
					matC[row][j] = dot_product(matA[row],matB[j]);
				}
			}

			return;
		}

};


Matrix* testMat;

class BDEqueue{
	public:

	vector<my_id> tasks;
	// atomic<int> bottom;
	// atomic<int> top;	
	int top=0, bottom=0;
	
	BDEqueue(int capacity)
	{
		tasks.resize(capacity);
		// top=0;
	}

	void pushbottom(my_id r)
	{
		tasks[bottom] =r;
		bottom++;
		// cout<<bottom;
	}

	bool isempty(){
		return(bottom <= top);
	}

	my_id pop_top(){
		int stamp[1];
		int oldstamp = stamp[0];
		int newstamp = oldstamp+1;

		int oldtop = top;
		int newtop = oldtop+1;
		
		if(bottom <= oldtop){
			return nul;
		}

		if(top == oldtop){	
			top = newtop;
			return tasks[oldtop];
		}

		return nul;
	}

	my_id pop_bottom(){
		if(bottom == 0)
			return nul;
		bottom--;
		my_id r = tasks[bottom];
		
		int oldtop = top;
		int newtop = 0;
		
		if(bottom > oldtop){
			return r;
		}

		if(bottom == oldtop){
			bottom = 0;
			if(top == oldtop){
				top = newtop;
				return r;
			}
		}

		top = newtop;
		bottom = 0;
		return nul;
	}
};

class workstealingthread{

public:
	vector<BDEqueue> que;
	int numthreads;
	// int matSize;
	
	workstealingthread(vector<BDEqueue> myqueue, int nthreads, int n){
		numthreads = nthreads;
		for (int j=0; j<nthreads; j++){
			que.push_back(myqueue[j]);
		}
	}
	
	void run(int id){
		int me = id;
		my_id task = que[me].pop_bottom();
		
		while(task.a[0] != -1){
			testMat->compute_row(task.a[0]);
			task = que[me].pop_bottom();
		}
			
		for (int ind = 0; ind < numthreads; ind++){
			int victim = ind;
			while(!que[victim].isempty()){
				task = que[victim].pop_top();
				if(task.a[0] != -1)
					testMat->compute_row(task.a[0]);
			}
		}
	}

};


 int main(){

	int num_threads, n;
	cout<<"threads - size"<<endl;;
	cin>>num_threads>>n;
	Matrix tempMat(n);
	testMat = &tempMat;
	nul.a[0] = -1;

	vector<BDEqueue> queues;
	BDEqueue tempq(n);
	for (int i = 0; i < num_threads; i++){
		queues.push_back(tempq);
	}

	for (int i = 0; i < num_threads; i++){
		my_id temp;
		temp.a[0] = i;
		while(temp.a[0] < n){
			queues[i].pushbottom(temp);
			temp.a[0] += num_threads;
		}	
	}

	workstealingthread service(queues, num_threads, n);
	thread workers[num_threads];
	
	double startTime, exitTime;
    startTime = omp_get_wtime();

	for(int i=0; i<num_threads; i++){
		workers[i] = thread(&workstealingthread::run, service, i);
	} 

	
	for(int i=0; i<num_threads; i++){
		workers[i].join();
	}


	exitTime = omp_get_wtime();
	printf("timetaken: %lf\n", exitTime-startTime);

	// for(auto e : testMat->matA){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<" ";
	// 	}
	// 	cout<<endl;
	// }
	// for(auto e : testMat->matB){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<" ";
	// 	}
	// 	cout<<endl;
	// }
	// for(auto e : testMat->matC){
	// 	for(auto e_sub : e){
	// 		cout<<e_sub<<" ";
	// 	}
	// 	cout<<endl;
	// }

	return 0;
}