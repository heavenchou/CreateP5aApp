//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Rtti.hpp>
#include <FMX.Memo.hpp>
#include "System.RegularExpressions.hpp"
#include <FMX.DialogService.hpp>
#include <map>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *sgApp;
	TEdit *edApp;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
	TStringColumn *StringColumn8;
	TPanel *Panel1;
	TButton *btCreate;
	TButton *btCopy;
	TButton *btPaste;
	TLabel *Label1;
	TEdit *edID;
	TEdit *edRefVer;
	TLabel *Label2;
	void __fastcall btCreateClick(TObject *Sender);
	void __fastcall btPasteClick(TObject *Sender);
	void __fastcall btCopyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	std::map <String, String> mapVer;
	void __fastcall LoadVerTxt();   // 載入版本列表, 格式為 A=金藏

	// 傳入ID , 可能是 T01n0001_p0001a0101
	// T 傳回【大】, sID 會傳回 0001a0101
	void __fastcall CheckIDVer(String &sID, String &sVer);

	String __fastcall Check_Cf(TStringList * slCf); // 處理修訂依據
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
