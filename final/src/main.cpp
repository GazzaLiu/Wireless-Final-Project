#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"
#include <iomanip>

using namespace std;

int main()
{
	cout << "Welcome XD" << endl;
	srand(time(NULL));
	model Model;
	int num = 50;
	random_i r;
	r.generate(num);
	vector<vector<double>> D = r.BS_dis();
	vector<double> bit_rate;
	vector<double> SINR;
	for (unsigned i = 0; i < D.size(); ++i) {
		bit_rate.push_back((Model.two_ray_ground(D[i])).bit_rate);
		SINR.push_back((Model.two_ray_ground(D[i])).SINR);
	}
	bool print = 0;
	bool no = 0;
	bool have = 0;
	bool capture = 1;
	int repeat = 50;


	/*
	aloha hello;
	hello.add_mobiles(r.dis());
	hello.total_time = 100;
	vector<double> time_stat = hello.random_time();

	for (unsigned bits = 100; bits < 30 * 100; bits = bits + 1) {
	hello.bits = bits;
	vector<bool> transmit = hello.collision(time_stat, bit_rate);
	double data = 0;
	for (unsigned i = 0; i < transmit.size(); ++i)
	if (transmit[i])
	data = data + bits;
	double size = transmit.size();

	}
	*/

	vector<int> result_number_no;
	vector<int> result_data_no;



	non_CSMA CSMA;
	CSMA.add_mobiles(r.dis());
	CSMA.total_time = 100;
	CSMA.total_duration = 100;
	CSMA.print = print;

	if (no) {
		cout << "no duration" << endl;
		for (CSMA.bits = 100; CSMA.bits <= 15000; CSMA.bits = CSMA.bits + 100) {
			//for (CSMA.total_duration = 100; CSMA.total_duration <= 10000; CSMA.total_duration = CSMA.total_duration + 100) {
			//	CSMA.bits = 1000;
			vector<double> time_stat_CSMA = CSMA.random_time();
			int data_transmitted = CSMA.collision_no_duration(time_stat_CSMA, bit_rate);
			//cout << "collision_no_duration final data_transmitted:" << data_transmitted << endl;
			result_number_no.push_back(data_transmitted);
			result_data_no.push_back(data_transmitted*CSMA.bits);
			//system("pause");
		}
		cout << endl << "rate:" << endl;
		for (int i = 0; i < result_number_no.size(); i++)cout << result_number_no[i] << " ";
		cout << endl;

		cout << endl << "data:" << endl;
		for (int i = 0; i < result_data_no.size(); i++)cout << result_data_no[i] << " ";
		cout << endl;
	}
	if (have) {
		vector<double> result_number_have_ave;
		for (CSMA.bits = 100; CSMA.bits <= 15000; CSMA.bits = CSMA.bits + 100)
		{
			result_number_have_ave.push_back(0);
		}
		cout << "have duration" << endl;
		for (int k = 0; k < repeat; k++)
		{
			vector<int> result_number_have;
			vector<int> result_data_have;
			for (CSMA.bits = 100; CSMA.bits <= 15000; CSMA.bits = CSMA.bits + 100) {
				//	unsigned bits = 100;
				vector<double> time_stat_CSMA = CSMA.random_time();
				int data_transmitted = CSMA.collision_have_duration(time_stat_CSMA, bit_rate);
				//cout << "collision_have_duration final data_transmitted:" << data_transmitted << endl;
				result_number_have.push_back(data_transmitted);
				result_data_have.push_back(data_transmitted*CSMA.bits);
			}
			/*			cout << endl << "rate:" << endl;
			for (int i = 0; i < result_number_have.size(); i++)cout << result_number_have[i] << " ";
			cout << endl;

			cout << endl << "data:" << endl;
			for (int i = 0; i < result_data_have.size(); i++)cout << result_data_have[i] << " ";
			cout << endl;
			*/
			for (int x = 0; x < result_number_have_ave.size(); x++)
			{
				result_number_have_ave[x] = result_number_have_ave[x] + (result_number_have[x] / 50.0);
			}
			/*
			cout << endl << "rate_ave:" << endl;
			for (int i = 0; i < result_number_have_ave.size(); i++)cout << result_number_have_ave[i] << " ";
			cout << endl;
			*/
		}

		cout << endl << "rate_ave:" << endl;
		for (int i = 0; i < result_number_have_ave.size(); i++)cout << result_number_have_ave[i] << " ";
		cout << endl;
	}
	if (capture) {

		vector<double> result_number_capture_ave;
		for (CSMA.bits = 100; CSMA.bits <= 15000; CSMA.bits = CSMA.bits + 100)
		{
			result_number_capture_ave.push_back(0);
		}

		cout << "capture" << endl;
		for (int k = 0; k < repeat; k++)
		{
			vector<int> result_number_capture;
			vector<int> result_data_capture;
			for (CSMA.bits = 100; CSMA.bits <= 15000; CSMA.bits = CSMA.bits + 100) {
				//for (CSMA.total_duration = 100; CSMA.total_duration <= 550; CSMA.total_duration = CSMA.total_duration + 1) {
				//for (CSMA.backoff_max = 2; CSMA.backoff_max <= 400; CSMA.backoff_max = CSMA.backoff_max + 5) {
				//for (CSMA.capture_threshold = 10; CSMA.capture_threshold <= 11; CSMA.capture_threshold = CSMA.capture_threshold + 2) {
				//	CSMA.bits = 1000;
				vector<double> time_stat_CSMA = CSMA.random_time();
				int data_transmitted = CSMA.collision_capture(time_stat_CSMA, bit_rate, SINR);
				//cout << "collision_capture final data_transmitted:" << data_transmitted << endl;
				result_number_capture.push_back(data_transmitted);
				result_data_capture.push_back(data_transmitted*CSMA.bits);
				//system("pause");
			}
			cout << endl << "rate:" << endl;
			for (int i = 0; i < result_number_capture.size(); i++)cout << result_number_capture[i] << " ";
			cout << endl;
			/*
			cout << endl << "data:" << endl;
			for (int i = 0; i < result_data_capture.size(); i++)cout << result_data_capture[i] << " ";
			cout << endl;
			*/
			for (int x = 0; x<result_number_capture_ave.size(); x++)
			{
				result_number_capture_ave[x] = result_number_capture_ave[x] + (result_number_capture[x] / 50.0);
			}

			cout << endl << "rate_ave:" << endl;
			for (int i = 0; i < result_number_capture_ave.size(); i++)cout << result_number_capture_ave[i] << " ";
			cout << endl;
		}
		cout << endl << "rate_ave:" << endl;
		for (int i = 0; i < result_number_capture_ave.size(); i++)cout << result_number_capture_ave[i] << " ";
		cout << endl;
	}
	system("pause");
	return 0;
}