#include<iostream>
//#define OPEN_THREAD//开启线程处理，有问题且提升不大
#include"Network.h"

using namespace std;
int main() {
	vector<double> g = {0.1,0.2,0.3,0.4,0.5};
	vector<double> in = { 0.9,0.3,0.8,0.4,0.1};
	Matrix<double> goal = g;
	Matrix<double> input = in;
	goal.rotate();
	input.rotate();
	Network<double> n(5,5,15,10,15,5);
	chrono::time_point<chrono::system_clock, chrono::milliseconds> tp = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now());
	long long now = tp.time_since_epoch().count();
	for (size_t i = 0; i < 6; i++)
	{
		n.output(input);
		cout<<n.train(goal)<<endl;
	}
	tp = chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now());
	now = tp.time_since_epoch().count()-now;
	cout << "time:" << now <<"ms" << endl;
	return 0;
}