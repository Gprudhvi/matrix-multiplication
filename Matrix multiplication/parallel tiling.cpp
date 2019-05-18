#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <random>
#include <chrono>
#include <bits/stdc++.h>
#include <semaphore.h>
#include <random>
#include <time.h>
#include <atomic>
#include <omp.h>
using namespace std;

int n,m,z,d;
int **a,**b,**c;
void multiply(int id, int num)
{
	int l=id;
	while(l<m)
	{
		for(int jj=0;jj<z;jj= jj+d)
		{
			for(int kk=0;kk<n;kk=kk+d)
			{

				for(int i=jj;i< min(jj+d,z);i=i+1)
				{
					int r=0;
					for(int k=kk;k<min(n,kk+d);k=k+1)
					{
						r=r+a[l][k]*b[k][i];
					}
					c[l][i] = c[l][i] + r;
				}
			
			}
		}
	l=l+num;
	}

}



int main()
{
	
	cout<< "enter matrix size(mxn,nxz)"<<endl;
	cin>>m;
	n =m;
	z=m;
	a = new int*[m];
	for(int i = 0; i < m; ++i)
    	a[i] = new int[n];
	// cout<<"1"<<endl;
	b = new int*[n];
	for(int i = 0; i < n; ++i)
    	b[i] = new int[z];
    // cout<<"2"<<endl;
	c = new int*[m];
	for(int i = 0; i < m; ++i)
    c[i] = new int[z];
	// cout<<"3"<<endl;
	srand(time(0));
	for (int i = 0; i < m; i++) { 
        for (int j = 0; j < n; j++) { 
            a[i][j] = rand() % 10; 
        } 
    }
    // cout<<"4"<<endl;
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < z; j++) { 
            b[i][j] = rand() % 10; 
        } 
    }
    // cout<<"5"<<endl;
    cout<< "enter number of threads:";
    int numl;
    cin>>numl;
    cout<<"enter tile size";
    cin>>d;
    thread workers[numl];
     double startTime, exitTime;
     startTime = omp_get_wtime();

    for (int i=0;i<numl;i++)
    {
    	workers[i] = thread(multiply,i,numl);
    }
    // exitTime = omp_get_wtime();
    // printf("timetaken: %lf\n", exitTime-startTime);
    
    for(int i=0;i<numl;i++)
    {
        workers[i].join();
    }
     exitTime = omp_get_wtime();
    printf("timetaken: %lf\n", exitTime-startTime);


    // for (int i=0;i<m;i++)
    // {
    // 	for (int j=0;j<n;j++)
    // 	{
    // 		cout<<a[i][j]<<" ";
    // 	}
    // 	cout<<endl;
    // }
    // cout<<endl;

    // for (int i=0;i<n;i++)
    // {
    // 	for (int j=0;j<z;j++)
    // 	{
    // 		cout<<b[i][j]<<" ";
    // 	}
    // 	cout<<endl;
    // }
    // cout<<endl;
    

    
    // for (int i=0;i<m;i++)
    // {
    // 	for (int j=0;j<z;j++)
    // 	{
    // 		cout<<c[i][j]<<" ";
    // 	}
    // 	cout<<endl;
    // }

    return 0;
}