#include <iostream>
#include <cstring>
#include <math.h>
#include "model.h"
#include <fstream>
#include<algorithm>
#include<ctime>
default_random_engine generator;

normal_distribution<double> distribution(0.0, 36.0);

using namespace std;


DATA
model::two_ray_ground(vector<double>& data) {
	double signal = 0.0;
	double SINR = 0.0;
	double bit_rate = 0.0;
	double tmp;
	for (unsigned i = 0; i < data.size(); ++i) {
		if (i == 0) tmp = inter(data[i]);
		else signal = signal + inter(data[i]);
		//cout<<inter(data[i])<<endl;
	}
	signal = signal + k*tem*band;

	SINR = 10 * log10(tmp / (signal));
	bit_rate = band / users*log2(1 + tmp / signal);
	signal = 10 * log10(tmp);
	DATA result(signal, SINR, bit_rate);
	return result;
};


//DATA
//model::shadowing(vector<double>& data) {
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
//};
double
model::inter(int distance) {
	vector<double> fin;
	double D = sqrt(pow((B_H - U_H), 2) + pow(distance, 2));
	double y = (U_H)*distance / (B_H + U_H);
	double x = distance - y;
	double d = sqrt(pow(x, 2) + pow(B_H, 2)) + sqrt(pow(y, 2) + pow(U_H, 2));
	double phi = 2 * 3.14*(d - D) / w_l;
	double result = pow(w_l / (4 * 3.14), 2)*(pow((1 / D - 1 / d*cos(-phi)), 2) + pow((1 / d*sin(-phi)), 2));
	//float phi=4*3.14*B_H*U_H/(w_l*distance);
	//double result=pow(w_l/(4*3.14),2)*pow(phi,2);
	//cout<<result<<endl;
	//double SINR=10*log10(result)-10*log10(k*tem*band);
	//cout<<10*log10(k*tem*band)<<endl;

	result = (result)*pow(10, power / 10)*pow(10, 2 * gain / 10);
	//fin.push_back(result);
	//fin.push_back(SINR);
	return result;
};
/*
vector<bool>
random_access::collision(vector<double> time, vector<double> rate) {
vector<bool> col(mn.size(), 1);
for (unsigned i = 0; i < time.size(); ++i) {
for (unsigned j = i + 1; j < time.size(); ++j) {
double t = time[i] + bits / rate[i];
if (t >= time[j]) {
col[j] = 0;
col[i] = 0;
j++;
}
else
break;
}
}
return col;
}
*/
vector<double>
aloha::random_time() {
	/*double lower_bound = 0;
	double upper_bound = total_time;
	uniform_real_distribution<double> unif(lower_bound, upper_bound);
	default_random_engine re;
	vector<double> random;
	for (unsigned i = 0; i < mn.size(); ++i) {
	double ran = unif(re);
	random.push_back(ran);
	}
	sort(random.begin(), random.end());
	for (int i = 0; i < random.size(); i++)
	{
	random[i] = int(random[i] * 100 + 0.5) / 100.0;
	}
	return random;*/
	vector<double> random;
	for (unsigned i = 0; i < mn.size(); ++i) {
		double ran;
		ran = ((rand() % (100 * total_time)) + 1) / 100.0;
		random.push_back(ran);
	}
	sort(random.begin(), random.end());
	return random;
};



void
random_i::generate(int n) {
	for (unsigned i = 0; i < n; i++) {
		double a = rand() % 250;
		double b = rand() % 360;
		R.push_back(a);
		theta.push_back(b);

	}


};
/*void
random_i::print(fstream& a) {
for (unsigned i = 0; i<R.size(); i++)
a << R[i] * cos(theta[i] / 180 * M_PI) << " " << R[i] * sin(theta[i] / 180 * M_PI) << endl;
};*/
const vector<double>
random_i::dis() {
	return R;
};

/*vector<vector<double>>
random_i::distance(const vector<double>& a, const vector<double>& b) {
vector<vector<double>> c;
for (unsigned i = 0; i<R.size(); ++i) {
vector<double> g;
for (unsigned j = 0; j<a.size(); ++j) {
int distance = sqrt(a[j] * a[j] + R[i] * R[i] -
cos((b[j] - theta[i]) / 180 * M_PI)*a[j] * R[i] * 2);
//cout<<distance<<endl;
g.push_back(distance);
}

c.push_back(g); g.clear();
}
return c;
};*/

