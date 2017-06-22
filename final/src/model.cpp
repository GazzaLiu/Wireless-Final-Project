#include <iostream>
#include <cstring>
#include <math.h>
#include "model.h"
#include <fstream>
#include<algorithm>
default_random_engine generator;

normal_distribution<double> distribution(0.0,36.0);

using namespace std;


DATA 
model::two_ray_ground(vector<double>& data){
	double signal=0.0;
	double SINR=0.0;
	double bit_rate=0.0;
	double tmp;
	for(unsigned i=0;i<data.size();++i){
	  if(i==0) tmp=inter(data[i]);
	  else signal=signal+inter(data[i]);
	  //cout<<inter(data[i])<<endl;
	}
	signal=signal+k*tem*band;
	
	SINR=10*log10(tmp/(signal));
	bit_rate=band/users*log2(1+tmp/signal);	
	signal=10*log10(tmp);
	DATA result(signal,SINR,bit_rate);
	return result;
};

	
DATA 
model::shadowing(vector<double>& data){
	/**vector<double> signal;
	vector<double> SINR;
	for (unsigned i=0;i<data.size();++i){
	  double number=distribution(generator);
	  signal.push_back(inter(data[i])[0]+number);
	  cout<<number<<endl;
	  SINR.push_back(inter(data[i])[1]+number);
	  				     }
	DATA result(signal,SINR);
        return result;**/
};
double 
model::inter(int distance){
		vector<double> fin;
		double D=sqrt(pow((B_H-U_H),2)+pow(distance,2));
		double y= (U_H)*distance/(B_H+U_H);
		double x=distance-y;
		double d=sqrt(pow(x,2)+pow(B_H,2))+sqrt(pow(y,2)+pow(U_H,2));
		double phi=2*3.14*(d-D)/w_l;
		double result=pow(w_l/(4*3.14),2)*(pow((1/D-1/d*cos(-phi)),2)+pow((1/d*sin(-phi)),2));
		//float phi=4*3.14*B_H*U_H/(w_l*distance);
		//double result=pow(w_l/(4*3.14),2)*pow(phi,2);
		//cout<<result<<endl;
		//double SINR=10*log10(result)-10*log10(k*tem*band);
		//cout<<10*log10(k*tem*band)<<endl;
		
		result=(result)*pow(10,power/10)*pow(10,2*gain/10);
		//fin.push_back(result);
		//fin.push_back(SINR);
		return result;
};
vector<bool>
random_access::collision(vector<double> time,vector<double> rate){
	vector<bool> col(mn.size(),1);
	for(unsigned i=0;i<time.size();++i){
	   for(unsigned j=i+1;j<time.size();++j){
		double t=time[i]+bits/rate[i];
		if(t>=time[j]){
		   col[j]=0;
		   col[i]=0;
		   j++;
		}
		else
		  break;
	   }
	}
	return col;
}
vector<double>
aloha::random_time(){
	double lower_bound=0;
	double upper_bound=total_time;
	uniform_real_distribution<double> unif(lower_bound,upper_bound);
	default_random_engine re;
	vector<double> random;
	for(unsigned i=0;i<mn.size();++i){
	   double ran=unif(re);
	   random.push_back(ran);
	}
	sort(random.begin(),random.end());
	return random;
};

// --
vector<double> one_CSMA::random_time(){
	double lower_bound=0;
	double upper_bound=total_time;
	uniform_real_distribution<double> unif(lower_bound,upper_bound);
	default_random_engine re;
	vector<double> random;
	for(unsigned i=0;i<mn.size();++i){
	   double ran=unif(re);
	   random.push_back(ran);
	}
	sort(random.begin(),random.end());
	return random;
};
// --

void
random_i::generate(int n){
        for(unsigned i=0;i<n;i++){
                double a=rand()%250;
                double b=rand()%360;
                R.push_back(a);
                theta.push_back(b);
                        }

};
void
random_i::print(fstream& a){
        for(unsigned i=0;i<R.size();i++)
                a<<R[i]*cos(theta[i]/180*M_PI)<<" "<<R[i]*sin(theta[i]/180*M_PI)<<endl;
};
const vector<double>
random_i::dis(){
        return R;
};

vector<vector<double>>
random_i::distance(const vector<double>& a , const vector<double>& b){
	vector<vector<double>> c;
        for(unsigned i=0;i<R.size();++i){
                vector<double> g;
                for(unsigned j=0;j<a.size();++j){
                        int distance=sqrt(a[j]*a[j]+R[i]*R[i]-
                                     cos((b[j]-theta[i])/180*M_PI)*a[j]*R[i]*2);
                        //cout<<distance<<endl;
                        g.push_back(distance);}

                c.push_back(g);g.clear();}
        return c;
};

vector<vector<double>>
random_i::BS_dis(){
        vector<vector<double>> c;
        vector<double> Rtmp=R;
        for(unsigned i=0;i<R.size();i++){
                double tmp=Rtmp[0];
                Rtmp[0]=Rtmp[i];
                Rtmp[i]=tmp;
                c.push_back(Rtmp);
        }
	return c;
};


		
