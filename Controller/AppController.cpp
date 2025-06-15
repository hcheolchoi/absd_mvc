// Controller/AppController.cpp

#pragma hdrstop

#include "Controller/AppController.h"

// .cpp 파일에서는 사용하는 모든 클래스의 전체 정의를 포함해야 합니다.
#include "Model/AircraftModel.h"
#include "View/MainView.h"
#include "Controller/NetworkService.h"
#include "Controller/DataParser.h"
#include "Component/OpenGLPanel.h"
#include "Utils/ntds2d.h"
#include "Utils/TimeFunctions.h"
#include "Map/MapSrc/FlatEarthView.h"
#include "Map/MapSrc/TileManager.h"
#include "Map/MapSrc/FileSystemStorage.h"
#include "Map/MapSrc/MapProvider/GoogleMapsProvider.h"
#include "Map/MapSrc/Layer.h"
#include <gl/gl.h>


AppController::AppController(AircraftModel* model, TMainView* view)
    : model(model), view(view), g_EarthView(nullptr), g_GETileManager(nullptr),
      g_MouseDownMask(0), g_MouseLeftDownX(0), g_MouseLeftDownY(0)
{
    networkService = std::make_unique<NetworkService>();
    networkService->setDataCallback([this](const std::vector<char>& data) {
        this->onDataReceived(data);
    });
    dataParser = std::make_unique<DataParser>();
}

AppController::~AppController()
{
    if (g_GETileManager) delete g_GETileManager;
    if (g_EarthView) delete g_EarthView;
}

void AppController::onDataReceived(const std::vector<char>& data)
{
    auto parsedAircraft = dataParser->parse(data);
    if (parsedAircraft.has_value()) {
        model->addOrUpdateAircraft(parsedAircraft.value());
    }
}

void AppController::onFormCreate()
{
    initializeMap();
    MakeAirplaneImages();
}

void AppController::onFormDestroy()
{
}

void AppController::connect(const String& ip, int port)
{
    if (networkService) {
        networkService->connect(ip, port);
    }
}

void AppController::disconnect()
{
    if (networkService) {
        networkService->disconnect();
    }
}

void AppController::getAllAircraft(std::vector<TADS_B_Aircraft>& list)
{
    if (model) {
        model->getAllAircraft(list);
    } else {
        list.clear();
    }
}

void AppController::onTimerTick()
{
    if (model) {
        model->updateAircraftStatus();
    }
    if (g_EarthView) {
        g_EarthView->Update();
    }
}

void AppController::initializeMap()
{
    mapCenterLat = 40.73612;
    mapCenterLon = -80.33158;
    g_EarthView = new FlatEarthView();
    g_EarthView->SetScreenSize(800, 600);
    g_EarthView->SetViewPoint(mapCenterLat, mapCenterLon, 2.0);
    IMAPProvider *provider = new GoogleMapsProvider(IMAPProvider::Satellite);
    Layer *layer = new Layer(provider);
    g_EarthView->AddLayer(layer);
    g_GETileManager = new TileManager(g_EarthView, new FileSystemStorage(L"C:\\Tiles"));
}

void AppController::drawDisplay(TOpenGLPanel* panel)
{
    if (!g_EarthView) return;

    panel->MakeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    g_EarthView->SetScreenSize(panel->Width, panel->Height);
    g_EarthView->Draw();

    BeginAirplaneBatch();
    if (model) {
        std::vector<TADS_B_Aircraft> aircraftList;
        model->getAllAircraft(aircraftList);

        for (std::vector<TADS_B_Aircraft>::iterator it = aircraftList.begin(); it != aircraftList.end(); ++it) {
            const TADS_B_Aircraft& aircraft = *it;
            if (aircraft.HaveLatLon) {
                double screenX, screenY;
                g_EarthView->GetScreenPos(aircraft.Latitude, aircraft.Longitude, screenX, screenY);
                if (screenX >= 0 && screenY >= 0) {
                     BatchAirplane(screenX, screenY, 1.0, aircraft.Heading, aircraft.SpriteImage, 1,1,0,1);
                }
            }
        }
    }
    RenderAirplaneBatch();

    panel->SwapBuffers();
}

void AppController::onMouseDown(TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button == mbLeft) {
        g_MouseDownMask |= 1;
        g_MouseLeftDownX = X;
        g_MouseLeftDownY = Y;
    }
}

void AppController::onMouseMove(TShiftState Shift, int X, int Y)
{
    if ((g_MouseDownMask & 1) && g_EarthView) {
        double dx = g_MouseLeftDownX - X;
        double dy = g_MouseLeftDownY - Y;
        g_EarthView->Move(dx, dy);
        g_MouseLeftDownX = X;
        g_MouseLeftDownY = Y;
        view->onModelUpdate();
    }
}

void AppController::onMouseUp(TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if (Button == mbLeft) {
        g_MouseDownMask &= ~1;
    }
}