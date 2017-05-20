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
	LMatrix operator *(long double f);		//���������ֵ

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

	
	LMatrix Cholesky();	//�õ�Cholesky�ֽ�������Ǿ���T��һ��Э�������C���Էֽ�ΪTT'
	

/*
	bool IsPositiveDefinite();			//�жϾ����Ƿ�����������
	bool IsHalfPositiveDefinite();		//�жϾ����Ƿ��ǰ���������
//	bool IsNDefinite();	//�жϾ����Ƿ��Ǹ�������
//	bool IsHNDefinite();	//�жϾ����Ƿ��ǰ븺������


//==================================================================================================================================
//	��ѧ���㡢����ת��
//==================================================================================================================================
public:
	LMatrix Abs();			//����ԭʼ����ľ���ֵ����
	LMatrix Floor();		//����ȡ��
	LMatrix Round(int dot);	//��������
	LMatrix Ceil();			//����ȡ��
	LMatrix AFloor();		//����ֵ����ȡ��
	LMatrix ACeil();		//����ֵ����ȡ��
	
	
//==================================================================================================================================
//	ͳ������
//==================================================================================================================================
public:
	LMatrix Average();	//��ÿһ����Ϊһ�����������ɾ�ֵ��������������
	LMatrix Var();			//��ÿһ����Ϊһ�����������ɷ�����������������
	LMatrix SVar();		//��ÿһ����Ϊһ�����������ɱ�׼����������������
	LMatrix AvgAndSVar();	//��ÿһ����Ϊһ�����������ɾ�ֵ������ֵ��һ�У��ͱ�׼��ڶ��У�
	LMatrix Cov();		//��ÿһ����Ϊһ������������Э�������
	LMatrix Rel();		//��ÿһ����Ϊһ���������������ϵ������

	long double Measure0();	//���о���Ԫ�صĺ�
	long double Measure1();	//���о���Ԫ�ؾ���ֵ�ĺ�
	long double Measure2();	//���о���Ԫ�ص�ƽ����

//==================================================================================================================================
//	�����㷨
//==================================================================================================================================
public:
	LMatrix& Sort(int direction,int col);	//�Ե�col�н��п�����������������ݲ���
															//col==-1ʱ�ֱ��ÿһ�н��п�������
	LMatrix SortIndex(int direction);		//����ֵ[1,3]=4��ʾ�����е�3����С��Ԫ���ǵ�4��Ԫ��
	LMatrix SortIndex2(int direction);

	long double Max();
	long double Min();
	
private:
	LMatrix& Sort_step(int direction,int col,int startrow,int endrow);
	LMatrix& SortIndex_step(int direction,int col,int startrow,int endrow,LMatrix &si);	
	*/
};

#endif 