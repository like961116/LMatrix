
#include "LMatrix.h"
#include <math.h>

//==================================================================================================================================
//
//	构造函数、析构函数
//
//==================================================================================================================================

LMatrix::LMatrix()
{
	m_nRow = m_nCol = 0;
	m_pBuf = NULL;
	m_fZero = 0;
}

LMatrix::LMatrix(int row,int col) 
{
	m_fZero = 0;
	
	//行列数不能小于0
	row=row<0?0:row;
	col=col<0?0:col;

	m_nRow = row;
	m_nCol = col; 

	//分配存储空间
	if (row*col!=0) 
	{
		m_pBuf = (long double*)malloc(row*col*sizeof(long double));
		memset(m_pBuf,0,m_nRow*m_nCol*sizeof(long double));
	}
}

LMatrix::LMatrix(LMatrix& m)
{
	m_nRow = m.GetRow();
	m_nCol = m.GetCol();
	m_fZero= m.GetZero();

	int nBufSize = m_nRow*m_nCol*sizeof(long double);
	m_pBuf = (long double*)malloc(nBufSize);
	memmove(m_pBuf,m.GetBuf(),nBufSize);
}

LMatrix& LMatrix::operator =(LMatrix& m)
{
	m_nRow = m.GetRow();
	m_nCol = m.GetCol();
	m_fZero= m.GetZero();

	int nBufSize = m_nRow*m_nCol*sizeof(long double);
	m_pBuf = (long double*)malloc(nBufSize);
	memmove(m_pBuf,m.GetBuf(),nBufSize);
	
	return *this;
}

LMatrix::~LMatrix()
{
	if (m_pBuf!=NULL) 
	{ 
		free(m_pBuf); 
		m_pBuf=NULL; 
	}
}

LMatrix LMatrix::I(int r)
{
	LMatrix m(r,r);
	memset(m.GetBuf(),0,r*r*sizeof(long double));
	for (int i=1;i<=m.GetRow();i++)
	{
		m(i,i)=1;
	}
	return m;
}


//==================================================================================================================================
//
//	属性操作
//
//==================================================================================================================================
int LMatrix::GetRow() { return m_nRow; }
int LMatrix::GetCol() { return m_nCol; }
long double LMatrix::GetValue(int row,int col) 
{ 
	return m_pBuf[(row-1)*m_nCol+col-1]; 
}
long double LMatrix::GetZero()  { return m_fZero; }
long double * LMatrix::GetBuf() { return m_pBuf; }
long double * LMatrix::GetRowAddr(int row)
{
	if(row>m_nRow||row<=0) return NULL;

	return m_pBuf+(row-1)*m_nCol;
}
//	=========================
//	函数名称	:	SetRow
//	函数功能	:	设置矩阵的行数
//	row			:	矩阵的新行数
//	注			:	新增元素初始化为0
void LMatrix::SetRow(int row) 
{ 
	if(row<0) return;

	if(row>m_nRow)	//扩展矩阵，由于m_nRow>=0，又row>m_nRow，所以row>0
	{
		int nMemLen = m_pBuf==NULL?0:_msize(m_pBuf);
		int nWantedLen = row*m_nCol*sizeof(long double);

		if(nMemLen<nWantedLen)	//需要重新分配存储空间
		{
			long double * p = (long double*)malloc(nWantedLen);
			if(p==NULL) return;	//存储空间不足，无法扩展矩阵
			
			//成功分配空间
			memcpy(p,m_pBuf,m_nRow*m_nCol*sizeof(long double));
			free(m_pBuf);
			m_pBuf = p;
		}
		
		memset(m_pBuf+m_nRow*m_nCol,0,(row-m_nRow)*m_nCol);

		m_nRow = row;
	}

	else	// if(row<=m_nRow)	//缩减矩阵
	{
		m_nRow = row; 
	}
}


