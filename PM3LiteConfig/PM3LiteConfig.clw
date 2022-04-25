; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "pm3liteconfig.h"
LastPage=0

ClassCount=7
Class1=CChildFrame
Class2=CMainFrame
Class3=CPM3LiteConfigApp
Class4=CAboutDlg
Class5=CPM3LiteConfigDoc
Class6=CPM3LiteConfigView

ResourceCount=4
Resource1=IDR_PM3LITTYPE
Resource2=IDD_PROJLIST
Resource3=IDD_ABOUTBOX
Class7=CProjectListView
Resource4=IDR_MAINFRAME

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=ID_VIEW_PROJECT_LIST

[CLS:CPM3LiteConfigApp]
Type=0
BaseClass=CWinApp
HeaderFile=PM3LiteConfig.h
ImplementationFile=PM3LiteConfig.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=PM3LiteConfig.cpp
ImplementationFile=PM3LiteConfig.cpp
LastObject=CAboutDlg

[CLS:CPM3LiteConfigDoc]
Type=0
BaseClass=CDocument
HeaderFile=PM3LiteConfigDoc.h
ImplementationFile=PM3LiteConfigDoc.cpp

[CLS:CPM3LiteConfigView]
Type=0
BaseClass=CView
HeaderFile=PM3LiteConfigView.h
ImplementationFile=PM3LiteConfigView.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_CONTEXT_HELP
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_PROJECT_LIST
Command7=ID_VIEW_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_HELP_FINDER
Command10=ID_APP_ABOUT
CommandCount=10

[MNU:IDR_PM3LITTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_HELP_FINDER
Command22=ID_APP_ABOUT
CommandCount=22

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_CONTEXT_HELP
Command16=ID_HELP
CommandCount=16

[DLG:IDD_PROJLIST]
Type=1
Class=CProjectListView
ControlCount=1
Control1=IDC_LIST1,listbox,1352728835

[CLS:CProjectListView]
Type=0
HeaderFile=ProjectListView.h
ImplementationFile=ProjectListView.cpp
BaseClass=CFormView
Filter=D
LastObject=ID_TEST
VirtualFilter=VWC

