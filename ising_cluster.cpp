#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <assert.h>
#include <fstream>
#include <math.h>
#include <vector>
#include <random>
#include <functional>
#define random(x)(rand()%x)
#define random_1()(double(random(10000))/10000)
#define SIZE 60
#define CHAIN_NUM 5000000 
#define IGNORE_FACTOR 0.2
#define h -0
#define J -0.5
using namespace std;
typedef int (*PMF)(int);
mt19937::result_type seed=time(NULL); 
auto dice_rand=std::bind(std::uniform_real_distribution<double>(0,1),mt19937(seed));

void my_print(bool *a){
	for(int i=0;i<SIZE*SIZE;++i){
		if(i%SIZE==0)
			cout<<endl;
		cout<<a[i]<<" ";
	}
	cout<<endl;
}

void my_random(bool *a,int range=2){
	for(int i=0;i<SIZE*SIZE;++i){
		a[i]=bool(random(range));
	}
}

int up_neighbour(int i){
	if(i<SIZE)
		return i+(SIZE-1)*SIZE;
	else
		return i-SIZE;
}

int down_neighbour(int i){
	if(i>=SIZE*(SIZE-1)) 
		return i-SIZE*(SIZE-1);
	else 
		return i+SIZE;
}


int left_neighbour(int i){
	if((i+1)%SIZE==1) 
		return i+(SIZE-1);
	else 
		return i-1;
} 

int right_neighbour(int i){
	if((i+1)%SIZE==0) 
		return i-(SIZE-1);
	else 
		return i+1;
}

PMF pmf[4]={up_neighbour,down_neighbour,right_neighbour,left_neighbour};

int near_condition(bool *a,int num){
	int con_tot=0;
	for(int i=0;i<4;++i){
		con_tot+=a[pmf[i](num)]*2;
	}
	con_tot-=4;
	return con_tot;
}

double Energy_i(bool *a,int num){
	double Energy=0;
	bool condition_now=a[num];
	if(condition_now==1)
		Energy+=h;
	else
		Energy-=h;
	Energy+=near_condition(a,num)*(condition_now*2-1)*J;
	return Energy;
}

double Energy(bool *a){
	double Energy_tot=0;
	for(int i=0;i<SIZE*SIZE;++i)
		Energy_tot+=Energy_i(a,i);
	return Energy_tot;
}

int count_m(bool *a){
	int m_tot=0;
	for(int i=0;i<SIZE*SIZE;++i){
		m_tot=m_tot+a[i]*2-1;
	}
	return m_tot;
}


void cluster(bool *a,int pivot,bool prime,vector<int>* cluster,double bia){
	for(int i=0;i<4;++i){
		int loc=pmf[i](pivot);
		double prob=dice_rand();
		if(a[loc]==prime&&prob<bia){
			(*cluster).push_back(loc);
			a[loc]=!a[loc];
		}
	}
}


void cluster_all(bool *a,double T){
	int pivot=random(3600);
	double bia=1-exp(4*J/T);
	vector<int> cluster_e;
	bool prime=a[pivot];
	cluster_e.push_back(pivot);
	a[pivot]=!a[pivot];
	int begin=0;
	while(true){
		cluster(a,pivot,prime,&cluster_e,bia);
		if(begin==cluster_e.size()-1)
			break;
		else{
			begin+=1;
			pivot=cluster_e[begin];
		}
	}
}



int main(){
	srand(double(time(0)));
	bool a[3600];
	my_random(a);
	double e=0;
	double m=0;
	ofstream ofile;
	ofstream ofile1;
	ofile1.open("result_m_1.txt");
	ofile.open("result_ene_1.txt");
	int count=0;
	for(double k=0;k<50;++k){
		double T=2.2+k*0.008;
		cout<<T<<endl; 
		int chain_num=100*pow(10,T-1);  
		for(int i=0;i<chain_num;++i){
			if(i>=0.6*chain_num){
				double m_tot=count_m(a);
				if(m_tot>=0)
					ofile1<<count_m(a)<<" ";
				ofile<<Energy(a)<<" ";
			}
			cluster_all(a,T);
		}
		ofile<<endl;
		ofile1<<endl;
	}
	/*for(int i=0;i<100000;++i){
		if(i>=0.4*100000){
			e+=Energy(a);
		}
		cluster_all(a,3);
	}
	cout<<e/60000<<endl;*/
	return 0;
}