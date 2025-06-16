//---------------------------------------------------------------------------
#ifndef MainViewH
#define MainViewH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Components/OpenGLv0.5BDS2006/Component/OpenGLPanel.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "Model/IModelObserver.h"
#include <System.Types.hpp>
#include "cspin.h"
#include "OpenGLPanel.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

// Forward declarations
class AppController;
class AircraftModel;
class EarthView;

//---------------------------------------------------------------------------
class TMainViewForm : public TForm, public IModelObserver
{
__published:	// IDE-managed Components
	TPanel *Panel_Top;
	TStatusBar *StatusBar_Bottom;
	TOpenGLPanel *OpenGLPanel1;
	TTimer *RenderTimer;
	TMainMenu *MainMenu1;
	TMenuItem *FileMenu;
	TMenuItem *ExitMenuItem;
	TMenuItem *MapMenu;
	TMenuItem *SourceMenu;
	TMenuItem *GoogleMapsMenu;
	TMenuItem *SkyVectorMenu;
	TMenuItem *VFRSectionalMenu;
	TMenuItem *IFRLowMenu;
	TMenuItem *IFRHighMenu;
	TMenuItem *DataMenu;
	TMenuItem *ConnectMenuItem;
	TMenuItem *DisconnectMenuItem;
	TMenuItem *N1;
	TMenuItem *RecordRawDataMenu;
	TMenuItem *PlaybackRawDataMenu;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *AreaMenu;
	TMenuItem *InsertAreaMenu;
	TMenuItem *CompleteAreaMenu;
	TMenuItem *CancelAreaMenu;
	TListView *AreaListView;
	TPanel *Panel_Right;
	TLabel *Label1;
	TMenuItem *DeleteAreaMenu;
	TMenuItem *HelpMenu;
	TMenuItem *AboutMenu;

	// Event Handlers
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OpenGLPanel1Paint(TObject *Sender);
	void __fastcall OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall OpenGLPanel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall OpenGLPanel1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall RenderTimerTimer(TObject *Sender);
	void __fastcall ExitMenuItemClick(TObject *Sender);
	void __fastcall ConnectMenuItemClick(TObject *Sender);
	void __fastcall DisconnectMenuItemClick(TObject *Sender);
	void __fastcall AboutMenuClick(TObject *Sender);


private:	// User declarations
    AppController* controller;
    AircraftModel* model;

    EarthView *g_EarthView;

    int lastMouseX, lastMouseY;
    bool isPanning;

    void InitializeOpenGL();
    void DrawMap();
    void DrawAircraft();

public:		// User declarations
	__fastcall TMainViewForm(TComponent* Owner);

    void SetController(AppController* ctrl);
    void SetModel(AircraftModel* mdl);

    // IModelObserver interface
    virtual void Update();

    // Map manipulation methods
    void PanMap(int deltaX, int deltaY);
    void ZoomMap(int delta);
    TPointF ScreenToGeo(int screenX, int screenY);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainViewForm *MainViewForm;
//---------------------------------------------------------------------------
#endif