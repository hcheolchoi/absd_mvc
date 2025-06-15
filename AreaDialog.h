// AreaDialog.h

#ifndef AreaDialogH
#define AreaDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ColorGrd.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
// [수정] MainView와 공유할 AreaData 구조체 선언 추가
struct AreaData {
    UnicodeString Name;
    TColor Color;
};
//---------------------------------------------------------------------------
class TAreaConfirm : public TForm
{
__published:	// IDE-managed Components
	TButton *OkButton;
	TButton *CancelButton;
	TEdit *AreaName;
	TColorBox *ColorBox1;
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	// [수정] FormShow 이벤트 핸들러 선언 추가
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TAreaConfirm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAreaConfirm *AreaConfirm;
//---------------------------------------------------------------------------
#endif