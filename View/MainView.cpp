// View/MainView.cpp

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "View/MainView.h"
#include "Controller/AppController.h"

// === 바로 아래 두 줄이 이 문제의 유일하고 정확한 해결책입니다 ===
#include "Model/Aircraft.h" // TADS_B_Aircraft 구조체의 전체 정의를 포함
#include <vector>           // std::vector의 멤버(iterator, begin, end)를 사용하기 위해 포함
// ========================================================

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainView *MainView;
//---------------------------------------------------------------------------
__fastcall TMainView::TMainView(TComponent* Owner)
    : TForm(Owner)
{
    this->controller = nullptr;
    this->g_MouseDownMask = 0;
}
//---------------------------------------------------------------------------
void TMainView::setController(AppController* controller)
{
    this->controller = controller;
}
//---------------------------------------------------------------------------
void TMainView::onModelUpdate()
{
    OpenGLPanel1->Invalidate();
    ListView1->Update();
}
//---------------------------------------------------------------------------
void __fastcall TMainView::FormCreate(TObject *Sender)
{
    if (controller) {
        controller->onFormCreate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::FormDestroy(TObject *Sender)
{
    if (controller) {
        controller->onFormDestroy();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::OpenGLPanel1Paint(TObject *Sender)
{
    if (controller) {
        controller->drawDisplay(OpenGLPanel1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::Timer1Timer(TObject *Sender)
{
    if (controller) {
        controller->onTimerTick();

        ListView1->Items->BeginUpdate();
        ListView1->Items->Clear();

        std::vector<TADS_B_Aircraft> aircraftList;
        controller->getAllAircraft(aircraftList);

        for (std::vector<TADS_B_Aircraft>::iterator it = aircraftList.begin(); it != aircraftList.end(); ++it) {
            const TADS_B_Aircraft& aircraft = *it;
            TListItem *item = ListView1->Items->Add();

            // char[] 배열을 VCL String 타입으로 변환
            item->Caption = AnsiString(aircraft.HexAddr);
            item->SubItems->Add(AnsiString(aircraft.FlightNum));
            item->SubItems->Add(IntToStr((int)aircraft.Altitude));
            item->SubItems->Add(FloatToStrF(aircraft.Speed, ffFixed, 8, 2));
            item->SubItems->Add(FloatToStrF(aircraft.Heading, ffFixed, 8, 2));
            item->SubItems->Add(FloatToStrF(aircraft.Latitude, ffFixed, 8, 6));
            item->SubItems->Add(FloatToStrF(aircraft.Longitude, ffFixed, 8, 6));
        }
        ListView1->Items->EndUpdate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::ConnectButtonClick(TObject *Sender)
{
    if(controller) {
        controller->connect(IPAddress->Text, Port->Text.ToInt());
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::DisconnectButtonClick(TObject *Sender)
{
    if(controller) {
        controller->disconnect();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    if (controller) {
        controller->onMouseDown(Button, Shift, X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::OpenGLPanel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
    if (controller) {
        controller->onMouseMove(Shift, X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::OpenGLPanel1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    if (controller) {
        controller->onMouseUp(Button, Shift, X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainView::Exit1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------