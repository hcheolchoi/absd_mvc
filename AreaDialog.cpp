// AreaDialog.cpp

#include <vcl.h>
#pragma hdrstop

#include "AreaDialog.h"
#include "MainView.h" // [수정] 'Form1' undeclared identifier 오류 해결

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAreaDialog *AreaDialog;
//---------------------------------------------------------------------------
__fastcall TAreaDialog::TAreaDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::InsertClick(TObject *Sender)
{
	if (NameEdit->Text.IsEmpty()) {
		ShowMessage("Name is required.");
		return;
	}

	// [수정] TArea 타입 오류 해결
	TArea *area = new TArea;
	area->Name = NameEdit->Text;
	area->min_lat = StrToFloat(MinLatEdit->Text);
	area->max_lat = StrToFloat(MaxLatEdit->Text);
	area->min_lon = StrToFloat(MinLonEdit->Text);
	area->max_lon = StrToFloat(MaxLonEdit->Text);

	TListItem *item = Form1->AreaListView->Items->Add();
	item->Caption = area->Name;
	item->SubItems->Add(FloatToStr(area->min_lat));
	item->SubItems->Add(FloatToStr(area->max_lat));
	item->SubItems->Add(FloatToStr(area->min_lon));
	item->SubItems->Add(FloatToStr(area->max_lon));
	item->Data = area;
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::DeleteClick(TObject *Sender)
{
	if (Form1->AreaListView->Selected)
	{
		TArea *area = (TArea*)Form1->AreaListView->Selected->Data;
		delete area;
		Form1->AreaListView->Selected->Delete();
	}
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::CompleteClick(TObject *Sender)
{
    if (Form1->AreaListView->Selected) {
		TArea *area = (TArea*)Form1->AreaListView->Selected->Data;
		area->Name = NameEdit->Text;
		area->min_lat = StrToFloat(MinLatEdit->Text);
		area->max_lat = StrToFloat(MaxLatEdit->Text);
		area->min_lon = StrToFloat(MinLonEdit->Text);
		area->max_lon = StrToFloat(MaxLonEdit->Text);

		Form1->AreaListView->Selected->Caption = area->Name;
		Form1->AreaListView->Selected->SubItems->Strings[0] = FloatToStr(area->min_lat);
		Form1->AreaListView->Selected->SubItems->Strings[1] = FloatToStr(area->max_lat);
		Form1->AreaListView->Selected->SubItems->Strings[2] = FloatToStr(area->min_lon);
		Form1->AreaListView->Selected->SubItems->Strings[3] = FloatToStr(area->max_lon);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAreaDialog::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TAreaDialog::FormShow(TObject *Sender)
{
	// [수정] TArea 타입 오류 해결
	if (Form1->AreaListView->Selected) {
		TArea *area = (TArea*)Form1->AreaListView->Selected->Data;
		NameEdit->Text = area->Name;
		MinLatEdit->Text = FloatToStr(area->min_lat);
		MaxLatEdit->Text = FloatToStr(area->max_lat);
		MinLonEdit->Text = FloatToStr(area->min_lon);
		MaxLonEdit->Text = FloatToStr(area->max_lon);
	}
}
//---------------------------------------------------------------------------