void LMatrix::SetCol(int col)
{
	if(col<0) return;

	else if(col<m_nCol)	//列数减小，数据需要向前移动
	{
		for (int i=1;i<m_nRow;i++)	//从第二行开始每行前移
		{
			memmove(m_pBuf+i*col,m_pBuf+i*m_nCol,col*sizeof(long double));			
		}
		m_nCol = col;
	}

	else if(col>m_nCol)	//列数增加，数据需要向后移动，还需要重新分配空间
	{
		int nMemLen = m_pBuf==NULL?0:_msize(m_pBuf);
		int nWantedLen = m_nRow*col*sizeof(long double);

		if(nMemLen<nWantedLen)	//需要重新分配存储空间
		{
			long double * p = (long double*)malloc(nWantedLen);
			if(p==NULL) return;	//存储空间不足，无法扩展矩阵
			
			//成功分配空间，首先将数据搬到新位置
			memcpy(p,m_pBuf,m_nRow*m_nCol*sizeof(long double));
			free(m_pBuf);
			m_pBuf = p;			
		}
		
		//从最后一行开始每行后移
		for (int i=m_nRow-1;i>=0;i--)
		{
			memmove(m_pBuf+i*col,m_pBuf+i*m_nCol,m_nCol*sizeof(long double));			
		}

		//将新增元素的值置为0
		for (i=0;i<m_nRow;i++)
		{
			memset(m_pBuf+i*m_nCol,0,(col-m_nCol)*sizeof(long double));
		}

		m_nCol = col;
	}

	else //if(col==m_nCol)
	{
	}
}

void LMatrix::SetValue(int row,int col,long double v)
{
	m_pBuf[(row-1)*m_nCol+col-1] = v;
}

void LMatrix::SetZero(long double z)
{
	m_fZero = z;
}


//==================================================================================================================================
//
//	行列操作
//
//==================================================================================================================================

//	========================================
//	函数名称	:	DelRows
//	函数功能	:	删除矩阵中的若干行
//	nStartRow	:	需要删除的起始行，从1开始
//	nRows		:	需要删除的行数，nRows==-1表示删除后续所有行
//	返回值		:	删除的行数
int LMatrix::DelRows(int nStartRow,int nRows)
{
	if(nStartRow>m_nRow||m_nRow==0 ) return 0;
	else if(nStartRow<=0) nStartRow=1;

	if(nRows<0) nRows=m_nRow-nStartRow+1;
	else if(nRows>m_nRow-nStartRow+1) nRows=m_nRow-nStartRow+1;
	
	//数据搬家
	memmove(m_pBuf+(nStartRow-1)*m_nCol,m_pBuf+(nStartRow-1+nRows)*m_nCol,(m_nRow+1-nStartRow-nRows)*m_nCol*sizeof(long double));

	//多余的空间清零
	memset(m_pBuf+(m_nRow-nRows)*m_nCol,0,nRows*m_nCol*sizeof(long double));

	m_nRow -= nRows;

	return nRows;
}

int LMatrix::DelCols(int nStartCol,int nCols)
{
	if(nStartCol>m_nCol||m_nCol==0) return 0;
	else if(nStartCol<=0) nStartCol=1;

	if(nCols>m_nCol-nStartCol+1) nCols=m_nCol-nStartCol+1;

	//每一行删除列数之后的数据前移
	for(int i=0;i<m_nRow;i++)
	{
		memmove(m_pBuf+i*m_nCol+nStartCol+nCols-1,m_pBuf+i*m_nCol+nStartCol-1,(m_nCol-nStartCol-nCols+1)*sizeof(long double));
	}

	SetCol(m_nCol-nCols);

	return nCols;
}

int LMatrix::DelRow(int nRow)
{
	return DelRows(nRow,1);
}


int LMatrix::DelCol(int nCol)
{
	return DelCols(nCol,1);
}

//	=============================================
//	函数名称	:	InsertRows
//	函数功能	:	插入若干行
//	nPos		:	插入到第nPos行之前，nPos==1表示插入到第一行之前，nPos<=0表示插入到末尾
//					
LMatrix& LMatrix::InsertRows(LMatrix &m1,int nPos)
{
	if (nPos<=0||nPos>m_nRow) nPos=m_nRow+1; 

	LMatrix m(m1);
	m.SetCol(this->GetCol());

	//1)首先扩充矩阵的行数
	SetRow(GetRow()+m.GetRow());

	//2)数据搬家
	memmove(this->GetRowAddr(nPos+m.GetRow()),this->GetRowAddr(nPos),(m_nRow-m.GetRow()-nPos+1)*m_nCol*sizeof(long double));
	memmove(this->GetRowAddr(nPos),m.GetBuf(),m.GetRow()*m.GetCol()*sizeof(long double));

	return *this;
}

