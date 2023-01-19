#pragma once
#ifndef Rnn_h
#define Rnn_h
#include"Matrix.h"
#include<math.h>
double tanh(double x) {
	return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}
double sigmoid(double x) {
	return 1.0 / (1 + exp(-x));
}
class Rnn
{
public:
	Rnn(int in_num, int out_num);
	Matrix<> OutPut(Matrix<> input);
	~Rnn();

private:
	Matrix<> WO_Data;
	Matrix<> WX_Data;
	Matrix<> WH_Data;
	Matrix<> InputX_Data;
	Matrix<> InputH_Data;
};

Rnn::Rnn(int in_num,int out_num)
:InputX_Data(1, in_num),InputH_Data(1, in_num),WH_Data(in_num, out_num)
,WX_Data(in_num, out_num), WO_Data(out_num, out_num){

}
Matrix<> Rnn::OutPut(Matrix<> input) {
	this->InputH_Data = input;
	this->InputX_Data = input;
	Matrix tempi = InputX_Data.hconcat(InputH_Data);
	Matrix tempw = WX_Data.hconcat(WH_Data);
	tempi.rotate();
	Matrix oh = tempw * tempi;
	oh.fun_handle(tanh);
	oh = WO_Data * oh;
	oh.fun_handle(sigmoid);
	return oh;

}

Rnn::~Rnn() {
}

#endif 
