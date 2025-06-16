//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AreaDialog.h"
// #include "MainView.h" // MainView.h 포함 구문 제거!
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAreaDialog *AreaDialog;
//---------------------------------------------------------------------------
__fastcall TAreaDialog::TAreaDialog(TComponent* Owner)
	: TForm(Owner), FIsEditMode(false)
{
}
//---------------------------------------------------------------------------
void TAreaDialog::SetupDialog(bool isEditMode, const UnicodeString& name, TColor color)
{
    FIsEditMode = isEditMode;
    InOutName = name;
    InOutColor = color;
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::FormShow(TObject *Sender)
{
    // 폼이 표시될 때, Controller로부터 전달받은 데이터로 컨트롤 초기화
    if (FIsEditMode) {
        this->Caption = "Edit Area";
        AreaName->Text = InOutName;
        ColorBox1->Selected = InOutColor;
    }
    else {
        this->Caption = "Insert New Area";
        AreaName->Text = "New Area";
        ColorBox1->Selected = clRed;
    }
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::OkButtonClick(TObject *Sender)
{
    if (AreaName->Text.IsEmpty()) {
        ShowMessage("Area Name is required.");
        return;
    }
    // 사용자가 입력한 최종 결과를 다시 public 멤버에 저장
    this->InOutName = AreaName->Text;
    this->InOutColor = ColorBox1->Selected;

    // Controller에게 성공적으로 작업이 끝났음을 알리고 창을 닫음
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::CancelButtonClick(TObject *Sender)
{
    ModalResult = mrCancel;
}
//---------------------------------------------------------------------------