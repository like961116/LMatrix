
#include "LMatrix.h"
#include <math.h>

//==================================================================================================================================
//
//	���캯������������
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
	
	//����������С��0
	row=row<0?0:row;
	col=col<0?0:col;

	m_nRow = row;
	m_nCol = col; 

	//����洢�ռ�
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
//	���Բ���
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
//	��������	:	SetRow
//	��������	:	���þ��������
//	row			:	�����������
//	ע			:	����Ԫ�س�ʼ��Ϊ0
void LMatrix::SetRow(int row) 
{ 
	if(row<0) return;

	if(row>m_nRow)	//��չ��������m_nRow>=0����row>m_nRow������row>0
	{
		int nMemLen = m_pBuf==NULL?0:_msize(m_pBuf);
		int nWantedLen = row*m_nCol*sizeof(long double);

		if(nMemLen<nWantedLen)	//��Ҫ���·���洢�ռ�
		{
			long double * p = (long double*)malloc(nWantedLen);
			if(p==NULL) return;	//�洢�ռ䲻�㣬�޷���չ����
			
			//�ɹ�����ռ�
			memcpy(p,m_pBuf,m_nRow*m_nCol*sizeof(long double));
			free(m_pBuf);
			m_pBuf = p;
		}
		
		memset(m_pBuf+m_nRow*m_nCol,0,(row-m_nRow)*m_nCol);

		m_nRow = row;
	}

	else	// if(row<=m_nRow)	//��������
	{
		m_nRow = row; 
	}
}


