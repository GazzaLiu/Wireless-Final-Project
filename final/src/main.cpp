#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <utility>
#include <string>
#include "model.h"

void help_message(){
	cout<<"usage:./bin/main -[downlink|uplink] <input_file> <output_file>"<<endl;
}

int main (int argc , char* argv[])
{
	srand(time(NULL)); // set random
	char buffer[200];
	//----------
	double total_time;
	int captureThreshold;
	double p;
	cout<<"Stimulation end time (default : 100) : ";
	cin>>total_time;
	cout<<"Cpature threshold (default : 10) : ";
	cin>>captureThreshold;
	cout<<"P value (default : 1) : ";
	cin>>p;
	for (int k = 1; k <= 20; k++) {
	//----------
	fstream file_out(argv[2]+to_string(k),ios::out);
	model Model;
	int num=50;
	random_i r;
	r.generate(num);
	if(!strcmp(argv[1],"-aloha")){
	    vector<vector<double>> D=r.BS_dis();
            vector<double> bit_rate;
	    vector<double> SINR;

            for(unsigned i=0;i<D.size();++i){
	       bit_rate.push_back((Model.two_ray_ground(D[i])).bit_rate);
	       SINR.push_back((Model.two_ray_ground(D[i])).SINR);
	    }
	    aloha hello;
	    hello.add_mobiles(r.dis());
            hello.total_time=100;
	    vector<double> time_stat=hello.random_time();
	    //for(unsigned i=0;i<time_stat.size();++i)
		//cout<<time_stat[i]<<endl;
		//cout<<bit_rate[i]<<endl;
		//cout<<r.dis()[i]<<endl;
		//cout<<SINR[i]<<endl;
	    for(unsigned bits=100;bits<30*100;bits=bits+1){
	       hello.bits=bits;	
	       vector<bool> transmit=hello.collision(time_stat,bit_rate); 
	       double data=0;
	       for(unsigned i=0;i<transmit.size();++i)
		if(transmit[i])
		   data=data+bits;
	       double size=transmit.size();
	       
	       file_out<<bits<<"  "<<data<<endl;
	    
	    //for(unsigned i=0;i<transmit.size();++i)
	    //	cout<<transmit[i]<<"/";
	    //cout<<endl;
	    }
	}

	// --CSMA(1-persistent)--
	else if (!strcmp(argv[1],"-one_CSMA")) {

		vector<vector<double>> D = r.BS_dis();
		vector<double> bit_rate;
		vector<double> SINR;

		for (int i = 0; i < D.size(); i++) {
	       bit_rate.push_back((Model.two_ray_ground(D[i])).bit_rate);
	       SINR.push_back((Model.two_ray_ground(D[i])).SINR);
		}

		one_CSMA CSMA_model;
		CSMA_model.startBits = 100;
		CSMA_model.endBits = 1000000;
		CSMA_model.bitsInterval = 1000;
		CSMA_model.add_mobiles(r.dis());

	    vector<double> time_stat = CSMA_model.random_time();

		// round time_stat
		int res = round(1.0 / CSMA_model.time_interval);
		for (int i = 0 ; i < num; i++) {
			time_stat[i] *= res;
			time_stat[i] = round(time_stat[i]);
			time_stat[i] /= res;
		}

		srand((unsigned)time(NULL));
		cout<<(double)rand() / RAND_MAX<<endl;
		
		//cout<<"Stimulation end time (default : 100) : ";
		CSMA_model.total_time = total_time;
		//cin>>CSMA_model.total_time;
		//cout<<"Cpature threshold (default : 10) : ";
		CSMA_model.captureThreshold = captureThreshold;
		//cin>>CSMA_model.captureThreshold;
		//cout<<"P value (default : 1) : ";
		CSMA_model.p = p;
		//cin>>CSMA_model.p;

		// log
		//cout<<"send time"<<'\t'<<"bit rate"<<'\t'<<"SINR"<<endl;
		for (int i = 0; i < D.size(); i++) {
			cout<<time_stat[i]<<'\t'<<bit_rate[i]<<'\t'<<SINR[i]<<endl;
		}

		
		for (int bits = CSMA_model.startBits; bits < CSMA_model.endBits; bits += CSMA_model.bitsInterval) {

			// debug log
			//cout<<"bits loop start"<<endl;
			//cout<<CSMA_model.total_time<<" "<<CSMA_model.time_interval<<endl;

			// initializing
			//CSMA_model.isBusy = false;
			CSMA_model.isCollision = false;
			CSMA_model.bits = bits;
			CSMA_model.count = 0;
			CSMA_model.data = 0;
			CSMA_model.idleTime = -1;
			CSMA_model.MScapture = -1;
			CSMA_model.MSsend = -1;
			CSMA_model.sending = -1;

			CSMA_model.succeed = 0;

			for (int j = 0; j < num; j++) {
				CSMA_model.MSstatus[j] = 0;
				CSMA_model.MScollide[j] = false;
				//CSMA_model.MSprepared[j] = false;
				CSMA_model.MSsucceed[j] = false;
				//CSMA_model.MSwaiting[j] = false;
			}

			//cout<<"start stimulation with total time "<<CSMA_model.total_time<<" (sec) and data "<<bits<<" (bits)"<<endl; // log

			//main loop
			for (double t = 0; t <= CSMA_model.total_time; t += CSMA_model.time_interval) {
				CSMA_model.isCollision = false;
				// send data
				for (int j = 0; j < num; j++) {
					if (CSMA_model.MSstatus[j] == 2 && CSMA_model.MSsend == -1) {
						CSMA_model.MSsend = j;
					}
					// collision
					else if (CSMA_model.MSstatus[j] == 2 && CSMA_model.MSsend != -1) {
						CSMA_model.MScollide[CSMA_model.MSsend] = true;
						CSMA_model.MScollide[j] = true;
						CSMA_model.MSstatus[CSMA_model.MSsend] = 3;
						CSMA_model.MSstatus[j] = 3;
						CSMA_model.isCollision = true;
						// break;
					}
				}
				// handle collision
				if (CSMA_model.isCollision) {
					vector<pair<double, int>> capture;
					//cout<<t<<" sec "<<"collision, "; // log
					for (int j = 0; j < num; j++) {
						if (CSMA_model.MScollide[j]) {
							capture.push_back(make_pair(SINR[j], j));
							//cout<<j<<"  ";
							CSMA_model.MScollide[j] = false;
						}
					}
					//cout<<"failed"<<endl; // log
					sort(capture.begin(), capture.end());
					for (int k = 0; k < capture.size(); k++) {
						//cout<<capture[k].first<<" ";
					}
					// capture
					if (capture.size() >= 2 && abs(capture[capture.size() - 1].first - capture[capture.size() - 2].first) >= CSMA_model.captureThreshold) {
						//cout<<capture[capture.size() - 1].second<<"captured others";
						CSMA_model.MSstatus[capture[capture.size() - 1].second] = 3;
						CSMA_model.MSsucceed[capture[capture.size() - 1].second] = true;
						CSMA_model.idleTime = t + bits / bit_rate[capture[capture.size() - 1].second]; // bit rate : KB/sec
						CSMA_model.idleTime *= res;
						CSMA_model.idleTime = round(CSMA_model.idleTime);
						CSMA_model.idleTime /= res;
						//cout<<CSMA_model.idleTime<< " sec "<<endl; // log
						CSMA_model.sending = capture[capture.size() - 1].second;
					}
					CSMA_model.MSsend = -1;
				}
				// no collision
				if (CSMA_model.MSsend != -1) {
					//cout<<t<<" sec "<<"no collision, "<<CSMA_model.MSsend<<" send with idle time "; // log
					CSMA_model.MSstatus[CSMA_model.MSsend] = 3;
					//CSMA_model.MSprepared[CSMA_model.MSsend] = false;
					CSMA_model.MSsucceed[CSMA_model.MSsend] = true;
					CSMA_model.idleTime = t + bits / bit_rate[CSMA_model.MSsend]; // bit rate : KB/sec
					CSMA_model.idleTime *= res;
					CSMA_model.idleTime = round(CSMA_model.idleTime);
					CSMA_model.idleTime /= res;
					//cout<<CSMA_model.idleTime<< " sec "<<endl; // log
					CSMA_model.sending = CSMA_model.MSsend;
					CSMA_model.MSsend = -1;
				}
				// set busy time (with double epsilon)
				if (abs(t - CSMA_model.idleTime) < CSMA_model.time_interval / 2) {
					//CSMA_model.isBusy = false;
					CSMA_model.sending = -1;
					CSMA_model.idleTime = -1;
					//cout<<"channel clear"; // debug log
				}
				// time to send (with double epsilon)
				if (abs(t - time_stat[CSMA_model.count]) < CSMA_model.time_interval / 2) {
					//cout<<t<<" sec "<<CSMA_model.count<<" time to send"<<endl; // log
					if (CSMA_model.idleTime == -1) {
						CSMA_model.MSstatus[CSMA_model.count] = 2;
						//CSMA_model.MSprepared[CSMA_model.count] = true;
					}
					else {
						CSMA_model.MSstatus[CSMA_model.count] = 1;
						//CSMA_model.MSwaiting[CSMA_model.count] = true;
					}
					CSMA_model.count++;
				}
				// one-persistent
				for (int j = 0; j < num; j++) {
					if ((double)rand() / RAND_MAX <= CSMA_model.p && CSMA_model.MSstatus[j] == 1 && CSMA_model.idleTime == -1) {
						CSMA_model.MSstatus[j] = 2;
						//CSMA_model.MSwaiting[j] = false;
						//CSMA_model.MSprepared[j] = true;
					}
				}
				if (CSMA_model.sending != -1) {
					CSMA_model.data = CSMA_model.data + CSMA_model.time_interval * bit_rate[CSMA_model.sending];
				}
			}
			for (int i = 0; i < D.size(); i++) {
				if (CSMA_model.MSsucceed[i]) {
					//CSMA_model.data += bits;
					CSMA_model.succeed++;
				}
				//cout<<CSMA_model.MSsucceed[i];

			}

			
			//cout<<bits<<"  "<<CSMA_model.data<<"  "<<CSMA_model.succeed / D.size()<<endl; // log
			file_out<<bits<<"  "<<CSMA_model.data<<"  "<<CSMA_model.succeed / D.size()<<endl;

		}

	}
	// --end--

	else if(!strcmp(argv[1],"-uplink")){
	}		
	else{
		help_message();
		return 0;
	}
}
	return 1;
}