//	=============================================
//	函数名称	:	InsertCols
//	函数功能	:	向当前矩阵中插入若干列数据
//	nPos		:	插入到第nPos列之前，nPos=1表示插入到第一列之前
//					nPos<=0或nPos>m_nCol表示插入到最后一列
LMatrix& LMatrix::InsertCols(LMatrix &m1,int nPos)
{
	if (nPos<=0||nPos>m_nCol) nPos=m_nCol+1; 

	LMatrix m(m1);
	m.SetRow(this->GetRow());

	//1)首先扩充矩阵的列数
	SetCol(GetCol()+m.GetCol());

	//2)数据搬家
	for(int i=1;i<=m_nRow;i++)
	{
		memmove(GetRowAddr(i)+nPos-1+m.GetCol(),GetRowAddr(i)+nPos-1,(m_nCol-nPos-m.GetCol()+1)*sizeof(long double));
		memmove(GetRowAddr(i)+nPos-1,m.GetRowAddr(i),m.GetCol()*sizeof(long double));
	}
	return *this;
}

LMatrix LMatrix::Sub(int sr,int sc,int er,int ec)
{
	int r,c;

	//首先确保起止行列数 合法
	if(er<sr) { r=er; er=sr; sr=r; }
	if(ec<sc) { c=ec; ec=sc; sc=c; }

	if (sr<=0) sr=1;
	if (sc<=0) sc=1;

	if (er>m_nRow) er=m_nRow;
	if (ec>m_nCol) ec=m_nCol;

	//生成符合子矩阵尺寸的新矩阵
	r = er-sr+1;
	c = ec-sc+1;
	LMatrix m(r,c);

	//数据搬家
	for (int i=0;i<m.GetRow();i++)
	{
		memcpy(m.GetBuf()+i*m.GetCol(),m_pBuf+(sr+i-1)*m_nCol+sc-1,c*sizeof(long double));
	}

	return m;
}

LMatrix LMatrix::Rows(int nStartRow,int nRows)	//取矩阵的nStartRow行开始的nRows行
{
	return Sub(nStartRow,1,nStartRow+nRows-1,m_nCol);
}
LMatrix LMatrix::Cols(int nStartCol,int nCols)	//取矩阵的nStartCol列开始的nCols列
{
	return Sub(1,nStartCol,nStartCol+nCols-1,m_nRow);
}

LMatrix LMatrix::Row(int row) {	return Sub(row,1,row,m_nCol); }
LMatrix LMatrix::Col(int col) { return Sub(1,col,m_nRow,col); }


//==================================================================================================================================
//
//	运算符重载
//
//==================================================================================================================================

LMatrix LMatrix::operator +(LMatrix &m)
{
	LMatrix r;
	if(m.GetRow()!=m_nRow||m.GetCol()!=m_nCol) return r;
	
	r = *this;

	for (int i=1;i<=m_nRow;i++)
		for (int j=1;j<=m_nCol;j++)
			r(i,j) = GetValue(i,j)+m(i,j);

	return r;
}


LMatrix LMatrix::operator -(LMatrix &m)
{
	LMatrix r;
	if(m.GetRow()!=m_nRow||m.GetCol()!=m_nCol) return r;
	
	r = *this;

	for (int i=1;i<=m_nRow;i++)
		for (int j=1;j<=m_nCol;j++)
			r(i,j) = GetValue(i,j)-m(i,j);

	return r;
}



