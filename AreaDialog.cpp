// AreaDialog.cpp

#include <vcl.h>
#pragma hdrstop

#include "AreaDialog.h"
#include "MainView.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
// [수정] 전역 변수와 클래스 이름을 TAreaConfirm으로 변경
TAreaConfirm *AreaConfirm;
//---------------------------------------------------------------------------
__fastcall TAreaConfirm::TAreaConfirm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAreaConfirm::OkButtonClick(TObject *Sender)
{
    if (AreaName->Text.IsEmpty()) {
        ShowMessage("Area Name is required.");
        return;
    }

    // MainView의 리스트에서 항목이 선택된 상태라면 '수정' 모드
    if (Form1->AreaListView->Selected) {
        // 기존에 저장된 AreaData 포인터를 가져와 내용만 수정
        AreaData *area = (AreaData*)Form1->AreaListView->Selected->Data;
        area->Name = AreaName->Text;
        area->Color = ColorBox1->Selected;

        // 화면에 보이는 리스트 항목의 캡션도 업데이트
        Form1->AreaListView->Selected->Caption = area->Name;
    }
    // 선택된 항목이 없다면 '추가' 모드
    else {
        // 새로운 AreaData 객체를 동적으로 생성
        AreaData *area = new AreaData;
        area->Name = AreaName->Text;
        area->Color = ColorBox1->Selected;

        TListItem *item = Form1->AreaListView->Items->Add();
        item->Caption = area->Name;
        // Data 속성에 생성한 객체의 포인터를 저장
        item->Data = area;
    }

	ModalResult = mrOk; // OK 버튼을 눌렀음을 알리고 창을 닫음
}
//---------------------------------------------------------------------------
void __fastcall TAreaConfirm::CancelButtonClick(TObject *Sender)
{
	ModalResult = mrCancel; // Cancel 버튼을 눌렀음을 알리고 창을 닫음
}
//---------------------------------------------------------------------------
void __fastcall TAreaConfirm::FormShow(TObject *Sender)
{
    // 폼이 화면에 표시될 때 호출됨
    // 만약 MainView의 리스트에서 항목을 선택한 상태로 이 창을 열었다면,
    // 해당 항목의 정보로 컨트롤들을 초기화합니다.
    if (Form1->AreaListView->Selected) {
        this->Caption = "Edit Area";
        AreaData* area = (AreaData*)Form1->AreaListView->Selected->Data;
        AreaName->Text = area->Name;
        ColorBox1->Selected = area->Color;
    }
    // 새 항목 추가를 위해 창을 열었다면, 기본값으로 설정합니다.
    else {
        this->Caption = "Insert New Area";
        AreaName->Text = "New Area";
        ColorBox1->Selected = clRed;
    }
}
//---------------------------------------------------------------------------