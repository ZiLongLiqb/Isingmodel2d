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
#define size 60
#define h 1
#define J 1
using namespace std;

void my_print(bool *a){
	for(int i=0;i<size*size;++i){
		if(i%size==0)
			cout<<endl;
		cout<<a[i]<<" ";
	}
	cout<<endl;
}

int up_neighbour(int i){
	if(i<size)
		return i+(size-1)*size;
	else
		return i-size;
}

int down_neighbour(int i){
	if(i>=size*(size-1)) 
		return i-size*(size-1);
	else 
		return i+size;
}


int left_neighbour(int i){
	if((i+1)%size==1) 
		return i+(size-1);
	else 
		return i-1;
} 

int right_neighbour(int i){
	if((i+1)%size==0) 
		return i-(size-1);
	else 
		return i+1;
}


int near_condition(bool *a,int num){
	int con_tot=0;
	int row_next=up_neighbour(num);
	int row_before=down_neighbour(num);
	int column_next=left_neighbour(num);
	int column_before=right_neighbour(num);
	//cout<<row_next<<" "<<row_before<<" "<<column_next<<" "<<column_before<<endl;
	con_tot=(a[row_next]+a[row_before]+a[column_before]+a[column_next])*2-4;
	//cout<<con_tot<<endl;
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

bool accept(double delta_E,double rnd,double T){
	if(delta_E<=0)
		return true;
	double thres=exp(-delta_E/T);
	if(rnd<thres)
	{
		return true;
	}
	return false;
}


vector<double> flip(bool *a,double rnd,vector<double> before_con,double T){
	int range=size*size;
	int flip_num=random(range);
	int near_con=near_condition(a,flip_num);
	bool now_con=a[flip_num];
	double delta_energy=2*(near_con*((!now_con)*2-1)-near_con*(now_con*2-1))*J+((!now_con)-now_con)*2*h;
	double delta_m=(!now_con-now_con)*2;
	if(accept(delta_energy,rnd,T)==true){
		a[flip_num]=!a[flip_num];
		return {before_con[0]+delta_energy,before_con[1]+delta_m,before_con[2]+1};
	}
	else
		return before_con;
}



int main(){
	mt19937::result_type seed=time(NULL);
	auto dice_rand=std::bind(std::uniform_real_distribution<double>(0,1),mt19937(seed));
	srand(double(time(0)));
	bool a[size*size];
	for(int i=0;i<=size*size;++i){
		a[i]=0;
	}
	vector<double> con_begin={size*size*(4*J-h),-size*size,0};
	vector<vector<double>> result;
	vector<double> E;
	vector<double> H;
	vector<double> accept_time;
	for(double T=10;T>0;T=T-0.05)
	{
		double e=0;
		double hh=0;
		for(int i=0;i<1000000;++i){
			//result.push_back(con_begin);
			e=e+con_begin[0];
			hh=hh+con_begin[1];
			double rnd=dice_rand();
			con_begin=flip(a,rnd,con_begin,T);
			//my_print(a);
			//cout<<con_begin[0];
		}
		E.push_back(e/1000000);
		H.push_back(hh/(360*1000000));
		accept_time.push_back(con_begin[2]);
		con_begin[2]=0;
	}
	ofstream ofile;
	ofile.open("result.txt");
	for(int i=0;i<200;++i){
		ofile<<E[i]<<" "<<H[i]<<" "<<accept_time[i]<<endl;
	}
	ofile.close();
}