LMatrix LMatrix::operator *(LMatrix &m)
{
	LMatrix r;
	if(m.GetRow()!=m_nCol) return r;
	
	r = *this;
	r.SetCol(m.GetCol());

	for (int i=1;i<=r.GetRow();i++)
		for (int j=1;j<=r.GetCol();j++)
		{
			long double s = 0.0;
			for (int k=1;k<=GetCol();k++)
			{
				s += GetValue(i,k)*m(k,j);
			}
			r(i,j) = s;
		}

	return r;
}
LMatrix LMatrix::operator +(long double f)
{
	int i,j;
	LMatrix r = *this;
	int nRow = GetRow();
	int nCol = GetCol();

	for (i=1;i<=nRow;i++)
		for(j=1;j<=nCol;j++)
			r(i,j)=GetValue(i,j)+f;
	return r;
}
LMatrix LMatrix::operator -(long double f)
{
	int i,j;
	LMatrix r = *this;
	int nRow = GetRow();
	int nCol = GetCol();

	for (i=1;i<=nRow;i++)
		for(j=1;j<=nCol;j++)
			r(i,j)=GetValue(i,j)-f;
	return r;
}
LMatrix LMatrix::operator *(long double f)
{
	int i,j;
	LMatrix r = *this;
	int nRow = GetRow();
	int nCol = GetCol();

	for (i=1;i<=nRow;i++)
		for(j=1;j<=nCol;j++)
			r(i,j)=GetValue(i,j)*f;
	return r;
}

LMatrix LMatrix::operator /(long double f)
{
	int i,j;
	LMatrix r = *this;
	int nRow = GetRow();
	int nCol = GetCol();

	for (i=1;i<=nRow;i++)
		for(j=1;j<=nCol;j++)
			r(i,j)=GetValue(i,j)/f;
	return r;
}

long double& LMatrix::operator ()(int row,int col)
{
	return m_pBuf[(row-1)*m_nCol+col-1];
}
long double LMatrix::operator ()(int row,int col) const
{
	return m_pBuf[(row-1)*m_nCol+col-1];
}


//==================================================================================================================================
//
//	矩阵运算
//
//==================================================================================================================================
int LMatrix::Inv()
{
	int i,j,k;
	//不是方阵或行列式等于0，则不可逆
	long double fDet = Det();
	if( m_nRow!=m_nCol || fDet==0 || fabs(fDet)<=m_fZero ) return 0;

	//单位阵
	LMatrix m = I(m_nRow);

	//变化为三角阵
	for(i=1;i<=m_nRow-1;i++)
	{
		//如果元素[i,i]==0，则后续各行i列中 必然有一个不为0的，否则行列式==0
		//此时将该行加到第i行上
		if(fabs(GetValue(i,i))<=m_fZero)
		{
			for(j=i+1;j<=m_nRow;j++)
			{
				if(fabs(GetValue(j,i))>m_fZero) 
				{
					for (k=1;k<=m_nRow;k++)
					{
						SetValue(i,k,GetValue(j,k)+GetValue(i,k));
						m(i,k) = m(j,k)+m(i,k);
					}
					break;
				}
			}
		}

		//变为三角阵
		for(j=i+1;j<=m_nRow;j++)
		{
			if( fabs(GetValue(j,i))>m_fZero )	//GetValue(j,i)!=0
			{
				long double r = GetValue(j,i)/GetValue(i,i);
				for (k=i+1;k<=m_nCol;k++)
				{
					SetValue(j,k,GetValue(j,k)-GetValue(i,k)*r);
				}
				SetValue(j,i,0);
				for (k=1;k<=m_nCol;k++)
				{
					m(j,k) = m(j,k)-m(i,k)*r;
				}
			}
		}
	}

	//变为对角阵
	for (i=m_nRow;i>1;i--)
	{
		for (j=i-1;j>=1;j--)
		{
			if( fabs(GetValue(j,i))>m_fZero )	//GetValue(j,i)!=0
			{
				long double r = GetValue(j,i)/GetValue(i,i);
				for (k=1;k<=m_nCol;k++)
				{
//					SetValue(j,k,GetValue(j,k)*r-GetValue(i,k));
					m(j,k) =m(j,k)-m(i,k)*r;
				}
//				SetValue(j,i,0);
			}
		}
	}

	for (i=1;i<=m.GetRow();i++)
	{
		for (j=1;j<=m.GetCol();j++)
		{
			m(i,j) = m(i,j)/GetValue(i,i);
		}
	}

	*this = m;	

	return 1;
}

