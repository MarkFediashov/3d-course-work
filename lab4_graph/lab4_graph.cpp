// lab4_graph.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab4_graph.h"
#include "RenderObject.h"

#define MAX_LOADSTRING 100

const int AmountObjects = 5;

float angle1 = 0;      //стартовое положение камеры
float angle2 = 20;
float r = 40;
float zoom = 0.002;

ViewPoint* camera = new ViewPoint(angle1, angle2, r, zoom);

float omega = camera->O_hor;
float fi_vert = camera->fi_vert;
float R = camera->R;

float view_matrix[4][4] = {
		{-sin(omega), -cos(fi_vert) * cos(omega), -sin(fi_vert) * cos(omega), 0},
		{cos(omega), -cos(fi_vert) * sin(omega), -sin(fi_vert) * sin(omega),  0},
		{0,				sin(fi_vert),				 -cos(fi_vert),		0},
		{0,				0,						R,					1}
};

const int FREQUENT = 150;

RenderableObject shape1 = load_model("left_panel_near.obj");     // и только потоооом объект
RenderableObject shape2 = load_model("left_panel_near.obj");     // и только потоооом объект
RenderableObject shape3 = load_model("right_panel_near.obj");     // и только потоооом объект
RenderableObject shape4 = load_model("right_panel_near.obj");     // и только потоооом объект

RenderableObject shape5 = load_model("spacecraft_module.obj");
RenderableObject objects[AmountObjects] = { shape1, shape2, shape3, shape4, shape5 };

float max1 = objects[0].findMax(AxisDirection::Y);

float max2 = objects[1].findMax(AxisDirection::Y); //вращаем вокруг максимального игрика

float min3 = objects[2].findMin(AxisDirection::Y);

float min4 = objects[3].findMax(AxisDirection::Y); //вращаем вокруг максимального игрика


RenderedPolygon* projections[AmountObjects];

