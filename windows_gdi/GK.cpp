#include <windows.h>
#include <gdiplus.h>
#include <cmath>
#include <cmath>

#define M_PI           3.14159265358979323846

using namespace Gdiplus;


POINT srodek_zegara = { 300, 300 };
COLORREF brazowy = RGB(165, 42, 42);
COLORREF czerwony = RGB(255, 0, 0);
COLORREF bialy = RGB(255, 255, 255);
COLORREF czarny = RGB(0, 0, 0);
int promien_zegara = 200;
int promien_tarczy = 100;
int dlugosc_podstawy = 200;
POINT click = { 400, 400 };
double angle = atan2(click.x - srodek_zegara.x, click.y - srodek_zegara.y) - M_PI / 2;

//deklaracja funkcji obslugi okna
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//funkcja Main - dla Windows
 int WINAPI WinMain(HINSTANCE hInstance,
               HINSTANCE hPrevInstance,
               LPSTR     lpCmdLine,
               int       nCmdShow)
{
	MSG meldunek;		  //innymi slowy "komunikat"
	WNDCLASS nasza_klasa; //klasa g��wnego okna aplikacji
	HWND okno;
	static char nazwa_klasy[] = "Podstawowa";
	
	GdiplusStartupInput gdiplusParametry;// parametry GDI+; domy�lny konstruktor wype�nia struktur� odpowiednimi warto�ciami
	ULONG_PTR	gdiplusToken;			// tzw. token GDI+; warto�� uzyskiwana przy inicjowaniu i przekazywana do funkcji GdiplusShutdown
   
	// Inicjujemy GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusParametry, NULL);

	//Definiujemy klase g��wnego okna aplikacji
	//Okreslamy tu wlasciwosci okna, szczegoly wygladu oraz
	//adres funkcji przetwarzajacej komunikaty
	nasza_klasa.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	nasza_klasa.lpfnWndProc   = WndProc; //adres funkcji realizuj�cej przetwarzanie meldunk�w 
 	nasza_klasa.cbClsExtra    = 0 ;
	nasza_klasa.cbWndExtra    = 0 ;
	nasza_klasa.hInstance     = hInstance; //identyfikator procesu przekazany przez MS Windows podczas uruchamiania programu
	nasza_klasa.hIcon         = 0;
	nasza_klasa.hCursor       = LoadCursor(0, IDC_ARROW);
	nasza_klasa.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	nasza_klasa.lpszMenuName  = "Menu" ;
	nasza_klasa.lpszClassName = nazwa_klasy;

    //teraz rejestrujemy klas� okna g��wnego
    RegisterClass (&nasza_klasa);
	
	/*tworzymy okno g��wne
	okno b�dzie mia�o zmienne rozmiary, listw� z tytu�em, menu systemowym
	i przyciskami do zwijania do ikony i rozwijania na ca�y ekran, po utworzeniu
	b�dzie widoczne na ekranie */
 	okno = CreateWindow(nazwa_klasy, "Grafika komputerowa", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	
	
	/* wyb�r rozmiaru i usytuowania okna pozostawiamy systemowi MS Windows */
   	ShowWindow (okno, nCmdShow) ;
    
	//odswiezamy zawartosc okna
	UpdateWindow (okno) ;

	// G��WNA P�TLA PROGRAMU
	while (GetMessage(&meldunek, NULL, 0, 0))
     /* pobranie komunikatu z kolejki; funkcja GetMessage zwraca FALSE tylko dla
	 komunikatu WM_QUIT; dla wszystkich pozosta�ych komunikat�w zwraca warto�� TRUE */
	{
		TranslateMessage(&meldunek); // wst�pna obr�bka komunikatu
		DispatchMessage(&meldunek);  // przekazanie komunikatu w�a�ciwemu adresatowi (czyli funkcji obslugujacej odpowiednie okno)
	}

	GdiplusShutdown(gdiplusToken);
	
	return (int)meldunek.wParam;
}


/********************************************************************
FUNKCJA OKNA realizujaca przetwarzanie meldunk�w kierowanych do okna aplikacji*/
LRESULT CALLBACK WndProc (HWND okno, UINT kod_meldunku, WPARAM wParam, LPARAM lParam)
{
	HMENU mPlik, mInfo, mGlowne;
    	
/* PONI�SZA INSTRUKCJA DEFINIUJE REAKCJE APLIKACJI NA POSZCZEG�LNE MELDUNKI */
	switch (kod_meldunku) 
	{
	case WM_CREATE:  //meldunek wysy�any w momencie tworzenia okna
		mPlik = CreateMenu();
		AppendMenu(mPlik, MF_STRING, 100, "&Zapiszcz...");
		AppendMenu(mPlik, MF_SEPARATOR, 0, "");
		AppendMenu(mPlik, MF_STRING, 101, "&Koniec");
		mInfo = CreateMenu();
		AppendMenu(mInfo, MF_STRING, 200, "&Autor...");
		mGlowne = CreateMenu();
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mPlik, "&Plik");
		AppendMenu(mGlowne, MF_POPUP, (UINT_PTR) mInfo, "&Informacja");
		SetMenu(okno, mGlowne);
		DrawMenuBar(okno);

	case WM_COMMAND: //reakcje na wyb�r opcji z menu
		switch (wParam)
		{
		case 100: if(MessageBox(okno, "Zapiszcze�?", "Pisk", MB_YESNO) == IDYES)
					MessageBeep(0);
                  break;
		case 101: DestroyWindow(okno); //wysylamy meldunek WM_DESTROY
        		  break;
		case 200: MessageBox(okno, "Imi� i nazwisko:\nNumer indeksu: ", "Autor", MB_OK);
		}
		return 0;
	
	case WM_LBUTTONDOWN: //reakcja na lewy przycisk myszki
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			click.x = x;
			click.y = y;
			angle = atan2(click.x - srodek_zegara.x, click.y - srodek_zegara.y) - M_PI / 2;
			InvalidateRect(okno, 0, TRUE);

			return 0;
		}

	case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC kontekst;

			kontekst = BeginPaint(okno, &paint);
		
			// MIEJSCE NA KOD GDI

			/*HPEN pioro = CreatePen(PS_SOLID, 10, RGB(165, 42, 42));
			HBRUSH pedzel = CreateSolidBrush(RGB(165, 42, 42));
			SelectObject(kontekst, pioro);

			Ellipse(kontekst, 100, 100, 400, 400);
			Rectangle(kontekst, 100, 250, 400, 400);
			
			SelectObject(kontekst, pedzel);
			FloodFill(kontekst, 200, 200, RGB(165, 42, 42));
			FloodFill(kontekst, 150, 300, RGB(165, 42, 42));
			*/

			HPEN pioro = CreatePen(PS_SOLID, 5, brazowy);
			SelectObject(kontekst, pioro);

			Ellipse(kontekst, srodek_zegara.x - promien_zegara, srodek_zegara.y - promien_zegara, srodek_zegara.x + promien_zegara, srodek_zegara.y + promien_zegara);
			Rectangle(kontekst, srodek_zegara.x - promien_zegara, srodek_zegara.y, srodek_zegara.x + promien_zegara, srodek_zegara.y + dlugosc_podstawy);

			
			HBRUSH pedzel = CreateSolidBrush(brazowy);
			SelectObject(kontekst, pedzel);
			FloodFill(kontekst, srodek_zegara.x, srodek_zegara.y - promien_tarczy, brazowy);
			FloodFill(kontekst, srodek_zegara.x, srodek_zegara.y + promien_tarczy, brazowy);

			HPEN czerwone_pioro = CreatePen(PS_SOLID, 5, czerwony);
			SelectObject(kontekst, czerwone_pioro);
			Ellipse(kontekst, srodek_zegara.x - promien_tarczy, srodek_zegara.y - promien_tarczy, srodek_zegara.x + promien_tarczy, srodek_zegara.y + promien_tarczy);

			HBRUSH bialy_pedzel = CreateSolidBrush(bialy);
			SelectObject(kontekst, bialy_pedzel);
			FloodFill(kontekst, srodek_zegara.x, srodek_zegara.y - 10, czerwony);

			HPEN czarny_dlugopis = CreatePen(PS_SOLID, 5, czarny);
			SelectObject(kontekst, czarny_dlugopis);
			MoveToEx(kontekst, srodek_zegara.x, srodek_zegara.y, NULL);

			/*POINT srodek_zegara = {300, 300};
			COLORREF brazowy = RGB(165, 42, 42);
			COLORREF czerwony = RGB(255, 0, 0);
			COLORREF bialy = RGB(255, 255, 255);
			COLORREF czarny = RGB(0, 0, 0);
			int promien_zegara = 200;
			int promien_tarczy = 100;
			int dlugosc_podstawy = 200;
			POINT click = { 400, 400 };
			double angle = atan2(click.x - srodek_zegara.x, click.y - srodek_zegara.y) - M_PI / 2;
			*/
			POINT czubek_strzalki = { srodek_zegara.x + (promien_tarczy * cos(angle)), srodek_zegara.y - (promien_tarczy * sin(angle)) };

			LineTo(kontekst, czubek_strzalki.x, czubek_strzalki.y);
			LineTo(kontekst, czubek_strzalki.x - (10 * sin(angle + M_PI / 4)), czubek_strzalki.y - (10 * cos(angle + M_PI / 4)));
			LineTo(kontekst, czubek_strzalki.x + (10 * sin(angle - M_PI / 4)), czubek_strzalki.y + (10 * cos(angle - M_PI / 4)));
			LineTo(kontekst, czubek_strzalki.x, czubek_strzalki.y);

			DeleteObject(pioro);

			// utworzenie obiektu umo�liwiaj�cego rysowanie przy u�yciu GDI+
			// (od tego momentu nie mo�na u�ywa� funkcji GDI
			//Graphics grafika(kontekst);
			
			// MIEJSCE NA KOD GDI+


			// utworzenie czcionki i wypisanie tekstu na ekranie
			//FontFamily  fontFamily(L"Times New Roman");
			//Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			//PointF      pointF(100.0f, 400.0f);
			//SolidBrush  solidBrush(Color(255, 0, 0, 255));

			//grafika.DrawString(L"To jest tekst napisany za pomoc� GDI+.", -1, &font, pointF, &solidBrush);

			Sleep(200);
			angle -= M_PI / 8;
			InvalidateRect(okno, 0, TRUE);

			EndPaint(okno, &paint);

			return 0;
		}
  	
	case WM_DESTROY: //obowi�zkowa obs�uga meldunku o zamkni�ciu okna
		PostQuitMessage (0) ;
		return 0;

	default: //standardowa obs�uga pozosta�ych meldunk�w
		return DefWindowProc(okno, kod_meldunku, wParam, lParam);
	}
}