int LMatrix::Trans()
{
	LMatrix m(m_nCol,m_nRow);
	for (int i=1;i<=m_nRow;i++)
		for (int j=1;j<=m_nCol;j++)
			m(j,i)=GetValue(i,j);
	*this = m;
	return 0;
}

long double LMatrix::Det()
{
	int i,k,j;
	LMatrix m = *this;
	
	for(i=1;i<=m_nRow-1;i++)
	{
		//如果元素[i,i]==0，则后续各行i列中 必然有一个不为0的，否则行列式==0
		//此时将该行加到第i行上
		if(fabs(m.GetValue(i,i))<=m_fZero)
		{
			for(j=i+1;j<=m_nRow;j++)
			{
				if(fabs(m(j,i))>m_fZero) 
				{
					for (k=1;k<=m_nRow;k++)
					{
						m(i,k)=m(j,k)+m(i,k);
					}
					break;
				}
			}
		}
		
		for(j=i+1;j<=m_nRow;j++)
		{
			if(fabs(m(j,i))>m_fZero)
			{
				long double r = m(j,i)/m(i,i);
				for (k=i+1;k<=m_nCol;k++)
				{
					m(j,k)=m(j,k)-m(i,k)*r;
				}
			}
		}
	}

	long double s = 1;

	for (i=1;i<=m_nRow;i++) s *= m(i,i);

	return s;
}

//	=========================
//	函数名称	:	Cholesky
//	函数功能	:	返回矩阵的Cholesky分解
//	Cholesky分解:	C是实对称正定矩阵，则必然存在下三角矩阵T，使得C=TT'
//					该函数返回的就是T
LMatrix LMatrix::Cholesky()
{
	int i,j,k;
	long double s;

	LMatrix t(m_nRow,m_nCol);

	for (i=1;i<=m_nRow;i++)
	{
		//1)计算tii
		s=0;
		for(k=1;k<i;k++) s+=t(i,k)*t(i,k);
		t(i,i) = sqrt(GetValue(i,i)-s);

		//2)计算tji
		for(j=i+1;j<=m_nRow;j++)
		{
			s=0;
			
			for(k=1;k<i;k++)
			{
				s+= t(i,k)*t(j,k);
			}
			t(j,i)=(GetValue(i,j)-s)/t(i,i);
		}
	}

	//下面把上三角置为0
	for (i=1;i<=m_nRow;i++)
		for (j=i+1;j<=m_nRow;j++)
			t(i,j) =0.0;

//	LStringA s2 = "刚刚计算得出的cholesky矩阵为t=：\r\n";
//	s2.SaveFile("c:\\simu.txt");
//	t.Output("c:\\simu.txt",8);
	return t;
}

bool LMatrix::IsPDefinite()
{
	for (int i=1;i<=m_nRow;i++)
	{
		LMatrix m = Sub(1,1,i,i);
		if(m.Det()<=0) return false;
	}
	return true;
}

bool LMatrix::IsHPDefinite()
{
	for (int i=1;i<=m_nRow;i++)
	{
		LMatrix m = Sub(1,1,i,i);
		if(m.Det()<0) return false;
	}
	return true;
}


//==================================================================================================================================
//
//	数学运算
//
//==================================================================================================================================
LMatrix LMatrix::Abs()
{
	LMatrix r(m_nRow,m_nCol);

	int i,j;
	for(i=1;i<=m_nRow;i++)
		for(j=1;j<=m_nCol;j++)
			r(i,j)=fabs(GetValue(i,j));

	return r;
}


//==================================================================================================================================
//	统计运算
//==================================================================================================================================

//	求列向量的均值，返回一个行向量
LMatrix LMatrix::Average()
{
	LMatrix m(1,m_nCol);
	for (int i=1;i<=m_nCol;i++)
	{
		long double s = 0.0;
		
		for(int j=1;j<=m_nRow;j++) s+=GetValue(j,i);

		m(1,i) = s/m_nRow;
	}

	return m;
}

