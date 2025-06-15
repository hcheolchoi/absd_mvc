// ------------------ View/MainView.cpp (이 코드로 전체 교체) ------------------
#include <vcl.h>
#pragma hdrstop
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "MainView.h"
#include "../Controller/AppController.h"
#include "../Model/Aircraft.h"

#pragma package(smart_init)
#pragma link "OpenGLPanel"
#pragma link "cspin"
#pragma resource "*.dfm"

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	// VCL 폼의 기본 생성자
}
//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
	// unique_ptr가 controller_를 자동으로 삭제하므로, 내용은 비워둡니다.
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	// MVC 컨트롤러를 생성합니다. 애플리케이션의 모든 로직은 여기서 시작됩니다.
	controller_ = std::make_unique<AppController>(this);

	// 네트워크 확인용 타이머의 속성을 설정합니다.
	if (NetworkTimer) {
		NetworkTimer->Interval = 200; // 0.2초 간격
		NetworkTimer->Enabled = false; // 처음에는 비활성화
	}

	// 초기 상태 메시지를 표시합니다.
	displayStatus("Ready. Press 'Connect' to start.");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	// 프로그램이 종료될 때 추가로 정리할 작업이 있다면 여기에 작성합니다.
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NetworkTimerTimer(TObject *Sender)
{
    if (controller_) {
        controller_->checkForData();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ConnectButtonClick(TObject *Sender)
{
    if(controller_) {
        controller_->toggleConnection();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AircraftListViewSelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
    if (controller_ && Selected && Item) {
        std::string icao = AnsiString(Item->Caption).c_str();
        controller_->selectAircraft(icao);
    }
}
//---------------------------------------------------------------------------
void TForm1::updateDisplay(const std::vector<const Aircraft*>& aircrafts)
{
    currentAircrafts_ = aircrafts;

    if (AircraftListView) {
		AircraftListView->Items->BeginUpdate();
		AircraftListView->Items->Clear();
		for (const auto& ac : aircrafts) {
			TListItem *item = AircraftListView->Items->Add();
			item->Caption = ac->getIcao24().c_str();
			item->SubItems->Add(ac->getFlightId().c_str());
			item->SubItems->Add(FloatToStr(ac->getLatitude()));
			item->SubItems->Add(FloatToStr(ac->getLongitude()));
			item->SubItems->Add(IntToStr((int)ac->getAltitude()));
		}
		AircraftListView->Items->EndUpdate();
	}

    if (ObjectDisplay) {
        ObjectDisplay->Invalidate();
    }
}
//---------------------------------------------------------------------------
void TForm1::displayStatus(const std::string& message)
{
    if (StatusBar) {
		StatusBar->SimpleText = message.c_str();
	}
}
//---------------------------------------------------------------------------
void TForm1::displaySelectedAircraftDetails(const Aircraft* aircraft)
{
    if (aircraft) {
        UnicodeString details;
        details.sprintf(L"Selected: %s (%s)",
            aircraft->getIcao24().c_str(),
            aircraft->getFlightId().c_str()
        );
        displayStatus(AnsiString(details).c_str());
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ObjectDisplayInit(TObject *Sender)
{
	// 가장 기본적인 OpenGL 설정만 남깁니다.
	if (ObjectDisplay) {
		glViewport(0,0,(GLsizei)ObjectDisplay->Width,(GLsizei)ObjectDisplay->Height);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ObjectDisplayPaint(TObject *Sender)
{
    // 화면을 단색으로 지우는 코드만 남깁니다.
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: 다음 단계에서 지도와 비행기를 그리는 코드를 추가합니다.
}
//---------------------------------------------------------------------------