// View/MainView.h

#ifndef MainViewH
#define MainViewH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include "OpenGLPanel.h"
#include "IModelObserver.h"
#include "cspin.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

// 전방 선언
class AppController;

//---------------------------------------------------------------------------
class TForm1 : public TForm, public IModelObserver
{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TMemo *Memo1;
	TOpenGLPanel *OpenGLPanel1;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Exit1;
	TMenuItem *Record1;
	TMenuItem *PlayBack1;
	TMenuItem *Stop1;
	TMenuItem *Help1;
	TMenuItem *About1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TPanel *Panel1;
	TPanel *Panel2;
	TListView *ListView1;
	TStatusBar *StatusBar1;
	TTabSheet *TabSheet2;
	TLabel *Label1;
	TEdit *IPAddress;
	TLabel *Label2;
	TEdit *Port;
	TButton *ConnectButton;
	TButton *DisconnectButton;
	TRadioGroup *DataSource;
	TCheckBox *RecordRawCheckBox;
	TCheckBox *RecordSBSCheckBox;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *LoadAircraftDatabase1;
	TMenuItem *LoadARTCCBoundaries1;
	TListView *AreaListView;
	TButton *Insert;
	TButton *Delete;
	TButton *Complete;
	TButton *Cancel;
	TImage *Image1;
	TLabel *Label3;
	TEdit *OwnshipICAO;
	TCheckBox *CheckBoxCPA;
	TLabel *Label4;
	TCheckBox *CheckBoxCollisionCourse;
	TLabel *Label5;
	TLabel *Label6;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall OpenGLPanel1Paint(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall OpenGLPanel1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);


private:	// User declarations
	AppController* theController;

	void DrawAircrafts();
	void DrawLabels();

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1() override; // 'override' 추가

	void __fastcall Update() override; // '__fastcall' 추가
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif