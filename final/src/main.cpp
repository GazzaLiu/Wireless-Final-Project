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

	model Model;
	int num = 100;
	random_i r;
	r.generate(num);
	vector<vector<double>> D = r.BS_dis();
	vector<double> bit_rate;
	vector<double> SINR;
	for (unsigned i = 0; i < D.size(); ++i) {
		bit_rate.push_back((Model.two_ray_ground(D[i])).bit_rate);
		SINR.push_back((Model.two_ray_ground(D[i])).SINR);
	}

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
	vector<int> result_number_have;
	vector<int> result_data_have;
	non_CSMA CSMA;
	CSMA.add_mobiles(r.dis());
	CSMA.total_time = 100;
	CSMA.total_duration = 500;
	vector<double> time_stat_CSMA = CSMA.random_time();



	for (CSMA.bits = 100; CSMA.bits < 10000; CSMA.bits = CSMA.bits + 100) {
		//	unsigned bits = 100;
		int data_transmitted = CSMA.collision_no_duration(time_stat_CSMA, bit_rate);
		cout << "collision_no_duration final data_transmitted:" << data_transmitted << endl;
		result_number_no.push_back(data_transmitted);
		result_data_no.push_back(data_transmitted*CSMA.bits);
	}

	for (CSMA.bits = 100; CSMA.bits < 10000; CSMA.bits = CSMA.bits + 100) {
		//	unsigned bits = 100;
		int data_transmitted = CSMA.collision_have_duration(time_stat_CSMA, bit_rate);
		cout << "collision_have_duration final data_transmitted:" << data_transmitted << endl;
		result_number_have.push_back(data_transmitted);
		result_data_have.push_back(data_transmitted*CSMA.bits);
	}

	cout << endl << "result_number_no:" << endl;
	for (int i = 0; i<result_number_no.size(); i++)cout << result_number_no[i] << " ";
	cout << endl;

	cout << endl << "result_data_no:" << endl;
	for (int i = 0; i<result_data_no.size(); i++)cout << result_data_no[i] << " ";
	cout << endl;

	cout << endl << "result_number_have:" << endl;
	for (int i = 0; i<result_number_have.size(); i++)cout << result_number_have[i] << " ";
	cout << endl;

	cout << endl << "result_data_have:" << endl;
	for (int i = 0; i<result_data_have.size(); i++)cout << result_data_have[i] << " ";
	cout << endl;


	system("pause");
	return 0;
}