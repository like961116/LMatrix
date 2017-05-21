#ifndef  __LEC_MatrixStorage_H__
#define __LEC_MatrixStorage_H__


#include <afxwin.h>		//主要用于调试功能


//		ClassName	：LMatrixStorage
//		Function	：Matrix Storage Operations. Can be used as base class of matrix operation.
//		Author		: LiKe


class LMatrixStorage
{

//==================================================================================================================================	
//	属性
//==================================================================================================================================
protected:
	long double * m_pBuf;	//buffer for matrix items
	int m_nRow;				//row number of matrix
	int m_nCol;				//col number of matrix
	int m_nBase;			//the value that matrix row/col index based. 
							//Usually set to 1 in accordance with matrix in mathematical sense.
							//0 is applicable for two-dimensional array of C/C++

//==================================================================================================================================
//	构造函数、析构函数
//==================================================================================================================================
public:
	LMatrixStorage();								//初始化为0行0列的矩阵
	LMatrixStorage(int row,int col);				//初始化为row行col列的矩阵，数据全为0
	LMatrixStorage(LMatrixStorage& m);				//初始化为与m完全相等的矩阵
	LMatrixStorage& operator =(LMatrixStorage& m);	//初始化为与m完全相等的矩阵

	~LMatrixStorage();		//析构函数
	LMatrixStorage I(int r);//创建一个r行r列的单位阵（对角元素为1）
	inline void Empty() { m_nRow=m_nCol=0; }			//矩阵中不包含任何元素，元素个数为0，即矩阵行列数都为0
	
//==================================================================================================================================
//	属性操作
//==================================================================================================================================
public:
	inline int GetRow(){ return m_nRow; }			//读取行数
	inline int GetCol(){ return m_nCol; }			//读取列数
	inline int GetBase(){ return m_nBase; }			//读取起始行列编号
	inline void SetBase(int base){ m_nBase=base; }	//设置起始行列编号
	inline long double * GetBuf(){ return m_pBuf; }	//返回缓冲区首地址
	inline long double * GetRowAddr(int row) {	return m_pBuf+(row-m_nBase)*m_nCol; }	//返回第row行的首地址
	
	void SetRow(int row);	//设置行数，行数增加时，新增数据默认为0
	void SetCol(int col);	//设置列数，列数增加时，新增数据默认为0


//==================================================================================================================================
//	行列操作
//==================================================================================================================================
public:
	//	==========================
	//	函数名称	:	DeleteRows
	//	函数功能	:	删除矩阵中的若干行
	//	nStartRow	:	从 nStartRow行开始删除，如果m_nBase==1，则nStartRow==1表示从实际上的第一行开始删除（如果nRows>0则第一行也会被删除）
	//					如果m_nBase==0，则nStartRow==1表示从实际上的第二行开始删除（如果nRows>0则第二行也会被删除）
	//	nRows			:	删除的行数，负数表示向前删除。
	//	例				:	矩阵共有9行
	//							m_nBase==1：
	//									DeleteRows(3,2)表示删除第3,4行；
	//									DeleteRows(3,-2):表示删除第1,2行；
	//									DeleteRows(-3,5)表示删除第-3,-2,-1,0,1行，但-3,-2,1,0行在逻辑上不存在，因此程序执行结果是删除第一行
	//							m_nBase==0：
	//									DeleteRows(3,2)表示删除第4,5行；
	//									DeleteRows(3,-2)表示删除第2,3行
	int DeleteRows(int nStartRow,int nRows);
	
	//	===========================
	//	函数名称	:	DeleteCols
	//	函数功能	:	删除矩阵中的若干列
	//	nStartCol		:	从 nStartCol列开始删除，如果m_nBase==1，则nStartCol==1表示从实际上的第一列开始删除（如果nCols>0则第一列也会被删除）
	//						如果m_nBase==0，则nStartCol==1表示从实际上的第二列开始删除（如果nCols>0则第二列也会被删除）
	//	nCols			:	删除的列数，负数表示向前删除。
	//	例				:	矩阵共有9列
	//							m_nBase==1：
	//									DeleteCols(3,2)表示删除第3,4列；
	//									DeleteCols(3,-2)表示删除第1,2列
	//									DeleteCols(-3,5)表示删除第-3,-2,-1,0,1列，但-3,-2,1,0行在逻辑上不存在，因此程序执行结果是删除第一列
	//							m_nBase==0：
	//									DeleteCols(3,2)表示删除第4,5列；
	//									DeleteCols(3,-2)表示删除第2,3列
	int DeleteCols(int nStartCol,int nCols);		