vector<vector<double>>
random_i::BS_dis() {
	vector<vector<double>> c;
	vector<double> Rtmp = R;
	for (unsigned i = 0; i < R.size(); i++) {
		double tmp = Rtmp[0];
		Rtmp[0] = Rtmp[i];
		Rtmp[i] = tmp;
		c.push_back(Rtmp);
	}
	return c;
};

int non_CSMA::collision_no_duration(vector<double> mobile_time, vector<double> bit_rate)
{

	vector<double> transmit_duration = calculate_transmit_duration(bits, bit_rate);
	last_busy_end = mobile_time[0] + transmit_duration[0];
	vector<int> backoff;

	//亂數出每個mobile要backoff的亂數
	for (unsigned i = 0; i < mn.size(); ++i) {
		int ran;
		ran = ((rand() % backoff_max) + 1);
		backoff.push_back(ran);
	}
	//若上一個busy結束的時間在可以容許的傳輸時間以內，第一個mobile就成功傳輸了
	if (last_busy_end < total_duration) { data_transmitted = 1; }

	if (print) {
		cout << endl << "mobile_time" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
		cout << endl;

		cout << endl << "backoff" << endl;
		for (int i = 0; i < backoff.size(); i++)cout << backoff[i] << " ";
		cout << endl;

		cout << endl << "mobile_time+backoff+transmit_duration" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] + backoff[i] * backoff_slot + transmit_duration[i] << " ";
		cout << endl;

		cout << endl << "transmit_duration" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << transmit_duration[i] << " ";
		cout << endl;

		cout << endl << "first last_busy_end:" << last_busy_end << endl;
	}
	mobile_time[0] = 0;

	while (mobile_time[mobile_time.size() - 1] != 0)
	{
		if (print) {
			cout << endl << "While start~~" << endl;
		}
		collision = 1;
		//起始時間在上一個busy time裡面的人，起始時間改成上一個busy time結束了以後
		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] == 0)
				continue;
			if (mobile_time[i] < last_busy_end)
			{
				mobile_time[i] = last_busy_end;
				if (print) {
					cout << endl;
					cout << "last_busy_end" << last_busy_end << endl;
					cout << "new mobile_time " << i << " = " << last_busy_end << endl;
				}
			}
			else
			{
				if (print) {
					cout << "last_busy_end" << last_busy_end << endl;
					cout << "else: no mobile ahead" << endl;
				}				break;
			}
		}
		if (print) {
			cout << endl << "mobile_time" << endl;
			for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
			cout << endl;
		}

		//判斷誰是下一個可以傳輸的mobile
		while (collision == 1)
		{
			all_mobile_collision = 1;
			if (print) {
				cout << "判斷誰是下一個可以傳輸的mobile" << endl;
			}
			collision = 0;
			for (int i = 1; i < mobile_time.size(); i++)
			{
				if (mobile_time[i] != 0)
				{
					next_mobile = i;
					all_mobile_collision = 0;
					break;
				}
			}
			if (print) {
				cout << endl << "all_mobile_collision" << all_mobile_collision << endl;
			}
			if (all_mobile_collision == 1)
			{
				last_busy_end = total_duration + 1;
				break;
			}

			if (print) {
				cout << "next_mobile(1)=" << next_mobile << endl;
			}

			for (int i = 1; i < mobile_time.size(); i++)
			{
				if (mobile_time[i] == 0)
					continue;
				if (mobile_time[i] + backoff[i] * backoff_slot < mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot)
				{
					next_mobile = i;
				}
			}

			if (print) {
				cout << "next_mobile(2)=" << next_mobile << endl;
			}

			//檢查目前選定的next_mobile有沒有collision
			for (int j = 1; j < mobile_time.size(); j++)
			{
				//如果有其他mobile這樣算出來的數字和next_mobile相同，就collision了
				if (j == next_mobile) { continue; }
				if (mobile_time[j] + backoff[j] * backoff_slot == mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot)
				{
					if (print) {
						cout << "collision: mobile " << j << endl;
					}
					collision = 1;
					mobile_time[j] = 0;
				}
			}
			if (collision == 1) {
				mobile_time[next_mobile] = 0;
			}
		}

		if (all_mobile_collision == 0)
		{
			last_busy_end = mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot + transmit_duration[next_mobile];
			mobile_time[next_mobile] = 0;
		}

		if (print) {
			cout << endl << "mobile_time" << endl;
			for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
			cout << endl;

			cout << "last_busy_end" << last_busy_end << endl;
		}
		if (last_busy_end > total_duration)
		{
			break;
		}
		else if (collision == 0)
		{
			data_transmitted++;
		}
		if (print) {
			cout << endl << "data_transmitted:" << data_transmitted << endl;
		}
	}
	return data_transmitted;
}

