#ifndef __LEC_MATRIX_H__
#define __LEC_MATRIX_H__

#include "LMatrixStorage.h"

class LMatrix : public LMatrixStorage
{
private:	
	long double m_fZero;	//
public:
	inline void SetZero(long double f)	{ m_fZero=f; }
	inline long double GetZero()		{ return m_fZero; }
	bool IsZero(long double f) { return f<=m_fZero&&f>=0-m_fZero; }
//==================================================================================================================================
//	Constructor / Destructor
//==================================================================================================================================
public:
	LMatrix();
	LMatrix(int row,int col);
	LMatrix(LMatrix& m);
	LMatrix(LMatrixStorage& m);

	//	=======================
	//	Name	:	I
	//	Func	:	Create an identity matrix. Is different with base class "LMatrixStorage" in that LMatrix::I call SetZero()
	LMatrix I(int row);

//==================================================================================================================================
//	Operator Overload
//==================================================================================================================================
public:
	//	=======================
	//	Name	:	operator ==
	//	Func	:	Determine whether the two matrices are equal
	//	return	:	true(1) means equal, false(0) means different
	//	note	:	1)	two empty matrices with different sizes are different. 
	//					for example, m1(0,1) is not equal to m2(0,0)
	//				2)	because lower bound of row and col num is 0
	//					so m1(-1,-1) is equal to m2(-2,-3)
	bool operator==(LMatrix& m);

	bool operator!=(LMatrix& m);
	LMatrix& operator =(LMatrix& m);
	LMatrix& operator =(LMatrixStorage& m);



	//	======================
	//	Name	:	operator +=
	//	Func		:	add another matrix to this one
	//	m			:	another matrix
	//	return	:	reference of this matrix
	//	note		:	if two matrices have different sizes, the return value is matrix with 0 row and 0 col
	LMatrix& operator +=(LMatrix& m);	

	//	======================
	//	Name	:	operator -=
	//	Func		:	subtract  another matrix from this one
	//	m			:	another matrix
	//	return	:	reference of this matrix
	//	note		:	if two matrices have different sizes, the return value is matrix with 0 row and 0 col
	LMatrix& operator -=(LMatrix& m);	

	//	======================
	//	Name	:	operator *=
    //	Func		:	multiplied by another matrix
	//	m			:	another matrix
	//	return	:	reference of this matrix
	//	note		:	if two matrices are not satisfy the condition of matrix multiply, then return matrix with 0 row and 0 col
	LMatrix& operator *=(LMatrix& m);	

	//	======================
	//	Name	:	operator /=
	//	Func	:	multiplied by the inverse of another matrix
	//	m		:	another matrix
	//	return	:	reference of this matrix
	//	note	:	if m is not inversable, or two matrices are not satisfy the condition of matrix multiply, then return matrix with 0 row and 0 col	
	LMatrix& operator /=(LMatrix& m);	

	//	======================
	//	Name	:	operator *=
	//	Func		:	multiply the matrix with a number
	//	f			:	multiplier
	//	return	:	reference of this matrix
	LMatrix& operator *=(long double f);

	//	======================
	//	Name	:	operator /=
	//	Func		:	divide the matrix with a number
	//	f			:	divisor
	//	return	:	reference of this matrix
	//	note		:	if f equal 0, return matrix with 0 row and 0 col
	LMatrix& operator /=(long double f);

	//	======================
	//	Name	:	operator ^=
	//	Func		:	power of the matrix
	//	f			:	exponent
	//	return	:	reference of this matrix
	//	note		:	if f < 0, means power of inverse matrix
	LMatrix& operator ^=(int f);

	//	======================
	//	Name	:	operator +
	//	Func		:	sum of two matrices, return a new matrix
	//	m			:	another matrix
	//	return	:	new matrix represent sum of two matrices
	//	note		:	if two matrices have different sizes, the return value is matrix with 0 row and 0 col
	LMatrix operator +(LMatrix& m);		

	//	======================
	//	Name	:	operator -
	//	Func		:	difference of two matrices, return a new matrix
	//	m			:	another matrix
	//	return	:	new matrix represent difference of two matrices
	//	note		:	if two matrices have different sizes, the return value is matrix with 0 row and 0 col
	LMatrix operator -(LMatrix& m);		

