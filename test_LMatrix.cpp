#include "test_LMatrix.h"

void Test_LMatrix_Equal()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Equal.txt"),CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	CString szText,szFormat;

	//1)Two Matrices With Same And Nagtive Sizes
	LMatrix m1(-1,-1),m2(-1,-1);
	szText.Format(_T("Two matrices with different sizes are not equal.\r\n\r\nTwo empty matrices(row<=0 or col<=0) with same sizes are equal.\r\n\r\nm1(-1,-1)==m2(-1,-1): %d\r\n"),m1==m2);
	m1.DBG(file,szText);

	int nBaseRow1=-1,nBaseCol1=-1,nBaseRow2=-1,nBaseCol2=-1;

	for(int i=1;i<4;i++)
	{
		nBaseRow1++;
		m1.SetRow(nBaseRow1);
		szFormat.Format(_T("m1(%d,%d)==m2(%d,%d)"),nBaseRow1,nBaseCol1,nBaseRow2,nBaseCol2);
		szText.Format(_T("%-20s: %d\r\n"),szFormat,m1==m2);
		m1.DBG(file,szText);

		nBaseCol1++;
		m1.SetCol(nBaseCol1);
		szFormat.Format(_T("m1(%d,%d)==m2(%d,%d)"),nBaseRow1,nBaseCol1,nBaseRow2,nBaseCol2);
		szText.Format(_T("%-20s: %d\r\n"),szFormat,m1==m2);
		m1.DBG(file,szText);

		nBaseRow2++;
		m2.SetRow(nBaseRow2);
		szFormat.Format(_T("m1(%d,%d)==m2(%d,%d)"),nBaseRow1,nBaseCol1,nBaseRow2,nBaseCol2);
		szText.Format(_T("%-20s: %d\r\n"),szFormat,m1==m2);
		m1.DBG(file,szText);

		nBaseCol2++;
		m2.SetCol(nBaseCol2);
		szFormat.Format(_T("m1(%d,%d)==m2(%d,%d)"),nBaseRow1,nBaseCol1,nBaseRow2,nBaseCol2);
		szText.Format(_T("%-20s: %d\r\n"),szFormat,m1==m2);
		m1.DBG(file,szText);
	}

	m1.InitTestValue();
	m2.InitTestValue();
	m1.DBG(file,_T("\r\n\r\nm1=\r\n"));

	m1.Save(file,8,1);
	m1.DBG(file,_T("\r\n\r\nm2=\r\n"));
	m2.Save(file,8,1);
	m2.DBG(file,_T("\r\n\r\n"));
	szText.Format(_T("m1==m2:    %d\r\n\r\nm2=\r\n"),m1==m2);
	m1.DBG(file,szText);

	m2(1,2) += 0.0001;
	m2.Save(file,10,4);
	m2.DBG(file,_T("\r\n\r\n"));
	szText.Format(_T("m1==m2:    %d\r\n\r\n"),m1==m2);
	m1.DBG(file,szText);

	m1.SetZero(0.0001);
	m2.DBG(file,_T("\r\n\r\n"));
	szText.Format(_T("set m1.zero=0.0001,m1==m2:    %d\r\n\r\n"),m1==m2);
	m1.DBG(file,szText);

	m1.SetZero(0.00001);
	m2.DBG(file,_T("\r\n\r\n"));
	szText.Format(_T("set m1.zero=0.00001,m1==m2:    %d\r\n\r\n"),m1==m2);
	m1.DBG(file,szText);

	file.Close();

}


void Test_LMatrix_Add_Subtract()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Add_Subtract.txt"),CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	CString szText,szText1,szText2,szText3;

	LMatrix m;
	szText.Format(_T("Use equation m1+m2-m2==m1\r\n\r\n"));
	m.DBG(file,szText);

	int Loop=1;

	for(int i=1;i<5;i++)
	{
		for(int j=1;j<5;j++)
		{
			LMatrix m1(i,j),m2(i,j);
			m1.SetZero(0.00000001);
			m2.SetZero(0.00000001);
			for(int i1=1;i1<=i;i1++)
			{
				for(int j1=1;j1<=j;j1++)
				{
					m1(i1,j1) = rand()*1.0*10/RAND_MAX;
					m2(i1,j1) = rand()*1.0*10/RAND_MAX;
				}
			}

			szText.Format(_T("\r\n\r\n========== Loop %d =========="),Loop);
			m.DBG(file,szText);
			m.DBG(file,_T("\r\nThe First Matrix m1=:\r\n"));
			m1.Save(file,14,10);
			m.DBG(file,_T("\r\nThe Second Matrix m2=:\r\n"));
			m2.Save(file,14,10);
			m=m1-m2+m2;
			m.DBG(file,_T("\r\nm1-m2+m2=:\r\n"));
			m.Save(file,14,10);

			if(m!=m1) 
			{
				m.DBG(file,_T("\r\n Failed \r\n=============================\r\n"));
			}
			else
			{
				m.DBG(file,_T("\r\n Success \r\n=============================\r\n"));
			}
			Loop++;

		}
	}
	file.Close();
}