LMatrix LMatrix::Var()
{
	int i,j;
	long double s;
	LMatrix v(1,m_nRow);
	LMatrix a = Average();

	for (i=1;i<=m_nCol;i++)
	{
		s=0;
		for(j=1;j<=m_nRow;j++) s+= (GetValue(j,i)-a(1,i))*(GetValue(j,i)-a(1,i));
		s/=(m_nRow-1);
		v(1,i)=s;
	}
	return v;
}
LMatrix LMatrix::SVar()
{
	LMatrix v=Var();
	for (int i=1;i<=v.GetCol();i++)
	{
		v(1,i)=sqrt(v(1,i));
	}
	return v;
}

LMatrix LMatrix::AvgAndSVar()
{
	LMatrix a = Average();
	LMatrix v = SVar();
	a.InsertRows(v,-1);
	return a;
}

//	把矩阵的每一列视为一个向量，求这些向量的协方差矩阵
//	返回值	:	协方差矩阵
//				如果向量长度（行数）<=1，则无法求协方差矩阵，此时返回的矩阵对角元素为-1
LMatrix LMatrix::Cov()
{
	int i,j,k;
	long double s;
	
	LMatrix c(m_nCol,m_nCol);
	if (m_nCol<=0) return c;		//没有向量，直接返回
	if (m_nRow<=1)					//向量长度至少为2
	{
		c(1,1) = -1.0;
		return c;
	}

	LMatrix a = Average();				//均值向量

	for (i=1;i<=c.GetRow();i++)
		for(j=1;j<=c.GetCol();j++)
		{
			s = 0;
			for (k=1;k<=m_nRow;k++) s+=(GetValue(k,i)-a(1,i))*(GetValue(k,j)-a(1,j));
			s /= (m_nRow-1);
			c(i,j) = s;
		}

	return c;
}

LMatrix LMatrix::Rel()
{
	int i,j;
	LMatrix r=Cov();
	if(r(1,1)==-1.0) return r;	

	for(i=1;i<=r.GetRow();i++)
	{
		for(j=1;j<=r.GetCol();j++) 
		{
			if(j!=i) 
			{
				long double f = sqrt(r(i,i)*r(j,j));
				r(i,j)=r(i,j)/f;	
			}
		}
	}

	for(i=1;i<=r.GetRow();i++) r(i,i)=1.0;

	return r;
}



long double LMatrix::Measure0()
{
	long double s=0;
	for (int i=1;i<=GetRow();i++)
		for(int j=1;j<=GetCol();j++)
			s+=GetValue(i,j);
	return s;
}

long double LMatrix::Measure1()
{
	long double s=0;
	for (int i=1;i<=GetRow();i++)
		for(int j=1;j<=GetCol();j++)
			s+=fabs(GetValue(i,j));
	return s;
}

long double LMatrix::Measure2()
{
	long double s=0;
	for (int i=1;i<=GetRow();i++)
		for(int j=1;j<=GetCol();j++)
			s+=GetValue(i,j)*GetValue(i,j);
	return s;
}

//==================================================================================================================================
//
//	排序算法
//
//==================================================================================================================================

//对第col列进行快速排序，如果col<=0，则对每一列单独进行排序
LMatrix& LMatrix::Sort(int direction,int col)
{
	if(col>0) Sort_step(direction,col,1,m_nRow);
	else 
	{
		for (int i=1;i<=GetCol();i++) Sort_step(direction,i,1,m_nRow);
	}
	return *this;
}


LMatrix& LMatrix::Sort_step(int direction,int col,int startrow,int endrow)
{
	int i=startrow,j=endrow;

	if(endrow>startrow)
	{
		long double key = GetValue(i,col);	//先把中间的数据保存起来

		for (;i!=j;)
		{
			for(;j>i&&GetValue(j,col)*direction>=key*direction;j--);
			
			if(j>i) 
			{
				(*this)(i,col)=(*this)(j,col);

			}
			for(;i<j&&GetValue(i,col)*direction<=key*direction;i++);
			
			if(i<j) (*this)(j,col)=(*this)(i,col);
		}

		(*this)(i,col) = key;

		Sort_step(direction,col,startrow,i-1);
		Sort_step(direction,col,i+1,endrow);
	}

	return *this;
}


