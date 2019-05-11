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
#define h 0
#define J -0.5
using namespace std;
mt19937::result_type seed=time(NULL); 
auto dice_rand=std::bind(std::uniform_real_distribution<double>(0,1),mt19937(seed));

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


int near_condition(bool *a,int num){
	int con_tot=0;
	int row_next=up_neighbour(num);
	int row_before=down_neighbour(num);
	int column_next=left_neighbour(num);
	int column_before=right_neighbour(num);
	con_tot=(a[row_next]+a[row_before]+a[column_before]+a[column_next])*2-4;
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


bool accept(double delta_E,double T){
	if(delta_E<=0)
		return true;
	double thres=exp(-delta_E/T);
	double rnd=dice_rand();
	if(rnd<thres)
	{
		return true;
	}
	return false;
}


vector<double> flip(bool *a,vector<double> before_con,double T){
	int range=SIZE*SIZE;
	int flip_num=random(range);
	int near_con=near_condition(a,flip_num);
	bool now_con=a[flip_num];
	double delta_energy=2*(near_con*((!now_con)*2-1)-near_con*(now_con*2-1))*J+((!now_con)-now_con)*2*h;
	if(accept(delta_energy,T)==true){
		a[flip_num]=!a[flip_num];
		double delta_m=(!now_con-now_con)*2;
		return {before_con[0]+delta_energy,before_con[1]+delta_m,before_con[2]+1};
	}
	else 
		return before_con;
}

vector<vector<double>> result_T(bool *a,vector<double> con_begin,double T){
	double e=0;
	double hh=0;
	int accept=0;
	vector<vector<double>> result;
	result.push_back(con_begin);
	for(int i=0;i<CHAIN_NUM;++i){
		con_begin=flip(a,con_begin,T);
		result.push_back(con_begin);
	}
	return result;
}

vector<double> data_handle_1(vector<vector<double>> result,double delta_t){
	double e_average_0=0;
	double e_average_1=0;
	double e_squ_0=0;
	double e_squ_1=0;
	double h_average_0=0;
	double h_average_1=0;
	double h_squ_0=0;
	double h_squ_1=0;
	double e_corre=0;
	double h_corre=0;
	double e_error=0;
	double h_error=0;
	double data_num=CHAIN_NUM-delta_t-CHAIN_NUM*IGNORE_FACTOR;
	for(int i=CHAIN_NUM*IGNORE_FACTOR;i<CHAIN_NUM-delta_t;++i){
		e_average_0+=result[i][0]/data_num;
		e_average_1+=result[i+delta_t][0]/data_num;
		h_average_0+=result[i][1]/data_num;
		h_average_1+=result[i+delta_t][1]/data_num;
	}
	for(int i=CHAIN_NUM*IGNORE_FACTOR;i<result.size()-delta_t;++i){
		e_corre+=(result[i][0]-e_average_0)*(result[i+delta_t][0]-e_average_1)/data_num;
		h_corre+=(result[i][1]-h_average_0)*(result[i+delta_t][1]-h_average_1)/data_num;
		e_squ_0+=(result[i][0]-e_average_0)*(result[i][0]-e_average_0)/data_num;
		h_squ_0+=(result[i][1]-h_average_0)*(result[i][1]-h_average_0)/data_num;
		e_squ_1+=(result[i+delta_t][0]-e_average_1)*(result[i+delta_t][0]-e_average_1)/data_num;
		h_squ_1+=(result[i+delta_t][1]-h_average_1)*(result[i+delta_t][1]-h_average_1)/data_num;
	}
	e_error=e_corre/(sqrt(e_squ_0)*sqrt(e_squ_1));
	h_error=h_corre/(sqrt(h_squ_0)*sqrt(h_squ_1));
	return {e_average_0,h_average_0,e_error,h_error,e_squ_0,h_squ_0};
}


int main(){
	srand(double(time(0)));
	bool a[SIZE*SIZE];
	for(int i=0;i<=SIZE*SIZE;++i){
		a[i]=0;
	}
	cout<<"------------begin-------------------"<<endl;
	vector<double> con_begin={SIZE*SIZE*(4*J-h),-SIZE*SIZE,0};
	vector<vector<double>> data_result;
	double temperature=3.5;
	//vector<vector<double>> result=result_T(a,con_begin,temperature);
	//vector<double> err=data_handle_1(result,1000);
	//cout<<err[0]/3600<<endl;
	//cout<<sqrt(err[4]/(CHAIN_NUM/(-2000/log(err[2]))))/<<endl;
	for(int i=0;i<=80;++i)
	{
		vector<vector<double>> result=result_T(a,con_begin,temperature);
		con_begin=result.back(); 
		vector<double> err=data_handle_1(result,1000);
		//cout<<err[0]/3600<<endl;
		//cout<<err[4]*1/(CHAIN_NUM/(1-2000/log(abs(err[2]))))/3600<<endl;
		data_result.push_back(err);
		if(i>39)
		{
			cout<<temperature<<endl;
			temperature+=0.05;
		}
		else
		{
			cout<<temperature<<endl; 
			temperature-=0.05;
		}
	}

	ofstream ofile;
	ofile.open("result2.txt"); 
	for(int i=0;i<data_result.size();++i){
		for(int j=0;j<data_result[i].size();++j)
			ofile<<data_result[i][j]<<" ";
	}
	ofile.close(); 
	return 0;
}  