void Test_LMatrix_Multiply()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Multiply.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	int Loop=1;
	LMatrix m;
	CString szText;
	for(int i=1;i<5;i++)
	{
		LMatrix m1(3,i);
		m1.InitTestValue();
		for(int j=1;j<5;j++)
		{
			szText.Format(_T("\r\n\r\n========== Loop %d ==========\r\n"),Loop);
			m1.DBG(file,szText);
			m1.DBG(file,_T("\r\n\r\nThe First Matrix m1=:\r\n"));
			m1.Save(file,20,10);

			LMatrix m2(i,j);
			m2.InitTestValue();
			m2.DBG(file,_T("\r\n\r\nThe Second Matrix m2=:\r\n"));
			m2.Save(file,20,10);

			LMatrix m3;
			m3 = m1*m2;
			szText.Format(_T("\r\n\r\nm1*m2=:\r\n"));
			m3.DBG(file,szText);
			m3.Save(file,20,10);
			m3.DBG(file,_T("\r\n\r\n=============================\r\n"));

			Loop++;
		}
	}

	LMatrix m1(2,3),m2(4,4);
	m1.DBG(file,_T("\r\n\r\n========== Multiply with mismatching sizes==========\r\n"));
	m1.InitTestValue();
	m2.InitTestValue();

	m1.DBG(file,_T("\r\n\r\n The First Matrix m1=:\r\n"));
	m1.Save(file,14,4);
	m2.DBG(file,_T("\r\n\r\n The Second Matrix m2=:\r\n"));
	m2.Save(file,14,4);
	LMatrix m3=m1*m2;
	m3.DBG(file,_T("\r\n\r\n m1*m2=:\r\n"));
	m3.Save(file,14,4);
	file.Close();
}

void Test_LMatrix_Divide()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Divide.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	CString szText;
	szText.Format(_T("Use equation m1*m2/m2==m1\r\n\r\n"));
	LMatrix	m;
	m.DBG(file,szText);

	int Loop=1;
	for(int i=1;i<5;i++)
	{
		LMatrix m1(i,i);
		LMatrix m2(i,i);
		
		m1.SetZero(0.00000001);
		m2.SetZero(0.00000001);

		for(int j=1;j<=i;j++)
		{
			for(int k=1;k<=i;k++)
			{
				m1(j,k) = rand()*10.0/RAND_MAX;
				m2(j,k)= rand()*10.0/RAND_MAX;
			}
		}

		szText.Format(_T("\r\n\r\n\r\n========== Loop %d==========\r\n"),Loop);
		m.DBG(file,szText);

		m1.DBG(file,_T("\r\n\r\nm1=\r\n"));
		m1.Save(file,8,4);

		m2.DBG(file,_T("\r\n\r\nm2=\r\n"));
		m2.Save(file,8,4);

		LMatrix m3 = m1*m2/m2;
		m3.DBG(file,_T("\r\n\r\nm1*m2/m2=\r\n"));
		m3.Save(file,8,4);

		if(m3==m1) m3.DBG(file,_T("\r\n\r\nSuccess\r\n\r\n"));
		else m3.DBG(file,_T("\r\n\r\nFailed\r\n\r\n"));

		Loop++;

	}

}

void Test_LMatrix_Inverse()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Inverse.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();


	CString szText;
	int Loop=1;
	for(int i=1;i<5;i++)
	{	
		LMatrix m(i,i);
		m.SetZero(0.00000001);
		for(int j=1;j<=i;j++)
			for(int k=1;k<=i;k++)
				m(j,k) = rand()*1.0/RAND_MAX;
		szText.Format(_T("\r\n\r\n========== Loop %d==========\r\n"),Loop);
		m.DBG(file,szText);
		m.DBG(file,_T("The Base Matrix is m:=\r\n"));
		m.Save(file,14,10);

		LMatrix m1=m^(-1);

		szText.Format(_T("\r\n\r\nThe Inverse Matrix is m1:=\r\n"));
		m1.DBG(file,szText);
		m1.Save(file,14,10);

		LMatrix m2 = m*m1;
		szText.Format(_T("\r\n\r\nThe Multiply Matrix m*m1 is m2:=\r\n"));
		m2.DBG(file,szText);
		m2.Save(file,14,10);
		
		LMatrix m3=m2.I(m2.GetRow());
		if(m2==m3) 
			m2.DBG(file,_T("\r\n\r\nm*m^-1==I,Success\r\n\r\n"));
		else
			m2.DBG(file,_T("\r\n\r\nm*m^-1!=I,Failed\r\n\r\n"));

		m2.DBG(file,_T("\r\n\r\n=============================\r\n"));

		Loop++;

	}
	file.Close();

}



