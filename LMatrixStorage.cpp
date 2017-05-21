

#include "LMatrixStorage.h"

// 构造函数，初始化为0行，0列的矩阵
LMatrixStorage::LMatrixStorage()
{
	m_nRow = m_nCol = 0;
	m_nBase = 1;
	m_pBuf = NULL;
}

// 构造函数，初始化为row行，col列的矩阵
// 矩阵元素值初始化为0
LMatrixStorage::LMatrixStorage(int row,int col)
{	
	m_nBase=1;

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
	else
	{
		m_pBuf=NULL;
	}
}

LMatrixStorage& LMatrixStorage::operator =(LMatrixStorage& m)
{
	if(this==&m) return *this;

	m_nRow = m.GetRow();
	m_nCol = m.GetCol();
	m_nBase = m.GetBase();

	int nBufSize = m_nRow*m_nCol*sizeof(long double);
	m_pBuf = (long double*)malloc(nBufSize);
	memcpy(m_pBuf,m.GetBuf(),nBufSize);	//两个不同的矩阵赋值，用memcpy就行，提高效率
	
	return *this;
}




LMatrixStorage::LMatrixStorage(LMatrixStorage& m)
{
	*this=m;
}


LMatrixStorage::~LMatrixStorage()
{
	if (m_pBuf!=NULL) 
	{ 
		free(m_pBuf); 
		m_pBuf=NULL; 
	}
}

LMatrixStorage LMatrixStorage::I(int r)
{
	LMatrixStorage m(r,r);	//此处构造函数中已经清零了
	
	for (int i=m.GetBase();i<=m.GetRow();i++)
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
//inline int LMatrixStorage::GetRow() { return m_nRow; }
//inline int LMatrixStorage::GetCol() { return m_nCol; }
//inline int LMatrixStorage::GetBase(){ return m_nBase;}
//inline void LMatrixStorage::SetBase(int base) { m_nBase=base; }
//inline long double& LMatrixStorage::operator ()(int row,int col)
//{
//	return m_pBuf[(row-m_nBase)*m_nCol+col-m_nBase];
//}

//inline long double LMatrixStorage::operator ()(int row,int col) const
//{
//	return m_pBuf[(row-m_nBase)*m_nCol+col-m_nBase];
//}


//inline long double * LMatrixStorage::GetBuf() { return m_pBuf; }

// 为提高效率，把该函数定义为内联函数
//inline long double * LMatrixStorage::GetRowAddr(int row)
//{
//	if(row>=m_nRow+m_nBase||row<m_nBase) return NULL;
//	return m_pBuf+(row-m_nBase)*m_nCol;
//}

//	=========================
//	函数名称	:	SetRow
//	函数功能	:	设置矩阵的行数
//	row			:	矩阵的新行数
//	注			:	新增元素初始化为0
void LMatrixStorage::SetRow(int row) 
{ 
	if(row<0) return;

	if(row>m_nRow)	//扩展矩阵，由于m_nRow>=0，又row>m_nRow，所以row>0
	{
		int nMemLen = m_pBuf==NULL?0:_msize(m_pBuf);
		int nWantedLen = row*m_nCol*sizeof(long double);

		if(nMemLen<nWantedLen)	//需要重新分配存储空间
		{
			long double * p = (long double*)realloc(m_pBuf,nWantedLen);
			if(p==NULL) return;	//存储空间不足，无法扩展矩阵
			else m_pBuf=p;
		}
		
		memset(m_pBuf+m_nRow*m_nCol,0,(row-m_nRow)*m_nCol*sizeof(long double));

		m_nRow = row;
	}

	else	// if(row<=m_nRow)	//缩减矩阵
	{
		m_nRow = row; 
	}
}


void LMatrixStorage::SetCol(int col)
{
	int i;

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
			long double * p = (long double*)realloc(m_pBuf,nWantedLen);
			if(p==NULL) return;	 	//存储空间不足，无法扩展矩阵
			else	m_pBuf = p;			
		}
		
		//从最后一行开始每行后移
		for (i=m_nRow-1;i>=0;i--)
		{
			memmove(m_pBuf+i*col,m_pBuf+i*m_nCol,m_nCol*sizeof(long double));			
		}

		//将新增元素的值置为0
		for (i=0;i<m_nRow;i++)
		{
			memset(m_pBuf+i*col+m_nCol,0,(col-m_nCol)*sizeof(long double));
		}

		m_nCol = col;
	}

	else //if(col==m_nCol)
	{
	}
}


//==================================================================================================================================
//
//	行列操作
//
//==================================================================================================================================

