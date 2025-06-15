// AreaDialog.h

#ifndef AreaDialogH
#define AreaDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

//---------------------------------------------------------------------------
// [수정] TArea 구조체 선언 추가
// ListView의 각 항목에 대한 데이터를 저장하기 위한 구조체입니다.
struct TArea {
	UnicodeString Name;
	double min_lat;
	double max_lat;
	double min_lon;
	double max_lon;
};
//---------------------------------------------------------------------------
class TAreaDialog : public TForm
{
__published:	// IDE-managed Components
	TButton *Insert;
	TButton *Delete;
	TButton *Complete;
	TButton *Cancel;
	TEdit *NameEdit;
	TEdit *MinLatEdit;
	TEdit *MaxLatEdit;
	TEdit *MinLonEdit;
	TEdit *MaxLonEdit;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	void __fastcall InsertClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall CompleteClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAreaDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAreaDialog *AreaDialog;
//---------------------------------------------------------------------------
#endif