static Point3D minO2(0, 0, 0);
static Point3D maxO4(0, 0, 0);

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	

	float minY = objects[0].findMin(AxisDirection::Y);
	float minZ = objects[0].findMin(AxisDirection::Z);

	minO2 = Point3D(0, minY, minZ);


	float maxY = objects[2].findMax(AxisDirection::Y);
	float maxZ = objects[2].findMin(AxisDirection::Z);

	maxO4 = Point3D(0, maxY, maxZ);

    // TODO: Разместите код здесь.
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB4GRAPH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB4GRAPH));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB4GRAPH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB4GRAPH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, NULL, 750, NULL);//установка таймера

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
	{
		HDC buffer = CreateCompatibleDC(GetDC(hWnd));
		HBITMAP fon = (HBITMAP)LoadImage(NULL, L"empty.bmp", IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE);

		SelectObject(buffer, fon);

		

		const float angle = 1 * PI / 180;
		static float current_angle = PI / 180;

		if (current_angle < PI / 2)
		{

			Transform::moveOn(Point3D(0, -max1, 0), objects);
			Transform::rotate(-angle, AxisDirection::X, objects);
			Transform::moveOn(Point3D(0, max1, 0), objects);

			Transform::moveOn(Point3D(0, -max2, 0), objects + 1);
			Transform::rotate(angle, AxisDirection::X, objects + 1);
			Transform::moveOn(Point3D(0, max2, 0), objects + 1);

			float mx = objects[0].findMin(AxisDirection::X);
			float my = objects[0].findMin(AxisDirection::Y);
			float mz = objects[0].findMin(AxisDirection::Z);
			Point3D currentMinLeft(mx, my, mz);

			Transform::moveOn(Point3D(0,
				2 * (currentMinLeft.y - minO2.y), 2 * (currentMinLeft.z - minO2.z)), objects + 1);

			minO2 = currentMinLeft;

			//---------------------------------------------------------------

			Transform::moveOn(Point3D(0, -min3, 0), objects + 2);
			Transform::rotate(angle, AxisDirection::X, objects + 2);
			Transform::moveOn(Point3D(0, min3, 0), objects + 2);

			Transform::moveOn(Point3D(0, -min3, 0), objects + 3);
			Transform::rotate(-angle, AxisDirection::X, objects + 3);
			Transform::moveOn(Point3D(0, min3, 0), objects + 3);

			float _mx = objects[2].findMax(AxisDirection::X);
			float _my = objects[2].findMax(AxisDirection::Y);
			float _mz = objects[2].findMin(AxisDirection::Z);

			Point3D currentMaxRight(_mx, _my, _mz);
			Transform::moveOn(Point3D(0,
				2 * (currentMaxRight.y - maxO4.y), 2 * (currentMaxRight.z - maxO4.z)), objects + 3);

			maxO4 = currentMaxRight;

			current_angle += angle;
		}
		else
		{
			for (int i = 0; i < AmountObjects - 1; i++)
			{
				Transform::rotate(-angle*3, AxisDirection::Y, objects + i);
			}
		}
		
		//*******************************************************************************************************************

		update_view_matrix(view_matrix, angle1, angle2, r);
		camera = new ViewPoint(angle1, angle2, r, zoom); //создаём объект камеры
		BitBlt(GetDC(hWnd), 0, 0, 1600, 1200, NULL, 0, 0, WHITENESS);
		for (int i = 0; i < AmountObjects; i++)
		{
			projections[i] = objects[i].render(camera, view_matrix);
			for (int j = 0; j < objects[i].getSize(); j++)
			{
				projections[i][j].goToScreenCoord();
				projections[i][j].drawPolygon(&buffer);
			}
			delete[] projections[i];
		}
		BitBlt(GetDC(hWnd), 0, 0, 1600, 1200, buffer, 0, 0, SRCCOPY);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		MoveWindow(hWnd, 0, 0, ScreenWidth, ScreenHeigth, NULL);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		BitBlt(GetDC(hWnd), 0, 0, 1600, 1200, NULL, 0, 0, WHITENESS);

		
		camera = new ViewPoint(angle1, angle2, r, zoom); //создаём объект камеры
		for (int i = 0; i < AmountObjects; i++)
		{
			projections[i] = objects[i].render(camera, view_matrix);
			for (int j = 0; j < objects[i].getSize(); j++)
			{
				projections[i][j].goToScreenCoord();
				projections[i][j].drawPolygon(&hdc);
			}
			delete[] projections[i];
		}
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		//EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
	{
		HDC buffer = CreateCompatibleDC(GetDC(hWnd));
		HBITMAP fon = (HBITMAP)LoadImage(NULL, L"empty.bmp", IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE);

		SelectObject(buffer, fon);

		
		const float angle = 1 * PI / 180;


		/*max2 = objects[1].findMax(AxisDirection::Y); //вращаем вокруг максимального игрика
		min2 = objects[1].findMin(AxisDirection::Y);
		median2 = (max2 + min2) / 2;*/
		
		switch (wParam) {
			
		case VK_SPACE: // ну тут как бы да, всё обновляем в соответствии с нажатыми клавишами
		{
			r += 30;
			break;
		}
		case VK_DOWN:
		{
			angle2 -= 0.2;
			break;
		}
		case VK_UP:
		{
			angle2 += 0.2;
			break;
		}
		case VK_RIGHT:
		{
			angle1 -= 0.2;
			break;
		}
		case VK_LEFT:
		{
			angle1 += 0.2;
			break;
		}
		case 0x4D:                   //M
		{
			r -= 30;
			break;
		}
		default:
			break;
		}

		//r = abs(r);
		update_view_matrix(view_matrix, angle1, angle2, r);
		camera = new ViewPoint(angle1, angle2, r, zoom); //создаём объект камеры
		BitBlt(GetDC(hWnd), 0, 0, 1600, 1200, NULL, 0, 0, WHITENESS);
		for (int i = 0; i < AmountObjects; i++)
		{
			projections[i] = objects[i].render(camera, view_matrix);
			for (int j = 0; j < objects[i].getSize(); j++)
			{
				projections[i][j].goToScreenCoord();
				projections[i][j].drawPolygon(&buffer);
			}
			BitBlt(GetDC(hWnd), 0, 0, 1600, 1200, buffer, 0, 0, SRCCOPY);
			delete[] projections[i];
		}
		//BitBlt(GetDC(hWnd), 0, 0, 1600, 1200, buffer, 0, 0, SRCCOPY);
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

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