LMatrix LMatrix::SortIndex(int direction)
{
	int i,j;
	LMatrix s=*this;
	LMatrix m(GetRow(),GetCol());

	for(i=1;i<=GetRow();i++)
		for(j=1;j<=GetCol();j++)
			m(i,j)=i;

	for (i=1;i<=GetCol();i++) s.SortIndex_step(direction,i,1,GetRow(),m);
	return m;
}

LMatrix& LMatrix::SortIndex_step(int direction,int col,int startrow,int endrow,LMatrix &m)
{
	int i=startrow,j=endrow;

	if(endrow>startrow)
	{
		double key1 = GetValue(i,col);	//先把中间的数据保存起来
		double key2 = m(i,col);

		for (;i!=j;)
		{
			for(;j>i&&GetValue(j,col)*direction>=key1*direction;j--);
			
			if(j>i) 
			{
				(*this)(i,col)=(*this)(j,col);
				m(i,col)=m(j,col);
			}

			for(;i<j&&GetValue(i,col)*direction<=key1*direction;i++);
			
			if(i<j)
			{
				(*this)(j,col)=(*this)(i,col);
				m(j,col)=m(i,col);
			}
		}

		(*this)(i,col) = key1;
		m(i,col)=key2;

		SortIndex_step(direction,col,startrow,i-1,m);
		SortIndex_step(direction,col,i+1,endrow,m);
	}

	return *this;
}

long double LMatrix::Max()
{
	long double key = GetValue(1,1);
	for(int i=1;i<=GetRow();i++)
	{
		for(int j=1;j<=GetCol();j++)
		{
			if(GetValue(i,j)>key) key = GetValue(i,j);
		}
	}
	return key;
}

//==================================================================================================================================
//
//	调试功能
//
//==================================================================================================================================
void LMatrix::Output(char * szFileName,int nLen)
{
	int i,j;
	DWORD dwWritten;

	LStringA szRet,dbg;
	
	LStringA s;

	for (i=1;i<=m_nRow;i++)
	{
		for(j=1;j<=m_nCol;j++)
		{
			s.SetBufB((double)GetValue(i,j),nLen);
			s.ConnectB(' ');
			s.ConnectB('\t');
			szRet.ConnectB(s);
		}
		szRet.ConnectB("\r\n");
	}

	HANDLE hFile = ::CreateFile(szFileName,GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile!=INVALID_HANDLE_VALUE)
	{
		::SetFilePointer(hFile,0,NULL,FILE_END);
		::WriteFile(hFile,szRet.GetBuf(),szRet.GetLenB(),&dwWritten,NULL);
		::CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
}


void LMatrix::Load(char * szFileName)				//从szFileName中读取矩阵的各个元素
{
	LStringA szFile;
	szFile.LoadFile("c:\\a.txt");
	LList pRows = szFile.SplitB("\r\n",-1);
	
	//删除空行
	LListNode * pNode = pRows.GetHead();
	LListNode * p;
	for(;pNode!=NULL;)
	{
		if(((LStringA *)(pNode->GetObj()))->GetLenB()<=0) 
		{
			p=pNode;
			pNode=pNode->GetNext();
			pRows.Delete(p);
		}
		else
		{
			pNode=pNode->GetNext();
		}
	}

	//设置行列数
	SetRow(pRows.GetNodeNum());
	pNode = pRows.GetHead();
	LStringA * pRow= (LStringA*)(pNode->GetObj());
	pRow->Trim("\t",-1).Trim(" ",-1);

	LList pList1 = pRow->Trim(" ",-1).Trim("\t",-1).SplitB(" ",-1);
	SetCol(pList1.GetNodeNum());

	int i=1;
	for(;pNode!=NULL;i++,pNode=pNode->GetNext())
	{
		LList pCols = ((LStringA*)(pNode->GetObj()))->Trim("\t",-1).Trim(" ",-1).SplitB(" ",-1);
		LListNode * pColNode = pCols.GetHead();
		
		int j=1;
		for(;pColNode!=NULL;pColNode=pColNode->GetNext(),j++)
		{
			double f = ((LStringA*)(pColNode->GetObj()))->Trim("\t",-1).Trim(" ",-1).ToFloat();
			SetValue(i,j,f);
		}

	}

}