// View/MainView.cpp

#include <vcl.h>
#pragma hdrstop

#include "MainView.h"
#include "Controller/AppController.h"
#include "AreaDialog.h" // 파일 이름은 AreaDialog.h, 내용은 TAreaConfirm
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>

// LatLonConv 클래스 직접 삽입
class LatLonConv {
public:
    static void convert(double lat, double lon, double& x, double& y, int width, int height);
};
void LatLonConv::convert(double lat, double lon, double& x, double& y, int width, int height) {
    x = (lon + 180.0) * (static_cast<double>(width) / 360.0);
    y = (90.0 - lat) * (static_cast<double>(height) / 180.0);
}

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner), theController(nullptr) {}

__fastcall TForm1::~TForm1()
{
	delete theController;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	theController = new AppController(this);
	theController->start();
}

void TForm1::onModelUpdate()
{
	if(theController) {
		Memo1->Text = AnsiString(theController->getSelectedAircraftInfoText().c_str());
	}
	OpenGLPanel1->Invalidate();
}

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
}

void __fastcall TForm1::ConnectButtonClick(TObject *Sender)
{
	if (theController)
	{
		std::string ip = AnsiString(IPAddress->Text).c_str();
		int port = StrToIntDef(Port->Text, 30003);
		theController->connectToServer(ip, port);
	}
}

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
    Close();
}

void __fastcall TForm1::InsertClick(TObject *Sender)
{
    if(AreaListView->Selected)
        AreaListView->Selected->Selected = false;
    AreaConfirm->ShowModal();
}

void __fastcall TForm1::DeleteClick(TObject *Sender)
{
    if(AreaListView->Selected)
    {
        // [수정] TArea 대신 AreaData 사용
        AreaData *area = (AreaData*)AreaListView->Selected->Data;
        delete area;
        AreaListView->Selected->Delete();
    }
}

void __fastcall TForm1::CompleteClick(TObject *Sender)
{
    if(AreaListView->Selected) {
        AreaConfirm->ShowModal();
    }
}

void __fastcall TForm1::CancelClick(TObject *Sender)
{
    Close();
}

void __fastcall TForm1::OpenGLPanel1Paint(TObject *Sender)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, OpenGLPanel1->Width, OpenGLPanel1->Height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawAircrafts();
	DrawLabels();

	SwapBuffers(GetDC(OpenGLPanel1->Handle));
}

void TForm1::DrawAircrafts()
{
	if (!theController) return;
	std::vector<AircraftInfo> aircraftToDraw = theController->getAllAircraftInfo();
	for (const auto& acInfo : aircraftToDraw)
	{
		double screenX, screenY;
		LatLonConv::convert(acInfo.lat, acInfo.lon, screenX, screenY, OpenGLPanel1->Width, OpenGLPanel1->Height);
		if (acInfo.selected) { glColor3f(1.0, 1.0, 0.0); } else { glColor3f(0.0, 1.0, 0.0); }
		glBegin(GL_QUADS);
		glVertex2f(screenX - 5, screenY - 5); glVertex2f(screenX + 5, screenY - 5);
		glVertex2f(screenX + 5, screenY + 5); glVertex2f(screenX - 5, screenY + 5);
		glEnd();
	}
}

void TForm1::DrawLabels() {}

void __fastcall TForm1::OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (!theController) return;
	unsigned int selectedIcao = 0;
	std::vector<AircraftInfo> allAircraft = theController->getAllAircraftInfo();
	for (const auto& acInfo : allAircraft)
	{
		double screenX, screenY;
		LatLonConv::convert(acInfo.lat, acInfo.lon, screenX, screenY, OpenGLPanel1->Width, OpenGLPanel1->Height);
		if (X >= screenX - 5 && X <= screenX + 5 && Y >= screenY - 5 && Y <= screenY + 5)
		{
			selectedIcao = acInfo.icao;
			break;
		}
	}
	theController->selectAircraft(selectedIcao);
}

void __fastcall TForm1::ObjectDisplayInit(TObject *Sender)
{
    // 이 이벤트 핸들러에 필요한 초기화 코드가 있다면 여기에 작성합니다.
    // 현재는 비워두어도 오류는 해결됩니다.
}
void __fastcall TForm1::RenderTimerTimer(TObject *Sender)
{
    // Controller->Update(); // 주석 처리됨
    // ObjectDisplay->Render(); // <-- 타이머가 돌 때마다 OpenGLPanel의 Render 함수를 호출
    // ObjectDisplay->SwapBuffers(); // <-- 더블 버퍼링으로 화면에 표시
}