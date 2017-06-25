#ifndef MODEL_H
#define MODEL_H
#include<vector>
#include<random>
#include<math.h>
#include<stdlib.h>
using namespace std;
//default_random_engine generator;

//normal_distribution<double> distribution(0.0,36.0);
struct DATA{
	public:
	DATA(){}
	//~DATA();
	DATA(double a,double b,double c):signal(a),SINR(b),bit_rate(c){}
	double SINR;
	double signal;
	double bit_rate;
	DATA operator =(const DATA& i){this->SINR=i.SINR;
				       this->signal=i.signal;
				       this->bit_rate=i.bit_rate;
				       return *this;}
	 
};

class model{
	public:
	  model(){}
	  DATA two_ray_ground(vector<double>& data);
	  DATA shadowing(vector<double>& data);
	  void mode(int i);
	  double inter(int distance);
	  double factor=k*tem*band;
	private:
	  double mobile=-7;
	  double B_H=51.5;
          double U_H=1.5;
          double tem=300.15;
          double band=pow(10,7);
	  double users=50;
          double k=1.37*pow(10,-23);
          double power=-7;
          double gain=14;
          double f=5*pow(10,6);
          double c=3*pow(10,8);
          double w_l=100;
	  //double inter(int distance);
};
class mobile{
	public:
	  double x=250;
	  double y=0;
	  double time=0;
	  double v=0;
	  double theta=0;
};



class random_access{
	public:
	vector<bool> collision(vector<double> time,vector<double> rate);
	void add_mobiles(vector<double> a){mn=a;}
	vector<bool> time_solt;
	int bits=10;	
	protected:
	float time_interval;
	//float gruad_time;
	double speed=3*pow(10,8);
	vector<double> mn;
	vector<bool> transmitting;
};

class aloha : public random_access{
	//now if all mns want to transmit 10kb data at a random time
	public:
	//int bts=10*1000;
	int total_time=0;
	vector<double> random_time();
	 
};


class solt_aloha : public random_access{
};


// -----
class one_CSMA : public random_access{
	public:
		bool isBusy;
		bool isCollision;
		int bitsInterval;
		int captureThreshold;
		int count;
		int endBits;	
		int sending;
		int startBits;
		double data;
		double p;
		double succeed;
		double time_interval = 0.01;
		double total_time;
		int MSstatus[50]; // 0:idle 1:waiting(listening) 2:prepared 3:sending(succeed) 4:sending(failed) 5:finished(succeed) 6:finished(failed)
		double idleTime[50];
		vector<double> random_time();
};
// -----

class non_CSMA : public random_access{
};


class random_i{
        public:
          random_i(){}
          void generate(int n);
          void print(fstream& a);
          vector<vector<double>> distance(const vector<double>& a,const vector<double>& b);
          vector<vector<double>> BS_dis();
          const vector<double> dis();
        private:
         vector<double> R;
         vector<double> theta;
};




#endif
