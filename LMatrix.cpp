
#include "LMatrix.h"
#include <math.h>



LMatrix::LMatrix() { m_nBase=1;m_fZero=0.0; }
LMatrix::LMatrix(int row,int col) : LMatrixStorage(row,col) { m_nBase=1;m_fZero=0.0;  }
LMatrix::LMatrix(LMatrix& m) : LMatrixStorage(m) { m_nBase=1;m_fZero=m.m_fZero; }
LMatrix::LMatrix(LMatrixStorage& m) : LMatrixStorage(m) { m_nBase=1;m_fZero=0.0; }
LMatrix& LMatrix::operator=(LMatrix& m) 
{
	if(this==&m) return *this;
	LMatrixStorage::operator=(m);
	m_fZero=m.m_fZero;
	return *this;
}


LMatrix& LMatrix::operator=(LMatrixStorage& m) 
{
	if(this==&m) return *this;
	LMatrixStorage::operator=(m);
	return *this;
}

LMatrix LMatrix::I(int row)
{
	LMatrix m(row,row);
	for(int i=1;i<=row;i++) m(i,i)=1;
	m.SetZero(GetZero());
	return m;
}

bool LMatrix::operator==(LMatrix& m)
{
	if(this==&m) return TRUE;
	else if(GetRow()!=m.GetRow()||GetCol()!=m.GetCol()) return FALSE;
	LMatrix m1 = *this-m;
	for(int i=1;i<=m1.GetRow();i++)
		for(int j=1;j<=m1.GetCol();j++)
			if(!IsZero(m1(i,j))) return false;
	return true;
}

bool LMatrix::operator!=(LMatrix& m)
{
	return !(*this==m);
}

//	======================
//	Name	:	operator +=
//	Func	:	add another matrix to this one
//	m		:	another matrix
//	return	:	reference of this matrix
//	note	:	if two matrices have different sizes, the return value is matrix with 0 row and 0 col
LMatrix& LMatrix::operator +=(LMatrix& m)
{
	// if two matrices have different size, result an empty matrix means "can not add"
	if (m.GetRow()!=m_nRow||m.GetCol()!=m_nCol)  m_nRow=m_nCol=0;
	else for(int i=1;i<=m_nRow;i++)  for(int j=1;j<=m_nCol;j++)  (*this)(i,j) += m(i,j);
	return *this;
}

//	======================
//	Name		:	operator -=
//	Func		:	subtract  another matrix from this one
//	m			:	another matrix
//	return		:	reference of this matrix
//	note		:	if two matrices have different sizes, the return value is matrix with 0 row and 0 col
LMatrix& LMatrix::operator -=(LMatrix& m)
{
	// if two matrices have different size, result an empty matrix means "can not subtract"
	if (m.GetRow()!=m_nRow||m.GetCol()!=m_nCol)  m_nRow=m_nCol=0;
	else for(int i=1;i<=m_nRow;i++) for(int j=1;j<=m_nCol;j++)  (*this)(i,j) -= m(i,j);
	return *this;
}

//	======================
//	Name	:	operator *=
//	Func		:	multiplied by another matrix
//	m1		:	another matrix
//	return	:	reference of this matrix
//	note		:	if two matrices are not satisfy the condition of matrix multiply, then return matrix with 0 row and 0 col
//	
LMatrix& LMatrix::operator *=(LMatrix& m)
{
	long double s;

	if (m.GetRow()!=m_nCol)  m_nRow=m_nCol=0;
	else 
	{
		LMatrix m1=*this;
		LMatrix m2=m;
		this->SetCol(m.GetCol());

		for(int i=1;i<=m_nRow;i++) 
		{
			for(int j=1;j<=m_nCol;j++)  
			{
				s=0;
				for(int k=1;k<=m1.GetCol();k++)
				{
					s +=m1(i,k) * m2(k,j);
				}
				(*this)(i,j)=s;
			}
		}
	}
	return *this;
}

