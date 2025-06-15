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
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    this->controller = nullptr;
    this->g_MouseDownMask = 0;
}
//---------------------------------------------------------------------------
void TForm1::setController(AppController* controller)
{
    this->controller = controller;
}
//---------------------------------------------------------------------------
void TForm1::onModelUpdate()
{
    OpenGLPanel1->Invalidate();
    ListView1->Update();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    if (controller) {
        controller->onFormCreate();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    if (controller) {
        controller->onFormDestroy();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenGLPanel1Paint(TObject *Sender)
{
    if (controller) {
        controller->drawDisplay(OpenGLPanel1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
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
void __fastcall TForm1::ConnectButtonClick(TObject *Sender)
{
    if(controller) {
        controller->connect(IPAddress->Text, Port->Text.ToInt());
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DisconnectButtonClick(TObject *Sender)
{
    if(controller) {
        controller->disconnect();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    if (controller) {
        controller->onMouseDown(Button, Shift, X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenGLPanel1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
    if (controller) {
        controller->onMouseMove(Shift, X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenGLPanel1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
    if (controller) {
        controller->onMouseUp(Button, Shift, X, Y);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Exit1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------