int non_CSMA::collision_have_duration(vector<double> mobile_time, vector<double> bit_rate)
{
	vector<double> transmit_duration = calculate_transmit_duration(bits, bit_rate);
	last_busy_end = mobile_time[0] + transmit_duration[0];
	vector<int> backoff;

	//亂數出每個mobile要backoff的亂數
	for (unsigned i = 0; i < mn.size(); ++i) {
		int ran;
		ran = ((rand() % backoff_max) + 1);
		backoff.push_back(ran);
	}
	//若上一個busy結束的時間在可以容許的傳輸時間以內，第一個mobile就成功傳輸了
	if (last_busy_end < total_duration) { data_transmitted = 1; }
	if (print) {
		cout << endl << "mobile_time" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
		cout << endl;

		cout << endl << "backoff" << endl;
		for (int i = 0; i < backoff.size(); i++)cout << backoff[i] << " ";
		cout << endl;

		cout << endl << "mobile_time+backoff+transmit_duration" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] + backoff[i] * backoff_slot + transmit_duration[i] << " ";
		cout << endl;

		cout << endl << "transmit_duration" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << transmit_duration[i] << " ";
		cout << endl;

		cout << endl << "first last_busy_end:" << last_busy_end << endl;
	}
	mobile_time[0] = 0;

	while (1)
	{
		if (print) {
			cout << endl << "While start~~" << endl;
		}
		collision = 1;
		//起始時間在上一個busy time裡面的人，起始時間改成上一個busy time結束了以後
		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] == 0)
				continue;
			if (mobile_time[i] < last_busy_end)
			{
				mobile_time[i] = last_busy_end;
				if (print) {
					cout << endl;
					cout << "last_busy_end" << last_busy_end << endl;
					cout << "new mobile_time " << i << " = " << last_busy_end << endl;
				}
			}
			else
			{
				if (print) {
					cout << "last_busy_end" << last_busy_end << endl;
					cout << "else: no mobile ahead" << endl;
				}
				break;
			}
		}
		if (print) {
			cout << endl << "mobile_time" << endl;
			for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
			cout << endl;
		}

		//判斷誰是下一個可以傳輸的mobile
		all_mobile_collision = 1;
		if (print) {
			cout << "判斷誰是下一個可以傳輸的mobile" << endl;
		}
		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] != 0)
			{
				next_mobile = i;
				all_mobile_collision = 0;
				break;
			}
		}
		if (print) {
			cout << endl << "all_mobile_collision: " << all_mobile_collision << endl;
		}
		if (all_mobile_collision == 1)
		{
			last_busy_end = total_duration + 1;
			break;
		}

		if (print) {
			cout << "next_mobile(1)=" << next_mobile << endl;
		}

		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] == 0)
				continue;
			if (mobile_time[i] + backoff[i] * backoff_slot < mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot)
			{
				next_mobile = i;
			}
		}

		if (print) {
			cout << "next_mobile(2)=" << next_mobile << endl;
		}

		last_busy_end = mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot + transmit_duration[next_mobile];

		if (print) {
			cout << "last_busy_end" << last_busy_end << endl;
		}

		//檢查目前選定的next_mobile有沒有collision
		collision = 0;
		for (int j = 1; j < mobile_time.size(); j++)
		{
			//如果有其他mobile這樣算出來的數字和next_mobile相同，就collision了
			if (j == next_mobile) { continue; }
			if (mobile_time[j] + backoff[j] * backoff_slot == mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot)
			{
				if (print) {
					cout << "collision: mobile " << j << endl;
				}
				collision = 1;
				if (mobile_time[j] + backoff[j] * backoff_slot + transmit_duration[j] > last_busy_end)
				{
					last_busy_end = mobile_time[j] + backoff[j] * backoff_slot + transmit_duration[j];
				}
				mobile_time[j] = 0;
			}
		}

		mobile_time[next_mobile] = 0;

		if (print) {
			cout << endl << "mobile_time" << endl;
			for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
			cout << endl;

			cout << "last_busy_end" << last_busy_end << endl;
		}
		if (last_busy_end > total_duration)
		{
			break;
		}
		else if (collision == 0)
		{
			data_transmitted++;
		}
		if (print) {
			cout << endl << "data_transmitted:" << data_transmitted << endl;
		}
	}

	return data_transmitted;
}

