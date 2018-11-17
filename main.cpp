//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{
    sgApp->ShowSizeGrip = true;
    sgApp->RowHeight = 40;
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btCreateClick(TObject *Sender)
{
	int Y = sgApp->Selected;
	if(Y < 0)
	{
		ShowMessage("�Цb��椤��ܭn�B�z�����@�C");
		return;
	}

	String sDate = sgApp->Cells[0][Y];
	String sProvider = sgApp->Cells[1][Y];
	String sSic = sgApp->Cells[2][Y];
	String sCorr = sgApp->Cells[3][Y];
	String sResp = sgApp->Cells[5][Y];

	/*
	<note n="0003a1101" resp="CBETA" type="add">���iCB�j�A�r�i�j�j</note>
	<app n="0003a1101">
		<lem wit="�iCB�j" resp="CBETA.maha" cb:provider="�Y��(2018-10-01)">��</lem>
		<rdg wit="�i�j�j">�r</rdg>
	</app>
	*/

	if(sProvider.Pos0("CBETA.")<0)
	{
		sProvider = " cb:provider=\"" + sProvider + "(" + sDate
			+ ")\"";
	}
	else
	{
		sProvider = "";
	}

	if(sCorr == "") sCorr = "<space quantity=\"0\"/>";
	if(sSic == "") sSic = "<space quantity=\"0\"/>";

	String sResult = "<note n=\"xxxx\" resp=\"CBETA\" type=\"add\">"
		+ sCorr + "�iCB�j�A" + sSic + "�i�j�j</note>"
		+ "<app n=\"xxxx\">"
		+ "<lem wit=\"�iCB�j\" resp=\"" + sResp + "\"" + sProvider
		+ ">" + sCorr + "</lem>"
		+ "<rdg wit=\"�i�j�j\">" + sSic + "</rdg>"
		+ "</app>";

	edApp->Text = sResult;

	if(sCorr == sSic)
        ShowMessage("ĵ�i : �׭q�e�᪺���e���@��");
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btPasteClick(TObject *Sender)
{

	String sLine = "";

	if(sgApp->Selected < 0)
	{
		ShowMessage("�Цb��椤��ܭn�K�W���_�l��m");
		return;
	}

	_di_IFMXClipboardService service;
	if(TPlatformServices::Current->SupportsPlatformService(__uuidof(IFMXClipboardService)) &&
		(service = TPlatformServices::Current->GetPlatformService(__uuidof(IFMXClipboardService)))) {
		TValue value = service->GetClipboard();
		if(!value.IsEmpty) {
			if(value.IsType<String>()) {
				sLine = value.ToString();
			}
			else
			{
				ShowMessage("�S����r��ƥi�H�K�W");
			}
		}
		else
		{
			ShowMessage("�S����ƥi�H�K�W");
		}
	}

	int X = sgApp->Col;
	int Y = sgApp->Selected;
	int iX = 0;
	int iY = 0;

	String sCell;

	while(sLine != "")
	{
		if(*sLine.FirstChar() == u'\"')
		{
			int iTab = sLine.Pos0("\"\t");
			int iEnter = sLine.Pos0("\"\r\n");

			if((iTab > 0 && iTab < iEnter) || (iTab > 0 && iEnter < 0))
			{
				// ��� tab
				sCell = sLine.SubString0(1,iTab-1);
				if(sgApp->RowCount <= Y+iY) sgApp->RowCount += 10;
				sgApp->Cells[X+iX][Y+iY] = sCell;
				sLine.Delete0(0,iTab+2);
				iX++;
			}
			else if((iEnter > 0 && iEnter < iTab) || (iEnter > 0 && iTab < 0))
			{
				// ��� Enter
				sCell = sLine.SubString0(1,iEnter-1);
				if(sgApp->RowCount <= Y+iY) sgApp->RowCount += 10;
				sgApp->Cells[X+iX][Y+iY] = sCell;
				sLine.Delete0(0,iEnter+3);
				iY++;
				iX=0;
			}
			else
			{
				sgApp->Cells[X+iX][Y+iY] = sLine;
				sLine = "";
			}
		}
		else
		{
			int iTab = sLine.Pos0("\t");
			int iEnter = sLine.Pos0("\r\n");

			if((iTab >= 0 && iTab < iEnter) || (iTab >= 0 && iEnter < 0))
			{
				// ��� tab
				sCell = sLine.SubString0(0,iTab);
				if(sgApp->RowCount <= Y+iY) sgApp->RowCount += 10;
				sgApp->Cells[X+iX][Y+iY] = sCell;
				sLine.Delete0(0,iTab+1);
				iX++;
			}
			else if((iEnter >= 0 && iEnter < iTab) || (iEnter >= 0 && iTab < 0))
			{
				// ��� Enter
				sCell = sLine.SubString0(0,iEnter);
				if(sgApp->RowCount <= Y+iY) sgApp->RowCount += 10;
				sgApp->Cells[X+iX][Y+iY] = sCell;
				sLine.Delete0(0,iEnter+2);
				iY++;
				iX=0;
			}
			else
			{
				if(sgApp->RowCount <= Y+iY) sgApp->RowCount += 10;
				sgApp->Cells[X+iX][Y+iY] = sLine;
				sLine = "";
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btCopyClick(TObject *Sender)
{
	_di_IFMXClipboardService service;
	if(TPlatformServices::Current->SupportsPlatformService(__uuidof(IFMXClipboardService)) &&
		(service = TPlatformServices::Current->GetPlatformService(__uuidof(IFMXClipboardService)))) {
		service->SetClipboard(edApp->Data);
	}
}
//---------------------------------------------------------------------------