	//	======================
	//	Name	:	operator *
	//	Func		:	product of two matrices, return a new matrix
	//	m			:	another matrix
	//	return	:	new matrix represent product of two matrices
	//	note		:	if two matrices are not satisfy the condition of matrix multiply, then return matrix with 0 row and 0 col
	LMatrix operator *(LMatrix& m);		


	//	======================
	//	Name	:	operator /
	//	Func		:	product one matrix with inverse of another, return a new matrix
	//	m			:	another matrix
	//	return	:	new matrix represent product of two matrices
	//	note		:	if m is not inversable, or two matrices are not satisfy the condition of matrix multiply, then return matrix with 0 row and 0 col
	LMatrix operator /(LMatrix& m);		
	
	
	//	======================
	//	Name	:	operator *
	//	Func		:	product one matrix with a number, return a new matrix
	//	f			:	multiplier
	//	return	:	a new matrix represent the product 
	LMatrix operator *(long double f);		//矩阵乘以数值

	//	======================
	//	Name	:	operator /
	//	Func		:	devide one matrix with a number, return a new matrix
	//	f			:	divisor
	//	return	:	new matrix represent the quotient
	LMatrix operator /(long double f);	

	//	======================
	//	Name	:	operator ^
	//	Func		:	power of the matrix, return a new matrix
	//	i			:	exponent, negative number means power of inverse matrix
	//	return	:	new matrix
	LMatrix operator ^(int i);
	
	

//==================================================================================================================================
//	Matrix Operation
//==================================================================================================================================
public:
	//	======================
	//	Name	:	Trans
	//	Func		:	transposition of matrix
	//	return	:	a new matrix  
	LMatrix Trans();

	//	======================
	//	Name	:	Deter
	//	Func		:	determinant of matrix
	//	return	:	determinant of matrix
	long double Deter();	 	

	
	LMatrix Cholesky();	//得到Cholesky分解的下三角矩阵T，一般协方差矩阵C可以分解为TT'
	

/*
	bool IsPositiveDefinite();			//判断矩阵是否是正定矩阵
	bool IsHalfPositiveDefinite();		//判断矩阵是否是半正定矩阵
//	bool IsNDefinite();	//判断矩阵是否是负定矩阵
//	bool IsHNDefinite();	//判断矩阵是否是半负定矩阵


//==================================================================================================================================
//	数学运算、类型转换
//==================================================================================================================================
public:
	LMatrix Abs();			//生成原始矩阵的绝对值矩阵
	LMatrix Floor();		//向下取整
	LMatrix Round(int dot);	//四舍五入
	LMatrix Ceil();			//向上取整
	LMatrix AFloor();		//绝对值向下取整
	LMatrix ACeil();		//绝对值向上取整
	
	
//==================================================================================================================================
//	统计运算
//==================================================================================================================================
public:
	LMatrix Average();	//把每一列视为一个向量，生成均值向量（行向量）
	LMatrix Var();			//把每一列视为一个向量，生成方差向量（行向量）
	LMatrix SVar();		//把每一列视为一个向量，生成标准差向量（行向量）
	LMatrix AvgAndSVar();	//把每一列视为一个向量，生成均值（返回值第一行）和标准差（第二行）
	LMatrix Cov();		//把每一列视为一个向量，生成协方差矩阵
	LMatrix Rel();		//把每一列视为一个向量，生成相关系数矩阵

	long double Measure0();	//所有矩阵元素的和
	long double Measure1();	//所有矩阵元素绝对值的和
	long double Measure2();	//所有矩阵元素的平方和

//==================================================================================================================================
//	排序算法
//==================================================================================================================================
public:
	LMatrix& Sort(int direction,int col);	//对第col列进行快速排序，其余各列数据不变
															//col==-1时分别对每一列进行快速排序
	LMatrix SortIndex(int direction);		//返回值[1,3]=4表示矩阵中第3列最小的元素是第4个元素
	LMatrix SortIndex2(int direction);

	long double Max();
	long double Min();
	
private:
	LMatrix& Sort_step(int direction,int col,int startrow,int endrow);
	LMatrix& SortIndex_step(int direction,int col,int startrow,int endrow,LMatrix &si);	
	*/
};

#endif 