

#include "LMatrixStorage.h"

// ���캯������ʼ��Ϊ0�У�0�еľ���
LMatrixStorage::LMatrixStorage()
{
	m_nRow = m_nCol = 0;
	m_nBase = 1;
	m_pBuf = NULL;
}

// ���캯������ʼ��Ϊrow�У�col�еľ���
// ����Ԫ��ֵ��ʼ��Ϊ0
LMatrixStorage::LMatrixStorage(int row,int col)
{	
	m_nBase=1;

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
	memcpy(m_pBuf,m.GetBuf(),nBufSize);	//������ͬ�ľ���ֵ����memcpy���У����Ч��
	
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
	LMatrixStorage m(r,r);	//�˴����캯�����Ѿ�������
	
	for (int i=m.GetBase();i<=m.GetRow();i++)
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

// Ϊ���Ч�ʣ��Ѹú�������Ϊ��������
//inline long double * LMatrixStorage::GetRowAddr(int row)
//{
//	if(row>=m_nRow+m_nBase||row<m_nBase) return NULL;
//	return m_pBuf+(row-m_nBase)*m_nCol;
//}

//	=========================
//	��������	:	SetRow
//	��������	:	���þ��������
//	row			:	�����������
//	ע			:	����Ԫ�س�ʼ��Ϊ0
void LMatrixStorage::SetRow(int row) 
{ 
	if(row<0) return;

	if(row>m_nRow)	//��չ��������m_nRow>=0����row>m_nRow������row>0
	{
		int nMemLen = m_pBuf==NULL?0:_msize(m_pBuf);
		int nWantedLen = row*m_nCol*sizeof(long double);

		if(nMemLen<nWantedLen)	//��Ҫ���·���洢�ռ�
		{
			long double * p = (long double*)realloc(m_pBuf,nWantedLen);
			if(p==NULL) return;	//�洢�ռ䲻�㣬�޷���չ����
			else m_pBuf=p;
		}
		
		memset(m_pBuf+m_nRow*m_nCol,0,(row-m_nRow)*m_nCol*sizeof(long double));

		m_nRow = row;
	}

	else	// if(row<=m_nRow)	//��������
	{
		m_nRow = row; 
	}
}


void LMatrixStorage::SetCol(int col)
{
	int i;

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
			long double * p = (long double*)realloc(m_pBuf,nWantedLen);
			if(p==NULL) return;	 	//�洢�ռ䲻�㣬�޷���չ����
			else	m_pBuf = p;			
		}
		
		//�����һ�п�ʼÿ�к���
		for (i=m_nRow-1;i>=0;i--)
		{
			memmove(m_pBuf+i*col,m_pBuf+i*m_nCol,m_nCol*sizeof(long double));			
		}

		//������Ԫ�ص�ֵ��Ϊ0
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
//	���в���
//
//==================================================================================================================================

//	========================================
//	��������	:	DeleteRows
//	��������	:	ɾ�������е�������
//	nStartRow	:	��Ҫɾ������ʼ�У���m_nBase��ʼ
//	nRows		:	��Ҫɾ����������nRows==-1��ʾɾ��nStartRow��ǰһ�У�nRows==-2��ʾ��ǰɾ��2��
//	����ֵ		:	ɾ��������
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


	//���ݰ��
	memmove(pStart,pEnd,(pTerm-pEnd)*sizeof(long double));

	//ֱ���޸�����������Ŀռ䲻��Ҫ������
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

	//ֱ���޸�����������Ŀռ䲻��Ҫ������
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
//	��������	:	InsertRows
//	��������	:	����������
//	m1				:	��Ҫ���뵽�����е����ݣ����m�����������ڵ�ǰ�������������m���������루�ضϣ�Ϊ�뵱ǰ������������
//	nPos			:	����λ��Ϊ��nPos��֮ǰ��nPos<m_nBase��������뵽����ĩβ
LMatrixStorage& LMatrixStorage::InsertRows(LMatrixStorage &m1,int nPos)
{
	//0��ȷ������λ��
	if (nPos-m_nBase<0||nPos-m_nBase>=m_nRow) nPos=m_nRow+m_nBase; 

	//1�����ȵ�����Ҫ����ľ����������ʹ֮�뵱ǰ������ͬ
	LMatrixStorage m(m1);
	m.SetCol(this->GetCol());

	//2��Ȼ�����䵱ǰ���������
	SetRow(m_nRow+m.GetRow());

	//3�����ݰ��
	memmove(this->GetRowAddr(nPos+m.GetRow()),this->GetRowAddr(nPos),(m_nRow-m.GetRow()-nPos+m_nBase)*m_nCol*sizeof(long double));
	memcpy(this->GetRowAddr(nPos),m.GetBuf(),m.GetRow()*m.GetCol()*sizeof(long double));

	return *this;
}

//	=============================================
//	��������	:	InsertCols
//	��������	:	��ǰ�����в�������������
//	nPos		:	���뵽��nPos��֮ǰ��nPos<m_nBase��ʾ���뵽���һ��
LMatrixStorage& LMatrixStorage::InsertCols(LMatrixStorage &m1,int nPos)
{
	//0��ȷ������λ��
	if (nPos-m_nBase<0||nPos-m_nBase>=m_nCol) nPos=m_nCol+m_nBase; 

	LMatrixStorage m(m1);
	m.SetRow(this->GetRow());

	//1)����������������
	SetCol(GetCol()+m.GetCol());

	//2)���ݰ��
	for(int i=m_nBase;i<m_nRow+m_nBase;i++)
	{
		memmove(GetRowAddr(i)+(nPos-m_nBase)+m.GetCol(),GetRowAddr(i)+nPos-m_nBase,(m_nCol-nPos-m.GetCol()+m_nBase)*sizeof(long double));
		//��Ϊ��ͬ����һ���������ʹ��memcpy�ӿ��ٶ�
		memcpy(GetRowAddr(i)+nPos-m_nBase,m.GetRowAddr(i),m.GetCol()*sizeof(long double));
	}
	return *this;
}


//	=================================
//	��������	:	Sub
//	��������	:	ȡ�����һ����
//	
LMatrixStorage LMatrixStorage::Sub(int sr,int sc,int nr,int nc)
{
	int r,c;
	int er = sr+nr;
	int ec = sc+nc;

	//����ȷ����ֹ������ �Ϸ�
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

	//���ɷ����Ӿ���ߴ���¾���
	r = er-sr;
	c = ec-sc;
	LMatrixStorage m(r,c);

	//���ݰ��
	for (int i=0;i<m.GetRow();i++)
	{
		memcpy(m.GetBuf()+i*m.GetCol(),m_pBuf+(sr+i-m_nBase)*m_nCol+sc-m_nBase,c*sizeof(long double));
	}

	return m;
}

LMatrixStorage LMatrixStorage::Rows(int nStartRow,int nRows)	//ȡ�����nStartRow�п�ʼ��nRows��
{
	return Sub(nStartRow,m_nBase,nRows,m_nCol);
}
LMatrixStorage LMatrixStorage::Cols(int nStartCol,int nCols)	//ȡ�����nStartCol�п�ʼ��nCols��
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
//	���Թ���
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

	//����Ϊ�գ�ֱ�ӷ���
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
void LMatrixStorage::Parse(CString& szText)				//��szText�ж�ȡ����ĸ���Ԫ��
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

}*/