//	========================================
//	函数名称	:	DeleteRows
//	函数功能	:	删除矩阵中的若干行
//	nStartRow	:	需要删除的起始行，从m_nBase开始
//	nRows		:	需要删除的行数，nRows==-1表示删除nStartRow的前一行，nRows==-2表示向前删除2行
//	返回值		:	删除的行数
int LMatrixStorage::DeleteRows(int nStartRow,int nRows)
{
	int nEndRow = nStartRow+nRows;
	if(nEndRow<nStartRow) 
	{
		int nTemp = nEndRow;
		nEndRow	  = nStartRow;
		nStartRow = nTemp;
	}

	if(nEndRow<=m_nBase||nStartRow>=m_nRow+m_nBase) return 0;

	nStartRow = nStartRow<m_nBase?m_nBase:nStartRow;
	nEndRow   = nEndRow>m_nRow+m_nBase?m_nRow+m_nBase:nEndRow;
	nRows = nEndRow-nStartRow;

	long double * pStart = GetRowAddr(nStartRow);
	long double * pEnd   = GetRowAddr(nEndRow);
	long double * pTerm  = GetRowAddr(m_nRow+m_nBase);


	//数据搬家
	memmove(pStart,pEnd,(pTerm-pEnd)*sizeof(long double));

	//直接修改行数，多余的空间不需要清零了
	m_nRow-=nRows;

	return nRows;
}

int LMatrixStorage::DeleteCols(int nStartCol,int nCols)
{
	int nEndCol = nStartCol+nCols;
	if(nEndCol<nStartCol) 
	{
		int nTemp = nEndCol;
		nEndCol	  = nStartCol;
		nStartCol = nTemp;
	}

	if(nEndCol<=m_nBase||nStartCol>=m_nCol+m_nBase) return 0;

	nStartCol = nStartCol<m_nBase?m_nBase:nStartCol;
	nEndCol   = nEndCol>m_nCol+m_nBase?m_nCol+m_nBase:nEndCol;
	nCols = nEndCol-nStartCol;

	for(int i=m_nBase;i<m_nRow+m_nBase;i++)
	{
		long double * pDes	 = GetBuf()+(i-m_nBase)*(m_nCol-nEndCol+nStartCol)+(nStartCol-m_nBase);
		long double * pSrc   = GetRowAddr(i)+(nEndCol-m_nBase);
		long double * pTerm  = GetRowAddr(i+1);
		memmove(pDes,pSrc,(pTerm-pSrc)*sizeof(long double));
	}

	//直接修改列数，多余的空间不需要清零了
	nCols = nEndCol-nStartCol;
	m_nCol-=nCols;
//	SetCol(m_nCol-nCols);

	return nCols;
}


int LMatrixStorage::DeleteRows(int nStartRow)
{
	return DeleteRows(nStartRow,m_nRow-nStartRow+m_nBase);
}

int LMatrixStorage::DeleteCols(int nStartCol)
{
	return DeleteCols(nStartCol,m_nCol-nStartCol+m_nBase);
}

int LMatrixStorage::DeleteRow(int nRow)
{
	return DeleteRows(nRow,1);
}

int LMatrixStorage::DeleteCol(int nCol)
{
	return DeleteCols(nCol,1);
}

//	=============================================
//	函数名称	:	InsertRows
//	函数功能	:	插入若干行
//	m1				:	需要插入到矩阵中的数据，如果m的列数不等于当前矩阵的列数，则将m的列数补齐（截断）为与当前矩阵的列数相等
//	nPos			:	插入位置为第nPos行之前，nPos<m_nBase则表明插入到矩阵末尾
LMatrixStorage& LMatrixStorage::InsertRows(LMatrixStorage &m1,int nPos)
{
	//0）确定插入位置
	if (nPos-m_nBase<0||nPos-m_nBase>=m_nRow) nPos=m_nRow+m_nBase; 

	//1）首先调整需要插入的矩阵的列数，使之与当前矩阵相同
	LMatrixStorage m(m1);
	m.SetCol(this->GetCol());

	//2）然后扩充当前矩阵的行数
	SetRow(m_nRow+m.GetRow());

	//3）数据搬家
	memmove(this->GetRowAddr(nPos+m.GetRow()),this->GetRowAddr(nPos),(m_nRow-m.GetRow()-nPos+m_nBase)*m_nCol*sizeof(long double));
	memcpy(this->GetRowAddr(nPos),m.GetBuf(),m.GetRow()*m.GetCol()*sizeof(long double));

	return *this;
}

//	=============================================
//	函数名称	:	InsertCols
//	函数功能	:	向当前矩阵中插入若干列数据
//	nPos		:	插入到第nPos列之前，nPos<m_nBase表示插入到最后一列
LMatrixStorage& LMatrixStorage::InsertCols(LMatrixStorage &m1,int nPos)
{
	//0）确定插入位置
	if (nPos-m_nBase<0||nPos-m_nBase>=m_nCol) nPos=m_nCol+m_nBase; 

	LMatrixStorage m(m1);
	m.SetRow(this->GetRow());

	//1)首先扩充矩阵的列数
	SetCol(GetCol()+m.GetCol());

	//2)数据搬家
	for(int i=m_nBase;i<m_nRow+m_nBase;i++)
	{
		memmove(GetRowAddr(i)+(nPos-m_nBase)+m.GetCol(),GetRowAddr(i)+nPos-m_nBase,(m_nCol-nPos-m.GetCol()+m_nBase)*sizeof(long double));
		//因为不同属于一个矩阵，因此使用memcpy加快速度
		memcpy(GetRowAddr(i)+nPos-m_nBase,m.GetRowAddr(i),m.GetCol()*sizeof(long double));
	}
	return *this;
}


