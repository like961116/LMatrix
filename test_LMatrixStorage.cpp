#include "test_LMatrixStorage.h"

void Test_LMatrixStorage_InsertRows_DeleteRows()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrixStorage_InsertRows_DeleteRows.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	LMatrixStorage ms1;
	ms1.SetBase(1);
	ms1.SetRow(2);
	ms1.SetCol(5);
	ms1.InitTestValue();
	ms1.DBG(file,_T("\r\nInit Value Of ms1=\r\n"));
	ms1.Save(file,8,0);
	ms1.DBG(file,_T("\r\n\r\n"));

	LMatrixStorage ms;
	ms=ms1;
	ms.DBG(file,_T("\r\nInit Value Of ms=\r\n"));
	ms.Save(file,8,0);
	ms.DBG(file,_T("\r\n\r\n"));

	CString szText;
	for(int i=-2;i<5;i++)
	{
		ms.InsertRows(ms1,i);
		szText.Format(_T("\r\n插入ms1之后——ms.InsertRows(ms1,%d)，数组ms=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,8,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	for(int i=-2;i<15;i++)
	{
		ms.DeleteRows(i,2);
		szText.Format(_T("\r\n删除2行之后——ms.DeleteRows(%d,2)，数组ms=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,8,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	file.Close();
}


void Test_LMatrixStorage_Sub()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrixStorage_Sub.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	LMatrixStorage ms1;
	ms1.SetBase(1);
	ms1.SetRow(9);
	ms1.SetCol(9);
	ms1.InitTestValue();
	ms1.DBG(file,_T("\r\nInit Value Of ms1=\r\n"));
	ms1.Save(file,8,0);
	ms1.DBG(file,_T("\r\n\r\n"));

	LMatrixStorage ms;

	CString szText;

	for(int i=-2;i<11;i++)
	{
		for(int j=-2;j<11;j++)
		{
			ms=ms1.Sub(i,j,2,3);
			
			szText.Format(_T("\r\n——ms1.Sub(%d,%d,2,3)=\r\n"),i,j);
			ms.DBG(file,szText);
			ms.Save(file,8,0);
			ms.DBG(file,_T("\r\n\r\n"));
		}
	}

	for(int i=-2;i<12;i++)
	{
		for(int j=-2;j<13;j++)
		{
			ms=ms1.Sub(i,j,-2,-3);
			
			szText.Format(_T("\r\n——ms1.Sub(%d,%d,-2,-3)=\r\n"),i,j);
			ms.DBG(file,szText);
			ms.Save(file,8,0);
			ms.DBG(file,_T("\r\n\r\n"));
		}
	}

	file.Close();
}

void Test_LMatrixStorage_InsertCols_DeleteCols()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrixStorage_InsertCols_DeleteCols.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	LMatrixStorage ms1;
	ms1.SetBase(1);
	ms1.SetRow(5);
	ms1.SetCol(2);
	ms1.InitTestValue();
	ms1.DBG(file,_T("\r\nInit Value Of ms1=\r\n"));
	ms1.Save(file,3,0);
	ms1.DBG(file,_T("\r\n\r\n"));

	LMatrixStorage ms;
	ms=ms1;
	ms.DBG(file,_T("\r\nInit Value Of ms=\r\n"));
	ms.Save(file,3,0);
	ms.DBG(file,_T("\r\n\r\n"));

	CString szText;
	for(int i=-2;i<5;i++)
	{
		ms.InsertCols(ms1,i);
		szText.Format(_T("\r\n插入ms1之后——ms.InsertCols(ms1,%d)，数组ms=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,3,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	for(int i=-2;i<15;i++)
	{
		ms.DeleteCols(i,2);
		szText.Format(_T("\r\n删除2列之后——ms.DeleteCols(%d,2)，数组ms=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,3,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	file.Close();
}

void Test_LMatrixStorage_SetRow()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrixStorage_SetRow.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	LMatrixStorage ms1;
	ms1.SetBase(1);
	ms1.SetRow(5);
	ms1.SetCol(2);
	ms1.InitTestValue();

	ms1.DBG(file,_T("\r\nInit Value Of ms1=\r\n"));
	ms1.Save(file,3,0);
	ms1.DBG(file,_T("\r\n\r\n"));

	LMatrixStorage ms;
	ms=ms1;
	ms.DBG(file,_T("\r\nInit Value Of ms=\r\n"));
	ms.Save(file,3,0);
	ms.DBG(file,_T("\r\n\r\n"));

	CString szText;
	for(int i=5;i>-3;i--)
	{
		ms.SetRow(i);
//		ms.InitTestValue();
		szText.Format(_T("\r\n设置行数之后——ms.SetRow(%d)，数组ms=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,3,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	file.Close();
} 


void Test_LMatrixStorage_SetCol()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrixStorage_SetCol.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	LMatrixStorage ms1;
	ms1.SetBase(1);
	ms1.SetRow(3);
	ms1.SetCol(5);
	ms1.InitTestValue();

	ms1.DBG(file,_T("\r\nInit Value Of ms1=\r\n"));
	ms1.Save(file,3,0);
	ms1.DBG(file,_T("\r\n\r\n"));

	LMatrixStorage ms;
	ms=ms1;
	ms.DBG(file,_T("\r\nInit Value Of ms=\r\n"));
	ms.Save(file,3,0);
	ms.DBG(file,_T("\r\n\r\n"));

	CString szText;
	for(int i=7;i>-3;i--)
	{
		ms.SetCol(i);
//		ms.InitTestValue();
		szText.Format(_T("\r\n设置列数之后——ms.SetCol(%d)，数组ms=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,3,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	file.Close();
} 

void Test_LMatrixStorage_Rows()
{
	CFile file;
	file.Open(_T("c:\\Test_LMatrixStorage_Rows.txt"),CFile::modeWrite|CFile::modeCreate);
	CArchive ar(&file,CArchive::store);	char szFFFE[2];	szFFFE[0]='\xFF';	szFFFE[1]='\xFE';	ar.Write(szFFFE,2);	ar.Close();

	LMatrixStorage ms1;
	ms1.SetBase(1);
	ms1.SetRow(5);
	ms1.SetCol(3);
	ms1.InitTestValue();

	ms1.DBG(file,_T("\r\nInit Value Of ms1=\r\n"));
	ms1.Save(file,3,0);
	ms1.DBG(file,_T("\r\n\r\n"));
	
	LMatrixStorage ms;
	
	CString szText;
	for(int i=-2;i<7;i++)
	{
		ms=ms1.Rows(i,5);
		szText.Format(_T("\r\nms1.Rows(%d,5)=\r\n"),i);
		ms.DBG(file,szText);
		ms.Save(file,3,0);
		ms.DBG(file,_T("\r\n\r\n"));
	}

	file.Close();
} 