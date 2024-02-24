
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int red, green, blue, yellow;
unsigned long foreground, background;


//*************************************************************************************************************************
//funkcja przydzielania kolorow

int AllocNamedColor(char *name, Display* display, Colormap colormap)
  {
    XColor col;
    XParseColor(display, colormap, name, &col);
    XAllocColor(display, colormap, &col);
    return col.pixel;
  } 

//*************************************************************************************************************************
// inicjalizacja zmiennych globalnych okreslajacych kolory

int init_colors(Display* display, int screen_no, Colormap colormap)
{
  background = WhitePixel(display, screen_no);  //niech tlo bedzie biale
  foreground = BlackPixel(display, screen_no);  //niech ekran bedzie czarny
  red=AllocNamedColor("red", display, colormap);
  green=AllocNamedColor("green", display, colormap);
  blue=AllocNamedColor("blue", display, colormap);
  yellow=AllocNamedColor("yellow", display, colormap);
}

int turned = 0;
int max_x = 0, max_y = 0, min_x = 200, min_y = 200;
int x_axis;

int draw_g_letter(Display* display, Window window, GC gc) {
  int f_p_x = 30, f_p_y = 10, s_p_x = 55, s_p_y = 10;
  XPoint points[] = {
    {f_p_x, f_p_y},
    {s_p_x, s_p_y},
    {75, 30},
    {60, 30},
    {50, 20},
    {40, 20},
    {30, 30},
    {30, 80},
    {40, 90},
    {50, 90},
    {60, 80},    
    {50, 80},
    {50, 70},
    {80, 70},
    {80, 80},
    {70, 80},
    {60, 100},
    {30, 100},
    {20, 80},
    {20, 60},
    {20, 20},
    {30, 10},
  };
  int point_length = sizeof(points)/sizeof(XPoint);
  for (int i = 0; i < point_length; i++) {
    if (points[i].x < min_x) min_x = points[i].x;
    if (points[i].x > max_x) max_x = points[i].x;
    if (points[i].y < min_y) min_y = points[i].y;
    if (points[i].y > max_y) max_y = points[i].y;
  }
  x_axis = max_x - min_x / 2; 
  if (turned) {
    for (int i = 0; i < point_length; i++) {
      int distance_from_axis = x_axis - points[i].x;
      int new_coordinates = distance_from_axis + 15;
      points[i].x = new_coordinates;
    }
  }
  XFillArc(display, window, gc, f_p_x, f_p_y-5, s_p_x - f_p_x, 10, 0,  180 * 64);
  XFillPolygon(display, window, gc, points, point_length, Nonconvex, CoordModeOrigin);
}

//*************************************************************************************************************************
// Glowna funkcja zawierajaca petle obslugujaca zdarzenia */

int main(int argc, char *argv[])
{
  char            icon_name[] = "Grafika";
  char            title[]     = "Grafika komputerowa";
  Display*        display;    //gdzie bedziemy wysylac dane (do jakiego X servera)
  Window          window;     //nasze okno, gdzie bedziemy dokonywac roznych operacji
  GC              gc;         //tu znajduja sie informacje o parametrach graficznych
  XEvent          event;      //gdzie bedziemy zapisywac pojawiajace sie zdarzenia
  KeySym          key;        //informacja o stanie klawiatury 
  Colormap        colormap;
  int             screen_no;
  XSizeHints      info;       //informacje typu rozmiar i polozenie ok
  
  char            buffer[8];  //gdzie bedziemy zapamietywac znaki z klawiatury
  int             hm_keys;    //licznik klawiszy
  int             to_end;

  display    = XOpenDisplay("");                //otworz polaczenie z X serverem pobierz dane od zmiennej srodowiskowej DISPLAY ("")
  screen_no  = DefaultScreen(display);          //pobierz domyslny ekran dla tego wyswietlacza (0)
  colormap = XDefaultColormap(display, screen_no);
  init_colors(display, screen_no, colormap);

  //okresl rozmiar i polozenie okna
  info.x = 100;
  info.y = 150;
  info.width = 500;
  info.height = 300;
  info.flags = PPosition | PSize;

  //majac wyswietlacz, stworz okno - domyslny uchwyt okna
  window = XCreateSimpleWindow(display, DefaultRootWindow(display),info.x, info.y, info.width, info.height, 7/* grubosc ramki */, foreground, background);
  XSetStandardProperties(display, window, title, icon_name, None, argv, argc, &info);
  //utworz kontekst graficzny do zarzadzania parametrami graficznymi (0,0) domyslne wartosci
  gc = XCreateGC(display, window, 0, 0);
  XSetBackground(display, gc, background);
  XSetForeground(display, gc, foreground);

  //okresl zdarzenia jakie nas interesuja, np. nacisniecie klawisza
  XSelectInput(display, window, (KeyPressMask | ExposureMask | ButtonPressMask| ButtonReleaseMask | Button1MotionMask));
  XMapRaised(display, window);  //wyswietl nasze okno na samym wierzchu wszystkich okien
      
  to_end = FALSE;

 /* petla najpierw sprawdza, czy warunek jest spelniony
     i jesli tak, to nastepuje przetwarzanie petli
     a jesli nie, to wyjscie z petli, bez jej przetwarzania */
  while (to_end == FALSE)
  {
    XNextEvent(display, &event);  // czekaj na zdarzenia okreslone wczesniej przez funkcje XSelectInput

    switch(event.type)
    {
      case Expose:
        if (event.xexpose.count == 0)
        {
          XSetForeground(display, gc, foreground);
          XClearWindow(display, window);
          draw_g_letter(display, window, gc);
        }
        break;

      case MappingNotify:
        XRefreshKeyboardMapping(&event.xmapping); // zmiana ukladu klawiatury - w celu zabezpieczenia sie przed taka zmiana trzeba to wykonac
        break;

      case ButtonPress:
        if (event.xbutton.button == Button1)  // sprawdzenie czy wcisnieto lewy przycisk		
        {
          if (event.xbutton.y > min_y && event.xbutton.y < max_y)
          {
            if (event.xbutton.x > min_x && event.xbutton.x < x_axis - 15) {
              turned = 1;
            }
            if (event.xbutton.x > x_axis - 15 && event.xbutton.x < max_x) {
              turned = 0;
            }
            XSetForeground(display, gc, foreground);
            XClearWindow(display, window);
            draw_g_letter(display, window, gc);
          }
        }
        break;


      case KeyPress:
        hm_keys = XLookupString(&event.xkey, buffer, 8, &key, 0);
        if (hm_keys == 1)
        {
          if (buffer[0] == 'q') to_end = TRUE;        // koniec programu
          
        }

      default:
        break;
    }
  }

  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