int non_CSMA::collision_capture(vector<double> mobile_time, vector<double> bit_rate, vector<double> SINR)
{
	//cout << "backoff_max: " << backoff_max << endl;
	vector<double> transmit_duration = calculate_transmit_duration(bits, bit_rate);
	last_busy_end = mobile_time[0] + transmit_duration[0];
	int SINR1 = 0;
	int SINR2 = 0;
	double longest_end = 0;
	vector<int> backoff;
	data_transmitted = 0;

	//亂數出每個mobile要backoff的亂數
	for (unsigned i = 0; i < mn.size(); ++i) {
		int ran;
		ran = ((rand() % backoff_max) + 1);
		backoff.push_back(ran);
	}
	//若上一個busy結束的時間在可以容許的傳輸時間以內，第一個mobile就成功傳輸了
	if (last_busy_end < total_duration) { data_transmitted = 1; }

	if (print) {
		cout << endl << "mobile_time" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
		cout << endl;

		cout << endl << "backoff" << endl;
		for (int i = 0; i < backoff.size(); i++)cout << backoff[i] << " ";
		cout << endl;

		cout << endl << "mobile_time+backoff+transmit_duration" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] + backoff[i] * backoff_slot + transmit_duration[i] << " ";
		cout << endl;

		cout << endl << "transmit_duration" << endl;
		for (int i = 0; i < mobile_time.size(); i++)cout << transmit_duration[i] << " ";
		cout << endl;

		cout << endl << "SINR" << endl;
		for (int i = 0; i < SINR.size(); i++)cout << SINR[i] << " ";
		cout << endl;

		cout << endl << "first last_busy_end:" << last_busy_end << endl;
	}
	mobile_time[0] = 0;

	while (1)
	{
		SINR1 = 0;
		SINR2 = 0;
		longest_end = 0;
		bool capture = 0;
		if (print) {
			cout << endl << "While start~~" << endl;
		}
		collision = 0;
		all_mobile_collision = 0;

		//判斷誰是下一個可以傳輸的mobile
		if (print) {
			cout << "判斷誰是下一個可以傳輸的mobile" << endl;
		}

		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] != 0)
			{
				next_mobile = i;
				break;
			}
		}

		if (print) {
			cout << "next_mobile(1)=" << next_mobile << endl;
		}

		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] == 0)
				continue;
			if (mobile_time[i] + backoff[i] * backoff_slot < mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot)
			{
				next_mobile = i;
			}
		}

		if (print) {
			cout << "next_mobile(2)=" << next_mobile << endl;
		}

		SINR1 = next_mobile;
		longest_end = mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot + transmit_duration[next_mobile];

		//檢查目前選定的next_mobile有沒有collision
		for (int j = 1; j < mobile_time.size(); j++)
		{
			//如果有其他mobile這樣算出來的數字和next_mobile相同，就collision了
			if (j == next_mobile) { continue; }   //自己不會跟自己collision(?
			if (mobile_time[j] + backoff[j] * backoff_slot == mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot)
			{
				collision = 1;

				//找出最久的mobile
				if (longest_end < mobile_time[j] + backoff[j] * backoff_slot + transmit_duration[j])
				{
					longest_end = mobile_time[j] + backoff[j] * backoff_slot + transmit_duration[j];
				}

				//找出SINR最大的前兩名
				if (SINR[j] > SINR[SINR1])
				{
					if (SINR1 != next_mobile) { mobile_time[SINR1] = 0; }
					SINR2 = SINR1;
					SINR1 = j;
				}
				else if (SINR[j] > SINR[SINR2])
				{
					mobile_time[j] = 0;
					SINR2 = j;
				}
				else if (SINR2 == 0)
				{
					SINR2 = j;
					mobile_time[j] = 0;
				}
				else
				{
					mobile_time[j] = 0;
				}
				if (print) {
					cout << "collision: " << collision << endl;
					cout << "collision: mobile " << j << endl;
					cout << "SINR1: " << SINR1 << endl;
					cout << "SINR2: " << SINR2 << endl;
					cout << "mobile_time:" << endl;
					for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
					cout << endl;
				}
			}
		}

		//看是否符合capture條件
		if ((collision == 1) && (SINR[SINR1] - SINR[SINR2] >= capture_threshold)) {
			if (print)
			{
				cout << "capture~" << endl;
			}
			capture = 1;
		}

		//計算last_busy_end
		if (collision == 0)
		{
			last_busy_end = mobile_time[next_mobile] + backoff[next_mobile] * backoff_slot + transmit_duration[next_mobile];
		}
		else
		{
			last_busy_end = longest_end;
		}
		mobile_time[SINR1] = 0;
		mobile_time[next_mobile] = 0;
		if (print) {
			cout << "last_busy_end" << last_busy_end << endl;
			cout << "mobile_time" << endl;
			for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
			cout << endl;
		}

		//判斷是否 all collision
		all_mobile_collision = 1;
		for (int i = 0; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] != 0)
			{
				all_mobile_collision = 0;
				break;
			}
		}
		if (print)
		{
			cout << "all_mobile_collision: " << all_mobile_collision << endl;
		}

		//判斷是否超時
		if (last_busy_end > total_duration)
		{
			if (print) {
				cout << endl << "total_duration: " << total_duration << endl;
				cout << endl << "time out~~ XD" << endl;
			}
			break;
		}

		//計算data_transmitted
		if (collision == 0)
		{
			data_transmitted++;
			if (all_mobile_collision == 1)
			{
				if (print) {
					cout << endl << "all_mobile_collision" << endl;
				}
				break;
			}
		}
		else if (capture)
		{
			data_transmitted++;
			if (all_mobile_collision == 1)
			{
				if (print) {
					cout << endl << "capture & all_mobile_collision" << endl;
				}
				break;
			}
		}
		else
		{
			if (all_mobile_collision == 1)
			{
				if (print) {
					cout << endl << "collision & all_mobile_collision" << endl;
				}
				break;
			}
		}
		mobile_time[SINR1] = 0;
		mobile_time[next_mobile] = 0;

		if (print) {
			cout << endl << "data_transmitted:" << data_transmitted << endl;
		}

		//起始時間在上一個busy time裡面的人，起始時間改成上一個busy time結束了以後
		for (int i = 1; i < mobile_time.size(); i++)
		{
			if (mobile_time[i] == 0)
				continue;
			if (mobile_time[i] < last_busy_end)
			{
				mobile_time[i] = last_busy_end;
				if (print) {
					cout << endl;
					cout << "last_busy_end" << last_busy_end << endl;
					cout << "new mobile_time " << i << " = " << last_busy_end << endl;
					for (int i = 0; i < mobile_time.size(); i++)cout << mobile_time[i] << " ";
					cout << endl;
				}
			}
			else
			{
				if (print) {
					cout << "last_busy_end" << last_busy_end << endl;
					cout << "else: no mobile ahead" << endl;
				}
				break;
			}
		}
	}
	return data_transmitted;
}

vector<double> non_CSMA::calculate_transmit_duration(double bits, vector<double> bit_rate)
{
	vector<double> transmit_duration;
	for (int i = 0; i < bit_rate.size(); i++)
	{
		transmit_duration.push_back(int((bits / bit_rate[i]) * 100 + 0.5) / 100.0);
	}
	return transmit_duration;
}
