#ifndef  __LEC_MatrixStorage_H__
#define __LEC_MatrixStorage_H__


#include <afxwin.h>		//��Ҫ���ڵ��Թ���


//		ClassName	��LMatrixStorage
//		Function	��Matrix Storage Operations. Can be used as base class of matrix operation.
//		Author		: LiKe


class LMatrixStorage
{

//==================================================================================================================================	
//	����
//==================================================================================================================================
protected:
	long double * m_pBuf;	//buffer for matrix items
	int m_nRow;				//row number of matrix
	int m_nCol;				//col number of matrix
	int m_nBase;			//the value that matrix row/col index based. 
							//Usually set to 1 in accordance with matrix in mathematical sense.
							//0 is applicable for two-dimensional array of C/C++

//==================================================================================================================================
//	���캯������������
//==================================================================================================================================
public:
	LMatrixStorage();								//��ʼ��Ϊ0��0�еľ���
	LMatrixStorage(int row,int col);				//��ʼ��Ϊrow��col�еľ�������ȫΪ0
	LMatrixStorage(LMatrixStorage& m);				//��ʼ��Ϊ��m��ȫ��ȵľ���
	LMatrixStorage& operator =(LMatrixStorage& m);	//��ʼ��Ϊ��m��ȫ��ȵľ���

	~LMatrixStorage();		//��������
	LMatrixStorage I(int r);//����һ��r��r�еĵ�λ�󣨶Խ�Ԫ��Ϊ1��
	inline void Empty() { m_nRow=m_nCol=0; }			//�����в������κ�Ԫ�أ�Ԫ�ظ���Ϊ0����������������Ϊ0
	
//==================================================================================================================================
//	���Բ���
//==================================================================================================================================
public:
	inline int GetRow(){ return m_nRow; }			//��ȡ����
	inline int GetCol(){ return m_nCol; }			//��ȡ����
	inline int GetBase(){ return m_nBase; }			//��ȡ��ʼ���б��
	inline void SetBase(int base){ m_nBase=base; }	//������ʼ���б��
	inline long double * GetBuf(){ return m_pBuf; }	//���ػ������׵�ַ
	inline long double * GetRowAddr(int row) {	return m_pBuf+(row-m_nBase)*m_nCol; }	//���ص�row�е��׵�ַ
	
	void SetRow(int row);	//������������������ʱ����������Ĭ��Ϊ0
	void SetCol(int col);	//������������������ʱ����������Ĭ��Ϊ0


//==================================================================================================================================
//	���в���
//==================================================================================================================================
public:
	//	==========================
	//	��������	:	DeleteRows
	//	��������	:	ɾ�������е�������
	//	nStartRow	:	�� nStartRow�п�ʼɾ�������m_nBase==1����nStartRow==1��ʾ��ʵ���ϵĵ�һ�п�ʼɾ�������nRows>0���һ��Ҳ�ᱻɾ����
	//					���m_nBase==0����nStartRow==1��ʾ��ʵ���ϵĵڶ��п�ʼɾ�������nRows>0��ڶ���Ҳ�ᱻɾ����
	//	nRows			:	ɾ����������������ʾ��ǰɾ����
	//	��				:	������9��
	//							m_nBase==1��
	//									DeleteRows(3,2)��ʾɾ����3,4�У�
	//									DeleteRows(3,-2):��ʾɾ����1,2�У�
	//									DeleteRows(-3,5)��ʾɾ����-3,-2,-1,0,1�У���-3,-2,1,0�����߼��ϲ����ڣ���˳���ִ�н����ɾ����һ��
	//							m_nBase==0��
	//									DeleteRows(3,2)��ʾɾ����4,5�У�
	//									DeleteRows(3,-2)��ʾɾ����2,3��
	int DeleteRows(int nStartRow,int nRows);
	
	//	===========================
	//	��������	:	DeleteCols
	//	��������	:	ɾ�������е�������
	//	nStartCol		:	�� nStartCol�п�ʼɾ�������m_nBase==1����nStartCol==1��ʾ��ʵ���ϵĵ�һ�п�ʼɾ�������nCols>0���һ��Ҳ�ᱻɾ����
	//						���m_nBase==0����nStartCol==1��ʾ��ʵ���ϵĵڶ��п�ʼɾ�������nCols>0��ڶ���Ҳ�ᱻɾ����
	//	nCols			:	ɾ����������������ʾ��ǰɾ����
	//	��				:	������9��
	//							m_nBase==1��
	//									DeleteCols(3,2)��ʾɾ����3,4�У�
	//									DeleteCols(3,-2)��ʾɾ����1,2��
	//									DeleteCols(-3,5)��ʾɾ����-3,-2,-1,0,1�У���-3,-2,1,0�����߼��ϲ����ڣ���˳���ִ�н����ɾ����һ��
	//							m_nBase==0��
	//									DeleteCols(3,2)��ʾɾ����4,5�У�
	//									DeleteCols(3,-2)��ʾɾ����2,3��
	int DeleteCols(int nStartCol,int nCols);		

