#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"

/*
void help_message(){
	cout<<"usage:./bin/main -[downlink|uplink] <input_file> <output_file>"<<endl;
}
*/

cout << "Welcome XD";

/*int main (int argc , char* argv[])
{
*/
int main() {

	/*	char buffer[200];
		fstream file_out(argv[2],ios::out);
		model Model;
		*/
	int num = 50;
	random_i r;
	r.generate(num);
	//	if(!strcmp(argv[1],"-aloha")){
	vector<vector<double>> D = r.BS_dis();
	vector<double> bit_rate;
	vector<double> SINR;
	for (unsigned i = 0; i < D.size(); ++i) {
		bit_rate.push_back((Model.two_ray_ground(D[i])).bit_rate);
		SINR.push_back((Model.two_ray_ground(D[i])).SINR);
	}
	aloha hello;
	hello.add_mobiles(r.dis());
	hello.total_time = 100;
	vector<double> time_stat = hello.random_time();
	//for(unsigned i=0;i<time_stat.size();++i)
	//cout<<time_stat[i]<<endl;
	//cout<<bit_rate[i]<<endl;
	//cout<<r.dis()[i]<<endl;
	//cout<<SINR[i]<<endl;
	for (unsigned bits = 100; bits < 30 * 100; bits = bits + 1) {
		hello.bits = bits;
		vector<bool> transmit = hello.collision(time_stat, bit_rate);
		double data = 0;
		for (unsigned i = 0; i < transmit.size(); ++i)
			if (transmit[i])
				data = data + bits;
		double size = transmit.size();

		//file_out << bits << "  " << data << endl;

		//for(unsigned i=0;i<transmit.size();++i)
		//	cout<<transmit[i]<<"/";
		//cout<<endl;
//	    }
	}

	/*	else if(!strcmp(argv[1],"-uplink")){
		}
		else{
			help_message();
			return 0;
		}
		return 1;
		*/
	return 0;
}
