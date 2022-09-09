#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#include<stdlib.h>
#include<iostream>
#include <iomanip>
#include<vector>
#include<mutex>
#include<thread>
#include<queue>
#include<atomic>
#ifdef OPEN_THREAD
static std::mutex update_mutex;
class Task
{
public:
	virtual void run() {
	}
	virtual ~Task() {
	}
};
class ThreadPool
{
public:
	ThreadPool(int count) :running_flag_(false) {
		std::cout << "thread open!\n";
		for (int i = 0; i < count; i++)
		{
			threads.emplace_back(&ThreadPool::work, this);
		}
	}


	~ThreadPool() {
		stop();
	}
	void push_task(Task* task) {
		std::unique_lock<std::mutex> unlock(this->mtx_);
		this->tasks.push(task);
		this->cv_.notify_one();
	}
	void start() {
		running_flag_ = true;
	}
	void stop() {
		running_flag_ = false;
		cv_.notify_all();
		for (auto& th : threads) {
			if (th.joinable()) {
				th.join();
			}
		}
	}
private:
	void work() {
		while (true) {
			if (!running_flag_) {
				return;
			}
			Task* task = nullptr;
			{
				std::unique_lock<std::mutex> unlock(this->mtx_);
				cv_.wait(unlock, [&] {
					return !tasks.empty() || !running_flag_;
					});
				if (running_flag_) {
					task = this->tasks.front();
					this->tasks.pop();
				}
			}
			if (task != nullptr) {
				task->run();
			}
		}
	}
	std::vector<std::thread> threads;
	std::queue<Task*> tasks;
	std::atomic_bool running_flag_;
	std::condition_variable cv_;
	std::mutex mtx_;

};
#endif // OPEN_THREAD


double sigmoid(double x) {
	double out = 1 / (1 + exp(-x));
	if (out < 1.0e-8) {
		return 0;
	}
	return out;
}