	//	==========================
	//	��������	:	DeleteRows
	//	��������	:	ɾ�������д�nStartRow��ʼ����ȫ����
	//	nStartRow	:	�� nStartRow�п�ʼɾ�������m_nBase==1����nStartRow==1��ʾ��ʵ���ϵĵ�һ�п�ʼɾ��������һ�У�
	//					���m_nBase==0����nStartRow==1��ʾ��ʵ���ϵĵڶ��п�ʼɾ�������ڶ��У�
	//	��				:	������9��
	//							m_nBase==1��DeleteRows(3)��ʾɾ����3-9�У�DeleteRows(9)��ʾɾ����9��
	//							m_nBase==0��DeleteRows(3)��ʾɾ����4-9�У�DeleteRows(8)��ʾɾ����9��
	int DeleteRows(int nStartRow);	

	//	==========================
	//	��������	:	DeleteCols
	//	��������	:	ɾ�������д�nStartCol��ʼ����ȫ����
	//	nStartRow	:	�� nStartCol�п�ʼɾ�������m_nBase==1����nStartCol==1��ʾ��ʵ���ϵĵ�һ�п�ʼɾ��������һ�У�
	//							���m_nBase==0����nStartCol==1��ʾ��ʵ���ϵĵڶ��п�ʼɾ�������ڶ��У�
	//	��				:	������9��
	//							m_nBase==1��DeleteCols(3)��ʾɾ����3-9�У�DeleteCols(9)��ʾɾ����9��
	//							m_nBase==0��DeleteCols(3)��ʾɾ����4-9�У�DeleteCols(8)��ʾɾ����9��
	int DeleteCols(int nStartCol);						

	int DeleteRow(int nRow);		//ɾ����nRow�У�ֱ�ӵ���DeleteRows(nRow,1)
	int DeleteCol(int nCol);		//ɾ����nCol�У� ֱ�ӵ���DeleteCols(nCol,1)

	//	===================================================
	//	��������	:	InsertRows
	//	��������	:	�ھ����в���������
	//	m			:	��Ҫ�ھ����в��������
	//	nPos		:	��Ҫ�����λ�ã����뵽nPos��֮ǰ����nPos<=m_nBaseʱ����������뵽��һ��֮ǰ
	//					nPos>m_nRowʱ���������뵽���һ��֮��
	LMatrixStorage& InsertRows(LMatrixStorage& m,int nPos);	//�ڵ�nPos��֮ǰ�������m��nPos֮���������
	LMatrixStorage& InsertCols(LMatrixStorage& m,int nPos);	//�ڵ�nPos��֮ǰ�������m��nPos֮���������
	
	LMatrixStorage Sub(int sr,int sc,int nr,int nc);	//ȡ�����sr�У�sc�п�ʼ���ģ�nr��nc��
	LMatrixStorage Rows(int nStartRow,int nRows);		//ȡ�����nStartRow�п�ʼ��nRows��
	LMatrixStorage Cols(int nStartCol,int nCols);		//ȡ�����nStartCol�п�ʼ��nCols��
	LMatrixStorage Row(int row);	//ȡ����ĵ�row��
	LMatrixStorage Col(int col);	//ȡ����ĵ�col��

	//	=====================================================
	//	��������	:	Clear
	//	��������	:	ֱ�ӽ������е����ݸ�ֵΪ0
	//	nRow		:	��տ�ʼ������
	//	nNum		:	1��ʾ��յ�nRow�У�2��ʾ��յ�nRow�����nRow+1��
	//					-1��ʾ��յ�nRow-1�У�-2��ʾ��յ�nRow-2,nRow-1��
	//	����ֵ		:	ʵ����յ�����
	int Clear(int nRow,int nNum);


//==================================================================================================================================
//	���������
//==================================================================================================================================
public:
	inline long double& operator ()(int row,int col){return m_pBuf[(row-m_nBase)*m_nCol+col-m_nBase];}
	inline long double operator ()(int row,int col) const{return m_pBuf[(row-m_nBase)*m_nCol+col-m_nBase];}
	
//==================================================================================================================================
//	���Թ���
//==================================================================================================================================
public:

	void Format(CString& szText,int nLen,int nDot);	//������ĸ���Ԫ��������ַ�����
//	void Parse(CString& szText);					//���ַ����ж�ȡ����ĸ���Ԫ��

	void Save(CFile& file,int nLen,int nDot);
	void DBG(CFile& file,LPCTSTR szText);
	void InitTestValue();



};

#endif 