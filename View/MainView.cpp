//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainView.h"
#include "Controller/AppController.h"
#include "Model/AircraftModel.h"
#include "Utils/ntds2d.h"
#include "Map/MapSrc/FlatEarthView.h" // 지도 관련 헤더
#include "Map/MapSrc/Texture.h"      // 지도 관련 헤더

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma link "OpenGLPanel"
#pragma resource "*.dfm"
TMainViewForm *VCL_MainView;
//---------------------------------------------------------------------------
__fastcall TMainViewForm::TMainViewForm(TComponent* Owner)
    : TForm(Owner), controller(nullptr), model(nullptr), isPanning(false), g_EarthView(nullptr)
{
}
//---------------------------------------------------------------------------
void TMainViewForm::SetController(AppController* ctrl)
{
    this->controller = ctrl;
}
//---------------------------------------------------------------------------
void TMainViewForm::SetModel(AircraftModel* mdl)
{
    this->model = mdl;
}
//---------------------------------------------------------------------------
void TMainViewForm::Update()
{
    // Model이 변경되었음을 알림 -> 화면을 다시 그리도록 요청
    OpenGLPanel1->Invalidate();
}
//---------------------------------------------------------------------------
void TMainViewForm::FormCreate(TObject *Sender)
{
    // 원본의 지도 초기화 로직 수행
    g_EarthView = new FlatEarthView();
    g_EarthView->SetScreenSize(OpenGLPanel1->Width, OpenGLPanel1->Height);
    g_EarthView->SetViewCenter(40.73, -80.33); // 예시: 피츠버그 근처
    g_EarthView->SetZoom(8);

    InitializeOpenGL();
    InitNTDS(); // ntds2d.cpp의 심볼 초기화
}
//---------------------------------------------------------------------------
void TMainViewForm::FormDestroy(TObject *Sender)
{
    delete g_EarthView;
    ReleaseNTDS(); // ntds2d.cpp의 리소스 해제
}
//---------------------------------------------------------------------------
void TMainViewForm::InitializeOpenGL()
{
    OpenGLPanel1->MakeCurrent();
    // 기본적인 OpenGL 상태 설정
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f); // 어두운 파란색 배경
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
//---------------------------------------------------------------------------
void TMainViewForm::OpenGLPanel1Paint(TObject *Sender)
{
    OpenGLPanel1->MakeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 뷰포트 및 프로젝션 설정
    g_EarthView->SetScreenSize(OpenGLPanel1->Width, OpenGLPanel1->Height);
    glViewport(0, 0, OpenGLPanel1->Width, OpenGLPanel1->Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, OpenGLPanel1->Width, 0, OpenGLPanel1->Height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    DrawMap();
    DrawAircraft();

    OpenGLPanel1->SwapBuffers();
}
//---------------------------------------------------------------------------
void TMainViewForm::DrawMap()
{
    // 원본의 지도 그리기 로직 (g_EarthView, TileManager 등 사용)
    // 이 부분은 MapSrc 라이브러리와 연동하여 구현 필요
}
//---------------------------------------------------------------------------
void TMainViewForm::DrawAircraft()
{
    if (!model || !g_EarthView) return;

    std::vector<TADS_B_Aircraft*> aircraftList = model->GetAllAircraft();
    uint32_t selectedICAO = model->GetSelectedAircraftICAO();

    for (const auto& ac : aircraftList) {
        if (ac->HaveLatLon) {
            int screenX, screenY;
            // 지리적 좌표를 화면 좌표로 변환
            g_EarthView->WorldToScreen(ac->Latitude, ac->Longitude, screenX, screenY);

            // 화면 밖이면 그리지 않음
            if (screenX < 0 || screenX > OpenGLPanel1->Width || screenY < 0 || screenY > OpenGLPanel1->Height) {
                continue;
            }

            // 원본의 ntds2d.cpp 함수를 사용하여 항공기 심볼 그리기
            DrawAirTrack(screenX, screenY, ac->Heading, UNKNOWN_FRIEND);

            // 선택된 항공기일 경우 특별히 표시
            if (ac->ICAO == selectedICAO) {
                DrawTrackHook(screenX, screenY);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button == mbLeft) {
        isPanning = true;
        lastMouseX = X;
        lastMouseY = Y;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::OpenGLPanel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    if (isPanning && controller) {
        int deltaX = X - lastMouseX;
        int deltaY = Y - lastMouseY;
        // 로직을 직접 처리하지 않고 Controller에 위임
        controller->HandleMapPan(deltaX, deltaY);
        lastMouseX = X;
        lastMouseY = Y;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::OpenGLPanel1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (isPanning) {
        isPanning = false;
        // 드래그가 아닌 단순 클릭이었는지 확인
        if (std::abs(X - lastMouseX) < 5 && std::abs(Y - lastMouseY) < 5) {
            if (controller) {
                // 항공기 선택 로직을 Controller에 위임
                controller->HandleAircraftSelection(X, OpenGLPanel1->Height - Y);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainViewForm::RenderTimerTimer(TObject *Sender)
{
	// 주기적으로 오래된 항공기 제거 및 화면 갱신
	if(model) model->RemoveStaleAircraft();
	OpenGLPanel1->Invalidate();
}
//---------------------------------------------------------------------------
void TMainViewForm::PanMap(int deltaX, int deltaY)
{
    if (g_EarthView) {
        g_EarthView->MoveView(deltaX, -deltaY); // Y축 방향이 VCL과 OpenGL이 반대
        OpenGLPanel1->Invalidate();
    }
}
//---------------------------------------------------------------------------
void TMainViewForm::ZoomMap(int delta)
{
    if (g_EarthView) {
        // VCL의 WheelDelta는 보통 120의 배수
        g_EarthView->Zoom(delta > 0 ? 1 : -1);
        OpenGLPanel1->Invalidate();
    }
}
//---------------------------------------------------------------------------
TPointF TMainViewForm::ScreenToGeo(int screenX, int screenY)
{
    if (g_EarthView) {
        double lat, lon;
        g_EarthView->ScreenToWorld(screenX, screenY, lat, lon);
        return TPointF(lon, lat);
	}
    return TPointF(0, 0);
}