double ss_back(double x) {
	return (1 - x) * x;
}
template<typename Type_ = double>
class Layer;
template<typename Type_=double>
class Matrix
{
	template<typename Type_ = double>
	friend class Layer;
public:
	Matrix() {
		this->Matrix_row = 0;
		this->Matrix_column = 0;
		this->Matrix_data = nullptr;
	}
	explicit Matrix(int Matrix_row, int Matrix_column) {
		this->Matrix_row = Matrix_row;
		this->Matrix_column = Matrix_column;
		Matrix_data = new Type_ * [Matrix_row];
		for (int i = 0; i < Matrix_row; i++)
		{
			Matrix_data[i] = new Type_[Matrix_column];
		}
		for (int i = 0; i < Matrix_row; i++)
		{
			for (int j = 0; j < Matrix_column; j++)
			{
				Matrix_data[i][j] = ((Type_)rand()) / RAND_MAX;
			}
		}
	};
	Matrix(const Matrix& old) :Matrix_row(old.Matrix_row), Matrix_column(old.Matrix_column), Matrix_data(new double* [old.Matrix_row]) {
		for (int i = 0; i < this->Matrix_row; i++)
		{
			this->Matrix_data[i] = new double[this->Matrix_column];
			memcpy(this->Matrix_data[i], old.Matrix_data[i], old.Matrix_column * sizeof(double));
		}
	}
	Matrix(Matrix&& old) :Matrix_row(old.Matrix_row), Matrix_column(old.Matrix_column), Matrix_data(old.Matrix_data) {
		old.Matrix_data = nullptr;
	}
	Matrix(std::vector<Type_>& vec_n):Matrix_row(1), Matrix_column(vec_n.size()){
		Matrix_data = new Type_ * [Matrix_row];
		for (int i = 0; i < Matrix_row; i++)
		{
			Matrix_data[i] = new Type_[Matrix_column];
			memcpy(Matrix_data[i], vec_n.data(), sizeof(Type_) * vec_n.size() );
		}

	}
	~Matrix() {
		if (Matrix_data != nullptr) {
			for (int i = 0; i < Matrix_row; i++)
			{
					delete Matrix_data[i];
					Matrix_data[i] = nullptr;
			}
			delete Matrix_data;
			Matrix_data = nullptr;
		}

	}
	void print() {
		for (int i = 0; i < Matrix_row; i++)
		{
			for (int j = 0; j < Matrix_column; j++)
			{
				printf("%3.4lf ", Matrix_data[i][j]);
			}
			printf("\n");
		}
	}
	Matrix& operator=(Matrix&& old) {
		if (this->Matrix_data != nullptr) {
			for (int i = 0; i < Matrix_row; i++)
			{
				delete this->Matrix_data[i];
				this->Matrix_data[i] = nullptr;
			}
			delete Matrix_data;
			this->Matrix_data = nullptr;
		}
		this->Matrix_column = old.Matrix_column;
		this->Matrix_data = old.Matrix_data;
		this->Matrix_row = old.Matrix_row;
		old.Matrix_data = nullptr;
		return *this;
	}
	Matrix& operator=(const Matrix& old) {
		if (this->Matrix_data != nullptr) {
			for (int i = 0; i < this->Matrix_row; i++)
			{
				delete this->Matrix_data[i];
				this->Matrix_data[i] = nullptr;
			}
			delete Matrix_data;
			this->Matrix_data = nullptr;
		}
		this->Matrix_column = old.Matrix_column;
		this->Matrix_row = old.Matrix_row;
		this->Matrix_data = new double* [this->Matrix_row];
		for (int i = 0; i < this->Matrix_row; i++)
		{
			Matrix_data[i] = new double[this->Matrix_column];
			memcpy(Matrix_data[i], old.Matrix_data[i], old.Matrix_column * sizeof(Type_));
		}
		return *this;
	}
	Matrix& operator=(std::vector<Type_>& vec_n){
		this->Matrix_row = 1;
		this->Matrix_column = vec_n.size();
		Matrix_data = new Type_ * [Matrix_row];
		for (int i = 0; i < Matrix_row; i++)
		{
			Matrix_data[i] = new Type_[Matrix_column];
			memcpy(Matrix_data[i], vec_n.data(), sizeof(Type_) * vec_n.size());
		}
		return *this;
	}
	Matrix operator*(Matrix& mutipul) {

		Matrix putn(this->Matrix_row, mutipul.Matrix_column);
		for (int i = 0; i < this->Matrix_row; i++)
		{
			for (int j = 0; j < mutipul.Matrix_column; j++)
			{
				double in = 0;
				for (int ti = 0; ti < this->Matrix_column; ti++)
				{
					in += this->Matrix_data[i][ti] * mutipul.Matrix_data[ti][j];
				}
				putn.Matrix_data[i][j] = in;
			}
		}
		return putn;
	}
	Matrix operator*(Type_ num) {
		for (int i = 0; i < this->Matrix_row; i++)
		{
			for (int j = 0; j < this->Matrix_column; j++)
			{
				
				this->Matrix_data[i][j] *=num ;
			}
		}
		return *this;
	}
	Matrix operator-(Matrix& sub) {
		Matrix putn(sub.Matrix_row, sub.Matrix_column);
		for (int i = 0; i < sub.Matrix_row; i++)
		{
			for (int j = 0; j < sub.Matrix_column; j++)
			{
				putn.Matrix_data[i][j] = this->Matrix_data[i][j] - sub.Matrix_data[i][j];
			}
		}
		return putn;
	}
	Matrix operator+(Matrix& add) {
		Matrix putn(add.Matrix_row, add.Matrix_column);
		for (int i = 0; i < add.Matrix_row; i++)
		{
			for (int j = 0; j < add.Matrix_column; j++)
			{
				putn.Matrix_data[i][j] = this->Matrix_data[i][j] + add.Matrix_data[i][j];
			}
		}
		return putn;
	}
	void fun_handle(Type_(*fun)(Type_)) {
		for (int i = 0; i < Matrix_row; i++)
		{
			for (int j = 0; j < Matrix_column; j++)
			{
				Matrix_data[i][j] = fun(Matrix_data[i][j]);
			}
		}
	}
	void rotate() {
		Type_** nn;
		nn = new Type_ * [Matrix_column];
		for (int i = 0; i < Matrix_column; i++)
		{
			nn[i] = new Type_[Matrix_row];
		}
		for (int i = 0; i < this->Matrix_row; i++)
		{
			for (int j = 0; j < this->Matrix_column; j++)
			{
				nn[j][i] = this->Matrix_data[i][j];
			}
		}
		for (int i = 0; i < Matrix_row; i++)
		{
			delete Matrix_data[i];
		}
		delete Matrix_data;
		Matrix_data = nn;
		int t = Matrix_column;
		Matrix_column = Matrix_row; Matrix_row = t;
	}
	friend std::ostream& operator<<(std::ostream& out, const Matrix& t) {
		for (int i = 0; i < t.Matrix_row; i++)
		{
			for (int j = 0; j < t.Matrix_column; j++)
			{
				out << std::fixed << std::setprecision(4) << t.Matrix_data[i][j] << " ";
			}
			out << std::endl;
		}
		return out;
	}
	const Type_* const* Get_Matrix_data(void) {
		return this->Matrix_data;
	}
	int Get_Matrix_row(void) {
		return this->Matrix_row;
	}
	int Get_Matrix_column(void) {
		return this->Matrix_column;
	}
private:
	Type_** Matrix_data;
	int Matrix_row;
	int Matrix_column;
};

