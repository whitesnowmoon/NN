#pragma once
#include"Matrix.h"

template<typename Type_>
class Layer
{
public:
	Matrix<Type_> weight_data;
	Matrix<Type_> output_data;
	Matrix<Type_> input_data;
	Type_ v_Learnspeed;
	Layer();
	virtual void updata_weight();
	virtual Matrix<Type_> forward(Matrix<Type_>& input);
 
	virtual ~Layer();
	
private:

};

