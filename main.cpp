//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmMain *fmMain;

using namespace std;

//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{
    sgApp->ShowSizeGrip = true;
	sgApp->RowHeight = 40;
	LoadVerTxt();   // ���J�����C��, �榡�� A=����
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::btCreateClick(TObject *Sender)
{
	int Y = sgApp->Selected;
	if(Y < 0)
	{
		TDialogService::ShowMessage("�Цb��椤��ܭn�B�z�����@�C");
		return;
	}

	String sDate = sgApp->Cells[0][Y];
	String sProvider = sgApp->Cells[1][Y];
	String sSic = sgApp->Cells[2][Y];
	String sCorr = sgApp->Cells[3][Y];
	TStringList * slCf = new TStringList;
	slCf->Text = sgApp->Cells[4][Y];
	String sResp = sgApp->Cells[5][Y];

	String sID = edID->Text;            // �յ��s��
	String sVer = "";	// ����, �w�]�O�j���ái�j�j

	// �ǤJID , �i��O T01n0001_p0001a0101
	// T �Ǧ^�i�j�j, sID �|�Ǧ^ 0001a0101
	CheckIDVer(sID, sVer);

	String sRefVer = edRefVer->Text;	// �ѦҪ����i�R-CB�j

	String sCf = Check_Cf(slCf);

	/*
	<note n="0003a1101" resp="CBETA" type="add">���iCB�j�i�R-CB�j�A�r�i�j�j</note>
	<app n="0003a1101">
		<lem wit="�iCB�j�i�R-CB�j" resp="CBETA.maha" cb:provider="�Y�� (2018-10-01)">��
		<note type="cf1">K17n0647_p0839b07</note>
		<note type="cf2">T01n0001_p0026c15</note>
		</lem>
		<rdg wit="�i�j�j">�r</rdg>
	</app>
	*/

	if(sProvider == "" || sProvider.Pos0("CBETA.")>=0)
	{
		sProvider = "";
	}
	else
	{
		sProvider = " cb:provider=\"" + sProvider;
		if(sDate != "")	sProvider += " (" + sDate + ")";
		sProvider += u"\"";
	}

	if(sCorr == "") sCorr = "<space quantity=\"0\"/>";
	if(sSic == "") sSic = "<space quantity=\"0\"/>";

	String sResult = "<note n=\"" + sID + "\" resp=\"CBETA\" type=\"add\">"
		+ sCorr + "�iCB�j" + sRefVer + "�A" + sSic + sVer + "</note>"
		+ "<app n=\"" + sID + "\">"
		+ "<lem wit=\"�iCB�j" + sRefVer + "\" resp=\"" + sResp + "\"" + sProvider
		+ ">" + sCorr + sCf + "</lem>"
		+ "<rdg wit=\"" + sVer + "\">" + sSic + "</rdg>"
		+ "</app>";

	edApp->Text = sResult;

	if(sCorr == sSic)
		TDialogService::ShowMessage(u"ĵ�i : �׭q�e�᪺���e���@��");
	if(sID.Length() != 9)
		TDialogService::ShowMessage(u"ĵ�i : �s�����פ��O 9");
	if(sID == "xxxxxxxxx")
		TDialogService::ShowMessage(u"ĵ�i : �S���s��");
	if(sResp == "")
		TDialogService::ShowMessage(u"ĵ�i : �S���׭q�T�{��");


	if(sProvider != "")
	{
		TRegEx * regex = new TRegEx();
		if(regex->Matches(sDate, "^\\d{4}\\-\\d{2}\\-\\d{2}$").Count == 0)
		{
			TDialogService::ShowMessage(u"ĵ�i : ����榡����");
		}
	}

	if(slCf) delete slCf;
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btPasteClick(TObject *Sender)
{

	String sLine = "";

	if(sgApp->Selected < 0)
	{
		TDialogService::ShowMessage("�Цb��椤��ܭn�K�W���_�l��m");
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
				TDialogService::ShowMessage(u"�S����r��ƥi�H�K�W");
			}
		}
		else
		{
			TDialogService::ShowMessage(u"�S����ƥi�H�K�W");
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
// �B�z�׭q�̾�
// �쥻�i��O�o��

// cf1: K17n0647_p0839b07
// cf2 : T01n0001_p0026c15

// �B�z���o��
// <note type="cf1">K17n0647_p0839b07</note><note type="cf2">T01n0001_p0026c15</note>
String __fastcall TfmMain::Check_Cf(TStringList * slCf)
{
	String sCf = "";
	for(int i=0; i<slCf->Count; i++)
	{
		String sString = slCf->Strings[i];

		String sPatten = "(cf\\d)\\s*[:�G]\\s*(.*)";
		TRegEx *regex;
		TMatchCollection reMatch;
		//TGroupCollection reGroup;

		regex = new TRegEx();
		reMatch = regex->Matches(sString, sPatten);

		if(reMatch.Count > 0)
		{
			String scf = reMatch.Item[0].Groups.Item[1].Value;
			String sLineHead = reMatch.Item[0].Groups.Item[2].Value;
			// <note type="cf1">K17n0647_p0839b07</note>
			sCf += "<note type=\"" + scf + "\">"+ sLineHead +"</note>";
		}

		// reMatch �O������쪺
		// reGroup = reMatch.Item[i].Groups �O�Y�@�է�쪺
		// reGroup.Item[j].Value �N���� perl $0, $1, $2 ....

	}
	return sCf;
}
//---------------------------------------------------------------------------
// �ǤJID , �i��O T01n0001_p0001a0101
// T �Ǧ^�i�j�j, sID �|�Ǧ^ 0001a0101
void __fastcall TfmMain::CheckIDVer(String &sID, String &sVer)
{
	String sStr = sID;
	String sPatten = "^(\\D+).*?p(.*)";
	TRegEx *regex;
	TMatchCollection reMatch;
	TGroupCollection reGroup;

	regex = new TRegEx();
	reMatch = regex->Matches(sStr, sPatten);

	// ���зǦ歺 T01n0001_p0123a0101
	if(reMatch.Count > 0)
	{
		sVer = reMatch.Item[0].Groups.Item[1].Value;    // T
		sID = reMatch.Item[0].Groups.Item[2].Value;

		if(mapVer.count(sVer))
			sVer = mapVer[sVer];    // �i�j�j
		else
		{
			sVer = u"�i" + sVer + u"�j";
			TDialogService::ShowMessage(u"�L�k�P�_���յ�����l����");
		}
	}
	else
	{
		sVer = u"�i�j�j";
		if(sStr == "") sID = "xxxxxxxxx";
		else sID = sStr;
	}
}
//---------------------------------------------------------------------------
// ���J�����C��, �榡�� A=����
void __fastcall TfmMain::LoadVerTxt()
{
	TStringList * slVer = new TStringList;
	slVer->LoadFromFile("verlist.txt",TEncoding::UTF8);

	for(int i=0; i<slVer->Count; i++)
	{
		String sStr = slVer->Strings[i];
		int iPos = sStr.Pos0(u"=");
		String sID = sStr.SubString0(0,iPos);
		sStr.Delete0(0,iPos+1);
		sID = sID.Trim();
		sStr = sStr.Trim();
		mapVer[sID] = sStr;
	}
}
//---------------------------------------------------------------------------