//	======================
//	Name	:	operator /=
//	Func		:	multiplied by the inverse of another matrix
//	m			:	another matrix
//	return	:	reference of this matrix
//	note		:	if m is not inversable, or two matrices are not satisfy the condition of matrix multiply, then return matrix with 0 row and 0 col	
LMatrix& LMatrix::operator /=(LMatrix& m)
{
	LMatrix m1 = m^(-1);		// devide m ==> multiply inverse of m
	if(m1.GetRow()==0||m1.GetCol()==0) 	m_nRow=m_nCol=0;	//m is not inversable

	(*this)*=m1;
	return *this;
}

//	======================
//	Name	:	operator *=
//	Func		:	multiply the matrix with a number
//	f			:	multiplier
//	return	:	reference of this matrix
LMatrix& LMatrix::operator *=(long double f)
{
	for(int i=1;i<=m_nRow;i++)
		for(int j=1;j<=m_nCol;j++)
			(*this)(i,j)*=f;
	return *this;
}

//	======================
//	Name	:	operator /=
//	Func		:	divide the matrix with a number
//	f			:	divisor
//	return	:	reference of this matrix
//	note		:	if f equal 0, return matrix with 0 row and 0 col
LMatrix& LMatrix::operator /=(long double f)
{
	if(IsZero(f)) m_nRow=m_nCol=0;
	else
	{
		for(int i=1;i<=m_nRow;i++)
			for(int j=1;j<=m_nCol;j++)
				(*this)(i,j)/=f;
	}
	return *this;
}

//	======================
//	Name	:	operator ^=
//	Func	:	power of the matrix
//	i		:	exponent
//	return	:	reference of this matrix
//	note	:	if i < 0, means power of inverse matrix
LMatrix& LMatrix::operator ^=(int i)
{
	int j,k;

	if(i==0)		//identity matrix
	{
		if(m_nRow!=m_nCol||m_nRow==0||m_nCol==0) m_nRow=m_nCol=0;
		else
		{
			*this=LMatrixStorage::I(m_nRow);

		}
		return *this;
	}
	else if(i<-1)	//power of inverse matrix
	{
		i=0-i;
		(*this)^=-1;
		(*this)^=i;
		return *this;
	}
	else if(i==-1)	// inverse matrix
	{
		//only square matrix is inversable
		if( m_nRow!=m_nCol) m_nRow=m_nCol=0;

		//append an "identity matrix" after the last col of matrix
		LMatrix m = *this;
		m.InsertCols(I(m_nRow),m_nRow+1);


		//change to triangle matrix
		for(i=1;i<=m.GetRow();i++)
		{
			//if [i,i]==0, there has at least one row " j " whose element of column " i "  is not zero in the following rows, otherwise the  determinant  is zero.
			//add row j to row i, then [i,i]=[j,i] is not zero
			if(IsZero(m(i,i)))
			{
				for(j=i+1;j<=m.GetRow();j++)
				{
					if(!IsZero(m(j,i))) 
					{
						for (k=i;k<=m.GetCol();k++)
						{
							m(i,k) += m(j,k);
						}
						break;
					}
				}
				if(j>m.GetRow())	//[i,i]==0, and elements of column " i " of all the following rows are zero
				{							
					m_nRow=m_nCol=0;	//an invalid matrix, means "can not inversable"
				}
			}

			//change to triangle matrix
			for(j=i+1;j<=m_nRow;j++)
			{
				if(!IsZero(m(j,i)))	//
				{
					long double r = m(j,i)/m(i,i);
					for (k=i+1;k<=m.GetCol();k++)
					{
						m(j,k) -= m(i,k)*r;
					}
					m(j,i)=0;

				}

			}
		}

		//change to diagonal matrix
		for (i=m.GetRow();i>=1;i--)
		{
			for (j=i-1;j>=1;j--)
			{
				if( !IsZero(m(j,i)))	//
				{
					long double r = m(j,i)/m(i,i);
					for (k=i+1;k<=m.GetCol();k++)
					{
						m(j,k) -= m(i,k)*r;
					}
					m(j,i)=0;
				}

			}
		}

		for (i=1;i<=m.GetRow();i++)
		{
			for (j=m.GetRow()+1;j<=m.GetCol();j++)
			{
				m(i,j) /= m(i,i);
			}

		}

		*this = m.Cols(m.GetRow()+1,m.GetRow());

		return *this;	
	}
	else if(i>0) 
	{
		LMatrix m = *this;
		LMatrix res = I(GetRow());
		while(i)
		{
			if(i&1) res *= m;
			i>>=1;
			m *= m;
		}
		*this=res;
		return *this;
	}
	return *this;
}

