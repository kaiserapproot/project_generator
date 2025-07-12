// ObjCWin32.m : Objective-C デリゲート/ビュー/ビューコントローラ構造でのWin32アプリ
//

#include <Foundation/Foundation.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// --- AppDelegate ---
@interface AppDelegate : NSObject
@property (nonatomic, assign) HINSTANCE hInstance;
@property (nonatomic, assign) int nCmdShow;
- (void)applicationWillFinishLaunching;
- (void)applicationDidFinishLaunching;
- (void)applicationDidBecomeActive;
- (void)applicationWillResignActive;
- (void)applicationWillTerminate;
@end

// --- View ---
@interface MyView : NSObject
@property (nonatomic, assign) HWND hWnd;
- (void)drawRect;
- (void)handleMouseDownAt:(POINT)pt;
@end

// --- ViewController ---
@interface MyViewController : NSObject
@property (nonatomic, strong) MyView *view;
- (void)loadViewWithParent:(HWND)parent;
- (void)handleMouseDownAt:(POINT)pt;
@end

@implementation AppDelegate
- (void)applicationWillFinishLaunching {
    NSLog(@"[AppDelegate] アプリケーションの起動準備中");
}
- (void)applicationDidFinishLaunching {
    NSLog(@"[AppDelegate] アプリケーションの起動が完了しました");
}
- (void)applicationDidBecomeActive {
    NSLog(@"[AppDelegate] アクティブになりました");
}
- (void)applicationWillResignActive {
    NSLog(@"[AppDelegate] 非アクティブになりました");
}
- (void)applicationWillTerminate {
    NSLog(@"[AppDelegate] アプリケーションを終了します");
}
@end

@implementation MyView
- (void)drawRect {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(self.hWnd, &ps);
    TextOutW(hdc, 20, 20, L"Hello from MyView", 18);
    EndPaint(self.hWnd, &ps);
}
- (void)handleMouseDownAt:(POINT)pt {
    NSLog(@"[MyView] マウスがクリックされました: x=%ld, y=%ld", pt.x, pt.y);
    // 必要ならここで再描画や他の処理
}
@end

@implementation MyViewController
- (void)loadViewWithParent:(HWND)parent {
    self.view = [[MyView alloc] init];
    self.view.hWnd = parent;
    NSLog(@"[MyViewController] ビューが親ウィンドウにアタッチされました");
}
- (void)handleMouseDownAt:(POINT)pt {
    if (self.view) {
        [self.view handleMouseDownAt:pt];
    }
}
@end

// グローバル変数
HINSTANCE hInst;
WCHAR szWindowClass[] = L"ObjCWin32WindowClass";
AppDelegate *appDelegate;
MyViewController *viewController;

// プロトタイプ宣言
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    appDelegate = [[AppDelegate alloc] init];
    appDelegate.hInstance = hInstance;
    appDelegate.nCmdShow = nCmdShow;
    [appDelegate applicationWillFinishLaunching];
    [appDelegate applicationDidFinishLaunching];

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    [appDelegate applicationDidBecomeActive];

    MSG msg;
    BOOL isActive = TRUE;
    while (GetMessage(&msg, 0, 0, 0))
    {
        if (msg.message == WM_ACTIVATEAPP) {
            if (msg.wParam && !isActive) {
                [appDelegate applicationDidBecomeActive];
                isActive = TRUE;
            } else if (!msg.wParam && isActive) {
                [appDelegate applicationWillResignActive];
                isActive = FALSE;
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    [appDelegate applicationWillTerminate];
    return (int) msg.wParam;
}

// ウィンドウクラス登録
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(0, IDI_APPLICATION); // デフォルトアイコン
    wcex.hCursor        = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0; // メニューなし
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION); // デフォルトアイコン
    return RegisterClassExW(&wcex);
}

// ウィンドウ生成
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;
   LPCWSTR windowTitle = L"Hello Objective-C Window";
   HWND hWnd = CreateWindowW(szWindowClass, windowTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, hInstance, 0);
   if (!hWnd) return FALSE;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   // ViewController生成・Viewロード
   viewController = [[MyViewController alloc] init];
   [viewController loadViewWithParent:hWnd];
   return TRUE;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        if (viewController && viewController.view) {
            [viewController.view drawRect];
        }
        break;
    case WM_LBUTTONDOWN: {
        if (viewController) {
            POINT pt = { LOWORD(lParam), HIWORD(lParam) };
            [viewController handleMouseDownAt:pt];
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