//	=================================
//	函数名称	:	Sub
//	函数功能	:	取矩阵的一部分
//	
LMatrixStorage LMatrixStorage::Sub(int sr,int sc,int nr,int nc)
{
	int r,c;
	int er = sr+nr;
	int ec = sc+nc;

	//首先确保起止行列数 合法
	if(er<sr) { r=er; er=sr; sr=r; }
	if(ec<sc) { c=ec; ec=sc; sc=c; }

	if(er<=m_nBase||ec<=m_nBase||sr>=m_nBase+m_nRow||sc>=m_nBase+m_nCol) 
	{
		LMatrixStorage ms;
		return ms;
	}

	if (sr<m_nBase) sr=m_nBase;
	if (sc<m_nBase) sc=m_nBase;

	if (er>m_nRow+m_nBase) er=m_nRow+m_nBase;
	if (ec>m_nCol+m_nBase) ec=m_nCol+m_nBase;

	//生成符合子矩阵尺寸的新矩阵
	r = er-sr;
	c = ec-sc;
	LMatrixStorage m(r,c);

	//数据搬家
	for (int i=0;i<m.GetRow();i++)
	{
		memcpy(m.GetBuf()+i*m.GetCol(),m_pBuf+(sr+i-m_nBase)*m_nCol+sc-m_nBase,c*sizeof(long double));
	}

	return m;
}

LMatrixStorage LMatrixStorage::Rows(int nStartRow,int nRows)	//取矩阵的nStartRow行开始的nRows行
{
	return Sub(nStartRow,m_nBase,nRows,m_nCol);
}
LMatrixStorage LMatrixStorage::Cols(int nStartCol,int nCols)	//取矩阵的nStartCol列开始的nCols列
{
	return Sub(m_nBase,nStartCol,m_nRow,nCols);
}

LMatrixStorage LMatrixStorage::Row(int row) { return Rows(row,1); }
LMatrixStorage LMatrixStorage::Col(int col) { return Cols(col,1); }

int LMatrixStorage::Clear(int nRow,int nNum=1)
{
	if(nNum==0) return 0;

	int nSRow,nERow;

	nSRow=nRow;	nERow=nSRow+nNum;
	if(nSRow>nERow) { nRow=nSRow;nSRow=nERow;nERow=nRow; }
	if(nERow<=m_nBase) return 0;
	if(nSRow>m_nRow+m_nBase-1) return 0;

	if(nSRow<m_nBase) nSRow=m_nBase;
	if(nERow>m_nRow+m_nBase) nERow=m_nRow+m_nBase;

	long double * p = GetRowAddr(nSRow);
	nNum = nERow-nSRow;
	memset(p,0,sizeof(long double)*nNum*m_nCol);

	return nNum;
}


//==================================================================================================================================
//
//	调试功能
//
//==================================================================================================================================
void LMatrixStorage::Format(CString& szText,int nLen,int nDot)
{
	CString szElement,szLen,szDot,szFormat;

	szLen.Format(_T("%d"),nLen);
	szDot.Format(_T("%d"),nDot);

	szFormat = _T("%-");
	szFormat+=szLen+_T(".")+szDot+_T("f");

	int i,j;

	//数组为空，直接返回
	if(m_nRow<=0||m_nCol<=0) 
	{
		szText+=_T("The Matrix is Empty");
		return;
	}
	else
	{
		for (i=m_nBase;i<m_nRow+m_nBase;i++)
		{
			for(j=m_nBase;j<m_nCol+m_nBase;j++)
			{
				szElement.Format(szFormat,(*this)(i,j));
				szText+=szElement;
			}
			if(i!=m_nRow+m_nBase-1) szText+=_T("\r\n");
		}
	}
}

void LMatrixStorage::Save(CFile& file,int nLen,int nDot)
{
	CString szText;
	Format(szText,nLen,nDot);
	file.Write(szText,szText.GetLength()*sizeof(TCHAR));
}

void LMatrixStorage::DBG(CFile& file,LPCTSTR szText)
{
	file.Write(szText,_tcslen(szText)*sizeof(TCHAR));
}

void LMatrixStorage::InitTestValue()
{
	for(int i=m_nBase;i<m_nRow+m_nBase;i++)
		for(int j=m_nBase;j<m_nCol+m_nBase;j++)
			(*this)(i,j)=i*10+j;
}

/*
void LMatrixStorage::Parse(CString& szText)				//从szText中读取矩阵的各个元素
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

}*/