void Test_LMatrix_Power()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Power.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	
	ar.WriteString(_T("Test Class LMatrix::operator^\r\n\r\n  1)Use equation (m^n)*[m^(-n)]=I"));
	ar.Close();

	CString szText;
	int Loop=1;

	for(int i=1;i<5;i++)
	{
		LMatrix m(i,i);
		for(int j=1;j<=i;j++)
			for(int k=1;k<=i;k++)
				m(j,k) = rand()*1.0/RAND_MAX;
		
		m.SetZero(0.00000001);

		szText.Format(_T("\r\n\r\n========== Loop %d ==========\r\n"),Loop);
		m.DBG(file,szText);
		m.DBG(file,_T("\r\nThe Origin Matrix m:=\r\n"));
		m.Save(file,14,10);
		
		for(int j=1;j<=5;j++)
		{
			szText.Format(_T("\r\n\r\nm^%d=\r\n"),j);
			LMatrix m1 = m^j;
			m1.DBG(file,szText);
			m1.Save(file,14,10);

			LMatrix m2 = m^(0-j);
			szText.Format(_T("\r\n\r\nm^%d=\r\n"),0-j);
			m2.DBG(file,szText);
			m2.Save(file,14,10);

			LMatrix m3 = m1*m2;
			szText.Format(_T("\r\n\r\nm^%d*m^%d=\r\n"),j,0-j);
			m3.DBG(file,szText);
			m3.Save(file,14,10);

			LMatrix m4=m3.I(m3.GetRow());

			if(m3==m4)
			{
				m3.DBG(file,_T("\r\n\r\nSuccess\r\n"));
			}
			else
			{
				m3.DBG(file,_T("\r\n\r\nFailed\r\n"));
			}
			m3.DBG(file,_T("\r\n====================\r\n\r\n"));
		}

		Loop++;
	}
}


void Test_LMatrix_Deter()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Deter.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();


	CString szText;
	srand(10);
	for(int i=1;i<5;i++)
	{
		LMatrix m(i,i);
		for(int j=1;j<=i;j++)
			for(int k=1;k<=i;k++)
				m(j,k) = (int)(rand()*10.0/RAND_MAX);

		m.DBG(file,_T("\r\n\r\nDeterminant\r\n\r\n"));
		m.Save(file,8,0);
		m.DBG(file,_T("\r\n=\r\n"));
		szText.Format(_T("%8.0f\r\n\r\n"),m.Deter());
		m.DBG(file,szText);
	}
	
}

void Test_LMatrix_Transposition()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Transposition.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	CString szText;
	szText.Format(_T("Use equation (m')'==m\r\n\r\n"));
	LMatrix	m;
	m.DBG(file,szText);

	int Loop=1;
	for(int i=1;i<5;i++)
	{
		LMatrix m1(i,5);
		
		m1.SetZero(0.00000001);

		for(int j=1;j<=i;j++)
		{
			for(int k=1;k<=5;k++)
			{
				m1(j,k) = rand()*10.0/RAND_MAX;
			}
		}

		szText.Format(_T("\r\n\r\n\r\n========== Loop %d==========\r\n"),Loop);
		m.DBG(file,szText);
		
		m1.DBG(file,_T("\r\n\r\nm1=\r\n"));
		m1.Save(file,8,4);
		
		LMatrix m2=m1.Trans();
		m2.DBG(file,_T("\r\n\r\nm2=m1'=\r\n"));
		m2.Save(file,8,4);

		LMatrix m3 = m2.Trans();
		m3.DBG(file,_T("\r\n\r\m3=m2'=(m1')'=\r\n"));
		m3.Save(file,8,4);

		if(m3==m1) m3.DBG(file,_T("\r\n\r\nSuccess\r\n\r\n"));
		else m3.DBG(file,_T("\r\n\r\nFailed\r\n\r\n"));

		Loop++;

	}
}

void Test_LMatrix_Cholesky()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrix_Cholesky.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	CString szText;
	szText.Format(_T("Use equation m==T*T'\r\n\r\n"));
	LMatrix	m;
	m.DBG(file,szText);

	int Loop=1;
	for(int i=1;i<5;i++)
	{
		LMatrix m1(i,i);
		
		m1.SetZero(0.00000001);

		for(int j=1;j<=i;j++)
		{
			for(int k=1;k<=j;k++)
			{
				m1(j,k) = rand()*10.0/RAND_MAX;
				m1(k,j) = m1(j,k);
			}
		}

		szText.Format(_T("\r\n\r\n\r\n========== Loop %d==========\r\n"),Loop);
		m.DBG(file,szText);

		m1.DBG(file,_T("\r\n\r\nm1=\r\n"));
		m1.Save(file,8,4);
		
		LMatrix m2=m1.Cholesky();
		m2.DBG(file,_T("\r\n\r\nm2=m1.Cholesky()=\r\n"));
		m2.Save(file,8,4);


		LMatrix m3=m2*m2.Trans();
		m3.DBG(file,_T("\r\n\r\nm3=m2*m2'=\r\n"));
		m3.Save(file,8,4);

		if(m3==m1) m3.DBG(file,_T("\r\n\r\nSuccess\r\n\r\n"));
		else m3.DBG(file,_T("\r\n\r\nFailed\r\n\r\n"));

		Loop++;

	}
}