LMatrix LMatrix::operator +(LMatrix &m)
{
	LMatrix r = *this;
	r+=m;
	return r;
}


LMatrix LMatrix::operator -(LMatrix &m)
{
	LMatrix r = *this;
	r-=m;
	return r;
}


LMatrix LMatrix::operator *(LMatrix &m)
{
	LMatrix r = *this;
	r*=m;
	return r;
}
	   
LMatrix LMatrix::operator /(LMatrix& m)
{
	LMatrix r=*this;
	r/=m;
	return r;
}

LMatrix LMatrix::operator *(long double f)
{
	LMatrix r = *this;
	r*=f;
	return r;
}

LMatrix LMatrix::operator /(long double f)
{
	LMatrix r = *this;
	r/=f;
	return r;
}

LMatrix LMatrix::operator^(int i)
{
	LMatrix r=*this;
	r^=i;
	return r;
}

//==================================================================================================================================
//
//	matrix operation
//
//==================================================================================================================================

LMatrix LMatrix::Trans()
{
	LMatrix m(m_nCol,m_nRow);
	for (int i=1;i<=m_nRow;i++)
		for (int j=1;j<=m_nCol;j++)
			m(j,i)=(*this)(i,j);
	return m;
}

long double LMatrix::Deter()
{
	int i,k,j;
	LMatrix m = *this;

	for(i=1;i<=m_nRow-1;i++)
	{
		//if [i,i]==0, there has at least one row " j " whose element of column " i "  is not zero in the following rows, otherwise the  determinant  is zero.
		//add row j to row i, then [i,i]=[j,i] is not zero
		if(IsZero(m(i,i)))
		{
			for(j=i+1;j<=m_nRow;j++)
			{
				if(!IsZero(m(j,i))) 
				{
					for (k=1;k<=m_nRow;k++)
					{
						m(i,k)+=m(j,k);
					}
					break;
				}
			}
		}
		

		for(j=i+1;j<=m_nRow;j++)
		{
			if(!IsZero(m(j,i)))
			{
				long double r = m(j,i)/m(i,i);
				for (k=i+1;k<=m_nCol;k++)
				{
					m(j,k)-=m(i,k)*r;
				}
				m(j,i)=0;
			}


		}

	}

	long double s = 1;

	for (i=1;i<=m_nRow;i++) s *= m(i,i);

	return s;
}

//	=======================
//	Name	:	Cholesky
//	Func	:	Cholesky decomposition
//	return	:	Lower Triangle Matrix T which satisfies equation M=T*T'
LMatrix LMatrix::Cholesky()
{
	int i,j,k;
	long double s;

	LMatrix t(m_nRow,m_nCol);

	for (i=1;i<=m_nRow;i++)
	{
		//1)calculate tii
		s=0;
		for(k=1;k<i;k++) s+=t(i,k)*t(i,k);
		t(i,i) = sqrt((*this)(i,i)-s);

		//2)计算tji
		for(j=i+1;j<=m_nRow;j++)
		{
			s=0;
			
			for(k=1;k<i;k++)
			{
				s+= t(i,k)*t(j,k);
			}
			t(j,i)=((*this)(i,j)-s)/t(i,i);
		}
	}

	//set upper triangle to zero
	for (i=1;i<=m_nRow;i++)
		for (j=i+1;j<=m_nRow;j++)
			t(i,j) =0.0;

	return t;
}
/*
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
*/