	//	==========================
	//	函数名称	:	DeleteRows
	//	函数功能	:	删除矩阵中从nStartRow开始向后的全部行
	//	nStartRow	:	从 nStartRow行开始删除，如果m_nBase==1，则nStartRow==1表示从实际上的第一行开始删除（含第一行）
	//					如果m_nBase==0，则nStartRow==1表示从实际上的第二行开始删除（含第二行）
	//	例				:	矩阵共有9行
	//							m_nBase==1：DeleteRows(3)表示删除第3-9行；DeleteRows(9)表示删除第9行
	//							m_nBase==0：DeleteRows(3)表示删除第4-9行；DeleteRows(8)表示删除第9行
	int DeleteRows(int nStartRow);	

	//	==========================
	//	函数名称	:	DeleteCols
	//	函数功能	:	删除矩阵中从nStartCol开始向后的全部列
	//	nStartRow	:	从 nStartCol列开始删除，如果m_nBase==1，则nStartCol==1表示从实际上的第一列开始删除（含第一列）
	//							如果m_nBase==0，则nStartCol==1表示从实际上的第二列开始删除（含第二列）
	//	例				:	矩阵共有9列
	//							m_nBase==1：DeleteCols(3)表示删除第3-9列；DeleteCols(9)表示删除第9列
	//							m_nBase==0：DeleteCols(3)表示删除第4-9列；DeleteCols(8)表示删除第9列
	int DeleteCols(int nStartCol);						

	int DeleteRow(int nRow);		//删除第nRow行，直接调用DeleteRows(nRow,1)
	int DeleteCol(int nCol);		//删除第nCol列， 直接调用DeleteCols(nCol,1)

	//	===================================================
	//	函数名称	:	InsertRows
	//	函数功能	:	在矩阵中插入若干行
	//	m			:	需要在矩阵中插入的数据
	//	nPos		:	需要插入的位置（插入到nPos行之前），nPos<=m_nBase时，则表明插入到第一行之前
	//					nPos>m_nRow时，表明插入到最后一行之后
	LMatrixStorage& InsertRows(LMatrixStorage& m,int nPos);	//在第nPos行之前插入矩阵m，nPos之后的行下移
	LMatrixStorage& InsertCols(LMatrixStorage& m,int nPos);	//在第nPos列之前插入矩阵m，nPos之后的列右移
	
	LMatrixStorage Sub(int sr,int sc,int nr,int nc);	//取矩阵的sr行，sc列开始处的，nr行nc列
	LMatrixStorage Rows(int nStartRow,int nRows);		//取矩阵的nStartRow行开始的nRows行
	LMatrixStorage Cols(int nStartCol,int nCols);		//取矩阵的nStartCol列开始的nCols列
	LMatrixStorage Row(int row);	//取矩阵的第row行
	LMatrixStorage Col(int col);	//取矩阵的第col列

	//	=====================================================
	//	函数名称	:	Clear
	//	函数功能	:	直接将若干行的数据赋值为0
	//	nRow		:	清空开始的行数
	//	nNum		:	1表示清空第nRow行，2表示清空第nRow行与第nRow+1行
	//					-1表示清空第nRow-1行，-2表示清空第nRow-2,nRow-1行
	//	返回值		:	实际清空的行数
	int Clear(int nRow,int nNum);


//==================================================================================================================================
//	运算符重载
//==================================================================================================================================
public:
	inline long double& operator ()(int row,int col){return m_pBuf[(row-m_nBase)*m_nCol+col-m_nBase];}
	inline long double operator ()(int row,int col) const{return m_pBuf[(row-m_nBase)*m_nCol+col-m_nBase];}
	
//==================================================================================================================================
//	调试功能
//==================================================================================================================================
public:

	void Format(CString& szText,int nLen,int nDot);	//将矩阵的各个元素输出到字符串中
//	void Parse(CString& szText);					//从字符串中读取矩阵的各个元素

	void Save(CFile& file,int nLen,int nDot);
	void DBG(CFile& file,LPCTSTR szText);
	void InitTestValue();



};

#endif 