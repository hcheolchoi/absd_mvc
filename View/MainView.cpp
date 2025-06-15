// View/MainView.cpp

#include <vcl.h>
#pragma hdrstop

#include "MainView.h"
#include "AppController.h"
#include <gl\gl.h>
#include <gl\glu.h>

// =========================================================================
// LatLonConv 클래스 직접 삽입 시작
// -------------------------------------------------------------------------
// 설명: 컴파일러가 LatLonConv.h를 찾지 못하는 문제를 해결하기 위해,
//       클래스 선언과 구현을 이 파일에 직접 포함시킵니다.
// =========================================================================

class LatLonConv {
public:
    // 위도, 경도 좌표를 화면의 X, Y 좌표로 변환하는 static 함수
    static void convert(double lat, double lon, double& x, double& y, int width, int height);
};

// LatLonConv 클래스의 convert 함수 구현
void LatLonConv::convert(double lat, double lon, double& x, double& y, int width, int height) {
    // 간단한 구면 좌표계-평면 좌표계 변환 공식 (Mercator projection 유사)
    x = (lon + 180.0) * (static_cast<double>(width) / 360.0);
    y = (90.0 - lat) * (static_cast<double>(height) / 180.0);
}

// =========================================================================
// LatLonConv 클래스 직접 삽입 끝
// =========================================================================


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	theController = new AppController(this);
	theController->start();
}

//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
	delete theController;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Update()
{
	Memo1->Text = AnsiString(theController->getSelectedAircraftInfoText().c_str());
	OpenGLPanel1->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (theController)
	{
		theController->onTimer();
	}
}

//---------------------------------------------------------------------------
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

//---------------------------------------------------------------------------
void TForm1::DrawAircrafts()
{
	if (!theController) return;

	int count = theController->getAircraftCount();

	for (int i = 0; i < count; ++i)
	{
		AircraftInfo acInfo = theController->getAircraftInfo(i);

		if (acInfo.icao != 0)
		{
			double screenX, screenY;
			// 이제 이 함수 호출은 같은 파일 안에 있는 LatLonConv 클래스를 사용하므로
			// 절대적으로 안전합니다.
			LatLonConv::convert(acInfo.lat, acInfo.lon, screenX, screenY,
								OpenGLPanel1->Width, OpenGLPanel1->Height);

			if (acInfo.selected) {
				glColor3f(1.0, 1.0, 0.0); // Yellow
			} else {
				glColor3f(0.0, 1.0, 0.0); // Green
			}

			glBegin(GL_QUADS);
			glVertex2f(screenX - 5, screenY - 5);
			glVertex2f(screenX + 5, screenY - 5);
			glVertex2f(screenX + 5, screenY + 5);
			glVertex2f(screenX - 5, screenY + 5);
			glEnd();
		}
	}
}
//---------------------------------------------------------------------------
void TForm1::DrawLabels()
{
}

//---------------------------------------------------------------------------
void __fastcall TForm1::OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	if (!theController) return;

	int count = theController->getAircraftCount();
	unsigned int selectedIcao = 0;

	for (int i = 0; i < count; ++i)
	{
		AircraftInfo acInfo = theController->getAircraftInfo(i);

		if(acInfo.icao != 0)
		{
			double screenX, screenY;
			LatLonConv::convert(acInfo.lat, acInfo.lon, screenX, screenY,
								OpenGLPanel1->Width, OpenGLPanel1->Height);

			if (X >= screenX - 5 && X <= screenX + 5 && Y >= screenY - 5 && Y <= screenY + 5)
			{
				selectedIcao = acInfo.icao;
				break;
			}
		}
	}
	theController->selectAircraft(selectedIcao);
}
//---------------------------------------------------------------------------