LMatrix LMatrix::_Elementary_Row_Transform_To_Upper_Triangle()
{

	int i,k,j;
	LMatrix m = *this;

	int nRows = m.GetRow();
	int nCols = m.GetCol();
	
	//	when the matrix have more rows than columns, after row transposition, the extra rows will be change to zero 
	if(nRows>nCols) 
	{
		int nNum = nRows-nCols;
		nRows=nCols;
		m.Clear(nRows+1,nNum);
	}
	for (i=1;i<nRows;i++)
	{
		//if [i,i]==0, there has at least one row " j " whose element of column " i "  is not zero in the following rows, otherwise the  determinant  is zero.
		//add row j to row i, then [i,i]=[j,i] is not zero
		if(IsZero(m(i,i)))
		{
			for(j=i+1;j<=m_nRow;j++)
			{
				if(!IsZero(m(j,i))) 
				{
					for (k=1;k<=m_nRow;k++)
					{
						m(i,k)+=m(j,k);
					}
					break;
				}
			}
		
			if(j>m_nRow) continue;	//means element [i,j] (j=i,i+1,...m_nRow) are all zero
		}	

		//Now,[i,i]!=0, set [i,j](j=i+1,i+2...)=0
		for(j=i+1;j<=nRows;j++)
		{
			if(!IsZero(m(j,i)))
			{
				long double r = m(j,i)/m(i,i);
				for (k=i+1;k<=m_nCol;k++)
				{
					m(j,k)-=m(i,k)*r;
				}
				m(j,i)=0;
			}

		}

	}

	return m;
}



bool LMatrix::IsSymmetric()
{
	if (m_nRow!=m_nCol) return false;
	for(int i=2;i<=m_nRow;i++)
	{
		for(int j=1;j<i;j++)
		{
			if(!IsZero((*this)(i,j)-(*this)(j,i)))
			{
				return false;
			}
		}
	}
	return true;
}

bool LMatrix::IsPositiveDefinite()
{
	if(!IsSymmetric()) return false;

	int i,k,j;
	LMatrix m = *this;

	for (i=1;i<=m.GetRow();i++)
	{
		//if [i,i]==0, the matrix is not positive definite
		if(m(i,i)<=0||IsZero(m(i,i))) return false;
		
		//Now,[i,i]>0, set [i,j](j=i+1,i+2...)=0
		for(j=i+1;j<=m.GetRow();j++)
		{
			if(!IsZero(m(j,i)))
			{
				long double r = m(j,i)/m(i,i);
				for (k=i+1;k<=m_nCol;k++)
				{
					m(j,k)-=m(i,k)*r;
				}
				m(j,i)=0;
			}

		}

	}

	return true;
}

bool LMatrix::IsNegativeDefinite()
{
	LMatrix m = *this;
	
	for(int i=1;i<=m_nRow;i++)
		for(int j=1;j<=m_nCol;j++)
			m(i,j) = 0-m(i,j);

	if(m.IsPositiveDefinite()) return true;
	else return false;

	//if(!IsSymmetric()) return false;

	//int i,k,j;
	//LMatrix m = *this;

	//for (i=1;i<=m.GetRow();i++)
	//{
	//	//if [i,i]==0, the matrix is not positive definite
	//	if(i%2==0)
	//	{
	//		if(m(i,i)<=0||IsZero(m(i,i))) return false;
	//	}
	//	else
	//	{
	//		if(m(i,i)>=0||IsZero(m(i,i))) return false;
	//	}

	//	//Now,[i,i]!=0, set [i,j](j=i+1,i+2...)=0
	//	for(j=i+1;j<=m.GetRow();j++)
	//	{
	//		if(!IsZero(m(j,i)))
	//		{
	//			long double r = m(j,i)/m(i,i);
	//			for (k=i+1;k<=m_nCol;k++)
	//			{
	//				m(j,k)-=m(i,k)*r;
	//			}
	//			m(j,i)=0;
	//		}

	//	}

	//}

	//return true;
}