void LMatrix::SetCol(int col)
{
	if(col<0) return;

	else if(col<m_nCol)	//������С��������Ҫ��ǰ�ƶ�
	{
		for (int i=1;i<m_nRow;i++)	//�ӵڶ��п�ʼÿ��ǰ��
		{
			memmove(m_pBuf+i*col,m_pBuf+i*m_nCol,col*sizeof(long double));			
		}
		m_nCol = col;
	}

	else if(col>m_nCol)	//�������ӣ�������Ҫ����ƶ�������Ҫ���·���ռ�
	{
		int nMemLen = m_pBuf==NULL?0:_msize(m_pBuf);
		int nWantedLen = m_nRow*col*sizeof(long double);

		if(nMemLen<nWantedLen)	//��Ҫ���·���洢�ռ�
		{
			long double * p = (long double*)malloc(nWantedLen);
			if(p==NULL) return;	//�洢�ռ䲻�㣬�޷���չ����
			
			//�ɹ�����ռ䣬���Ƚ����ݰᵽ��λ��
			memcpy(p,m_pBuf,m_nRow*m_nCol*sizeof(long double));
			free(m_pBuf);
			m_pBuf = p;			
		}
		
		//�����һ�п�ʼÿ�к���
		for (int i=m_nRow-1;i>=0;i--)
		{
			memmove(m_pBuf+i*col,m_pBuf+i*m_nCol,m_nCol*sizeof(long double));			
		}

		//������Ԫ�ص�ֵ��Ϊ0
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
//	���в���
//
//==================================================================================================================================

//	========================================
//	��������	:	DelRows
//	��������	:	ɾ�������е�������
//	nStartRow	:	��Ҫɾ������ʼ�У���1��ʼ
//	nRows		:	��Ҫɾ����������nRows==-1��ʾɾ������������
//	����ֵ		:	ɾ��������
int LMatrix::DelRows(int nStartRow,int nRows)
{
	if(nStartRow>m_nRow||m_nRow==0 ) return 0;
	else if(nStartRow<=0) nStartRow=1;

	if(nRows<0) nRows=m_nRow-nStartRow+1;
	else if(nRows>m_nRow-nStartRow+1) nRows=m_nRow-nStartRow+1;
	
	//���ݰ��
	memmove(m_pBuf+(nStartRow-1)*m_nCol,m_pBuf+(nStartRow-1+nRows)*m_nCol,(m_nRow+1-nStartRow-nRows)*m_nCol*sizeof(long double));

	//����Ŀռ�����
	memset(m_pBuf+(m_nRow-nRows)*m_nCol,0,nRows*m_nCol*sizeof(long double));

	m_nRow -= nRows;

	return nRows;
}

int LMatrix::DelCols(int nStartCol,int nCols)
{
	if(nStartCol>m_nCol||m_nCol==0) return 0;
	else if(nStartCol<=0) nStartCol=1;

	if(nCols>m_nCol-nStartCol+1) nCols=m_nCol-nStartCol+1;

	//ÿһ��ɾ������֮�������ǰ��
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
//	��������	:	InsertRows
//	��������	:	����������
//	nPos		:	���뵽��nPos��֮ǰ��nPos==1��ʾ���뵽��һ��֮ǰ��nPos<=0��ʾ���뵽ĩβ
//					
LMatrix& LMatrix::InsertRows(LMatrix &m1,int nPos)
{
	if (nPos<=0||nPos>m_nRow) nPos=m_nRow+1; 

	LMatrix m(m1);
	m.SetCol(this->GetCol());

	//1)����������������
	SetRow(GetRow()+m.GetRow());

	//2)���ݰ��
	memmove(this->GetRowAddr(nPos+m.GetRow()),this->GetRowAddr(nPos),(m_nRow-m.GetRow()-nPos+1)*m_nCol*sizeof(long double));
	memmove(this->GetRowAddr(nPos),m.GetBuf(),m.GetRow()*m.GetCol()*sizeof(long double));

	return *this;
}

//	=============================================
//	��������	:	InsertCols
//	��������	:	��ǰ�����в�������������
//	nPos		:	���뵽��nPos��֮ǰ��nPos=1��ʾ���뵽��һ��֮ǰ
//					nPos<=0��nPos>m_nCol��ʾ���뵽���һ��
LMatrix& LMatrix::InsertCols(LMatrix &m1,int nPos)
{
	if (nPos<=0||nPos>m_nCol) nPos=m_nCol+1; 

	LMatrix m(m1);
	m.SetRow(this->GetRow());

	//1)����������������
	SetCol(GetCol()+m.GetCol());

	//2)���ݰ��
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

	//����ȷ����ֹ������ �Ϸ�
	if(er<sr) { r=er; er=sr; sr=r; }
	if(ec<sc) { c=ec; ec=sc; sc=c; }

	if (sr<=0) sr=1;
	if (sc<=0) sc=1;

	if (er>m_nRow) er=m_nRow;
	if (ec>m_nCol) ec=m_nCol;

	//���ɷ����Ӿ���ߴ���¾���
	r = er-sr+1;
	c = ec-sc+1;
	LMatrix m(r,c);

	//���ݰ��
	for (int i=0;i<m.GetRow();i++)
	{
		memcpy(m.GetBuf()+i*m.GetCol(),m_pBuf+(sr+i-1)*m_nCol+sc-1,c*sizeof(long double));
	}

	return m;
}

LMatrix LMatrix::Rows(int nStartRow,int nRows)	//ȡ�����nStartRow�п�ʼ��nRows��
{
	return Sub(nStartRow,1,nStartRow+nRows-1,m_nCol);
}
LMatrix LMatrix::Cols(int nStartCol,int nCols)	//ȡ�����nStartCol�п�ʼ��nCols��
{
	return Sub(1,nStartCol,nStartCol+nCols-1,m_nRow);
}

LMatrix LMatrix::Row(int row) {	return Sub(row,1,row,m_nCol); }
LMatrix LMatrix::Col(int col) { return Sub(1,col,m_nRow,col); }


//==================================================================================================================================
//
//	���������
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
//	��������
//
//==================================================================================================================================
int LMatrix::Inv()
{
	int i,j,k;
	//���Ƿ��������ʽ����0���򲻿���
	long double fDet = Det();
	if( m_nRow!=m_nCol || fDet==0 || fabs(fDet)<=m_fZero ) return 0;

	//��λ��
	LMatrix m = I(m_nRow);

	//�仯Ϊ������
	for(i=1;i<=m_nRow-1;i++)
	{
		//���Ԫ��[i,i]==0�����������i���� ��Ȼ��һ����Ϊ0�ģ���������ʽ==0
		//��ʱ�����мӵ���i����
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

		//��Ϊ������
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

	//��Ϊ�Խ���
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
		//���Ԫ��[i,i]==0�����������i���� ��Ȼ��һ����Ϊ0�ģ���������ʽ==0
		//��ʱ�����мӵ���i����
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
//	��������	:	Cholesky
//	��������	:	���ؾ����Cholesky�ֽ�
//	Cholesky�ֽ�:	C��ʵ�Գ������������Ȼ���������Ǿ���T��ʹ��C=TT'
//					�ú������صľ���T
LMatrix LMatrix::Cholesky()
{
	int i,j,k;
	long double s;

	LMatrix t(m_nRow,m_nCol);

	for (i=1;i<=m_nRow;i++)
	{
		//1)����tii
		s=0;
		for(k=1;k<i;k++) s+=t(i,k)*t(i,k);
		t(i,i) = sqrt(GetValue(i,i)-s);

		//2)����tji
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

	//�������������Ϊ0
	for (i=1;i<=m_nRow;i++)
		for (j=i+1;j<=m_nRow;j++)
			t(i,j) =0.0;

//	LStringA s2 = "�ոռ���ó���cholesky����Ϊt=��\r\n";
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
//	��ѧ����
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
//	ͳ������
//==================================================================================================================================

//	���������ľ�ֵ������һ��������
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

//	�Ѿ����ÿһ����Ϊһ������������Щ������Э�������
//	����ֵ	:	Э�������
//				����������ȣ�������<=1�����޷���Э������󣬴�ʱ���صľ���Խ�Ԫ��Ϊ-1
LMatrix LMatrix::Cov()
{
	int i,j,k;
	long double s;
	
	LMatrix c(m_nCol,m_nCol);
	if (m_nCol<=0) return c;		//û��������ֱ�ӷ���
	if (m_nRow<=1)					//������������Ϊ2
	{
		c(1,1) = -1.0;
		return c;
	}

	LMatrix a = Average();				//��ֵ����

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
//	�����㷨
//
//==================================================================================================================================

//�Ե�col�н��п����������col<=0�����ÿһ�е�����������
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
		long double key = GetValue(i,col);	//�Ȱ��м�����ݱ�������

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
		double key1 = GetValue(i,col);	//�Ȱ��м�����ݱ�������
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
//	���Թ���
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


void LMatrix::Load(char * szFileName)				//��szFileName�ж�ȡ����ĸ���Ԫ��
{
	LStringA szFile;
	szFile.LoadFile("c:\\a.txt");
	LList pRows = szFile.SplitB("\r\n",-1);
	
	//ɾ������
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

	//����������
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