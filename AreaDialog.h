//---------------------------------------------------------------------------
#ifndef AreaDialogH
#define AreaDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
// MainView에 대한 의존성 제거

class TAreaDialog : public TForm // 클래스 이름 변경 권장
{
__published:
	TButton *OkButton;
	TButton *CancelButton;
	TEdit *AreaName;
	TColorBox *ColorBox1;
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:
    // Controller로부터 데이터를 받기 위한 변수
    bool FIsEditMode;

public:
    // Controller와 데이터를 주고받기 위한 public 멤버
    UnicodeString InOutName;
    TColor InOutColor;

	__fastcall TAreaDialog(TComponent* Owner);

    // Controller가 이 함수를 통해 수정 모드 여부와 초기 데이터를 전달
    void SetupDialog(bool isEditMode, const UnicodeString& name, TColor color);
};
//---------------------------------------------------------------------------
extern PACKAGE TAreaDialog *AreaDialog;
//---------------------------------------------------------------------------
#endif