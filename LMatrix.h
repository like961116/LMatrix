#ifndef __LEC_MATRIX_H__
#define __LEC_MATRIX_H__

class LMatrix 
{

//==================================================================================================================================	
//	属性
//==================================================================================================================================
private:
	long double * m_pBuf;	//保存矩阵数据
	int m_nRow;					//矩阵的行数
	int m_nCol;					//矩阵的列数
	long double m_fZero;	//零值，矩阵运算中，绝对值小于该值的数字认为是0，一般默认为0

//==================================================================================================================================
//	构造函数、析构函数
//==================================================================================================================================
public:
	LMatrix();											//初始化为0行0列的矩阵
	LMatrix(int row,int col);					//初始化为row行col列的矩阵，数据全为0
	LMatrix(LMatrix& m);						//初始化为与m完全相等的矩阵
	LMatrix& operator =(LMatrix& m);	//初始化为与m完全相等的矩阵

	~LMatrix();							//析构函数
	LMatrix I(int r);						//创建一个r行r列的单位阵
	
//==================================================================================================================================
//	属性操作
//==================================================================================================================================
public:
	int GetRow();	//读取行数
	int GetCol();	//读取列数
	long double GetValue(int row,int col);	//读取row行col列处的元素值
	long double GetZero();					//读取矩阵的零值
	long double * GetBuf();					//返回缓冲区首地址
	long double * GetRowAddr(int row);		//返回第row行的首地址

	void SetRow(int row);	//设置行数，行数增加时，新增数据默认为0
	void SetCol(int col);	//设置列数，列数增加时，新增数据默认为0
	void SetValue(int row,int col,long double v);	//设置row行col列处的元素值
	void SetZero(long double z);					//设置零值

//==================================================================================================================================
//	行列操作
//==================================================================================================================================
public:
	int DelRows(int nStartRow,int nRows);	//从nStartRow行开始，共删除nRows行
	int DelCols(int nStartCol,int nCols);	//从nStartCol列开始，共删除nCols列

	int DelRow(int nRow);	//删除第nRow行
	int DelCol(int nCol);	//删除第nCol列

	LMatrix& InsertRows(LMatrix& m,int nPos);	//在第nPos行之前插入矩阵m，nPos之后的行下移
	LMatrix& InsertCols(LMatrix& m,int nPos);	//在第nPos列之前插入矩阵m，nPos之后的列右移
	
	LMatrix Sub(int sr,int sc,int er,int ec);	//取矩阵的sr至er行，sc至ec列的子阵
	LMatrix Rows(int nStartRow,int nRows);		//取矩阵的nStartRow行开始的nRows行
	LMatrix Cols(int nStartCol,int nCols);		//取矩阵的nStartCol列开始的nCols列
	LMatrix Row(int row);	//取矩阵的第row行
	LMatrix Col(int col);	//取矩阵的第col列

//==================================================================================================================================
//	运算符重载
//==================================================================================================================================
public:
	LMatrix operator +(LMatrix &m);		
	LMatrix operator -(LMatrix &m);
	LMatrix operator *(LMatrix &m);
	LMatrix operator +(long double f);	
	LMatrix operator -(long double f);
	LMatrix operator *(long double f);
	LMatrix operator /(long double f);
	long double& operator ()(int row,int col);
	long double operator ()(int row,int col) const;
	
//==================================================================================================================================
//	矩阵运算
//==================================================================================================================================
public:
	int Inv();			//逆矩阵，返回1表示可逆，返回0表示不可逆
	int Trans();		//转置
	long double Det();	//行列式
	LMatrix Cholesky();	//得到Cholesky分解的下三角矩阵T，一般协方差矩阵C可以分解为TT'
	

	bool IsPDefinite();		//判断矩阵是否是正定矩阵
	bool IsHPDefinite();	//判断矩阵是否是半正定矩阵
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


//==================================================================================================================================
//	调试功能
//==================================================================================================================================
public:
	void Output(char * szFileName,int nLen);	//将矩阵的各个元素输出到文件szFileName中
	void Load(char * szFileName);					//从szFileName中读取矩阵的各个元素

public:
	

};

#endif 