template<typename Type_ = double>
#ifdef OPEN_THREAD
class Layer :public Task
#else
class Layer
#endif 
{
public:

	virtual void run() {
			//std::cout << std::this_thread::get_id() << std::endl;
			this->updata();

	}
	explicit Layer(int row_num, int column_num) :w(row_num, column_num), input(column_num, 1), output(row_num, 1), e(row_num, 1), ss(row_num, 1) {

	}
	Matrix<Type_> out_(Matrix<Type_>& input) {
		this->input = input;
		this->output = w * input;
		this->output.fun_handle(sigmoid);
		this->ss = this->output;
		ss.fun_handle(ss_back);
		return this->output;
	}
	void updata() {
		for (int i = 0; i < ss.Matrix_row; i++)
		{
			for (int j = 0; j < ss.Matrix_column; j++)
			{
				ss.Matrix_data[i][j] = ss.Matrix_data[i][j] * e.Matrix_data[i][j];
			}
		}
		input.rotate();
		Matrix<Type_> dw = ss * input;
		w = w - dw;

	}
	void get_e(Matrix<Type_>& goal) {
		this->e_sum = 0;
		for (int i = 0; i < e.Matrix_row; i++)
		{
			for (int j = 0; j < e.Matrix_column; j++)
			{
				double t = (this->output.Matrix_data[i][j] - goal.Matrix_data[i][j]);
				this->e.Matrix_data[i][j] = t;
				this->e_sum += fabs(t);

			}
		}
	}
	Matrix<Type_> e_back(void) {
		Matrix<Type_> bw = this->w;
		bw.rotate();
		return bw * e;
	}
	void set_e(Matrix<Type_>& E) {
		this->e = E;
	}
	~Layer() {}

	Matrix<Type_> input;
	Matrix<Type_> output;
	Matrix<Type_> e;
	double v = 0.9;
	double e_sum=0;
private:
	Matrix<Type_> w;
	Matrix<Type_> ss;
};

template<typename Type_ = double>
class Network 
{
public:
	std::vector<Layer<Type_>> net_layers;
#ifdef OPEN_THREAD
	ThreadPool Pool;
	Network(int int_Layer_number,...):Pool(std::thread::hardware_concurrency()) {
		Pool.start();
		int* parameter = &int_Layer_number;
		parameter++;
		for (int i = 0; i < int_Layer_number -1; i++) {
			net_layers.emplace_back(parameter[i + 1], parameter[i]);
		}
	}
#else
	Network(int int_Layer_number, ...){
		int* parameter = &int_Layer_number;
		parameter++;
		for (int i = 0; i < int_Layer_number - 1; i++) {
			net_layers.emplace_back(parameter[i + 1], parameter[i]);
		}
	}
#endif 
	Matrix<Type_> output(Matrix<Type_>& in) {
		Matrix<Type_> t;
		t = in;
		for (unsigned int i = 0; i < net_layers.size(); i++)
		{
			t = net_layers[i].out_(t);
		}
		return t;
	}
	Matrix<Type_> train(Matrix<Type_>& goal) {
		Matrix<Type_> t;
		net_layers[net_layers.size() - 1].get_e(goal);
		for (int i = net_layers.size() - 1; i >= 1; i--)
		{
			t = net_layers[i].e_back();
			net_layers[i - 1].set_e(t);
		}
		for (int i = 0; i < net_layers.size(); i++)
		{
#ifdef OPEN_THREAD
			Pool.push_task(&net_layers[i]);
#else
			net_layers[i].updata();
#endif // OPEN_THREAD
		}
		return net_layers[net_layers.size() - 1].e;
	}

};


#endif 

