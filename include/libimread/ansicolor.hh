
#ifndef LIBIMREAD_ANSICOLORS_HH_
#define LIBIMREAD_ANSICOLORS_HH_

#include <string>
#include <iostream>
#include <map>

namespace ansi {

    // template <typename M, typename N>
    // using map = std::unordered_map<M, N>;
    using std::map;
    
    enum ANSICode {
        FM_RESET            = 0,
        FM_BOLD             = 1,
        FM_DIM              = 2,
        FM_UNDERLINE        = 4,
        FM_BOLD_OFF         = 1,
        FM_DIM_OFF          = 2,
        FM_UNDERLINE_OFF    = 4,
        
        FG_BLACK            = 30,
        FG_RED              = 31,
        FG_GREEN            = 32,
        FG_YELLOW           = 33,
        FG_BLUE             = 34,
        FG_MAGENTA          = 35,
        FG_CYAN             = 36,
        FG_LIGHTGRAY        = 37,
        FG_DEFAULT_COLOR    = 39,
        BG_RED              = 41,
        BG_GREEN            = 42,
        BG_BLUE             = 44,
        BG_DEFAULT          = 49,
        FG_DARKGRAY         = 90,
        FG_LIGHTRED         = 91,
        FG_LIGHTGREEN       = 92,
        FG_LIGHTYELLOW      = 93,
        FG_LIGHTBLUE        = 94,
        FG_LIGHTMAGENTA     = 95,
        FG_LIGHTCYAN        = 96,
        FG_WHITE            = 97
    };
    
    class ANSI {
        private:
            ANSICode code;
        public:
            ANSI(ANSICode c)
                :code(c)
                {}
            friend std::ostream&
            operator<<(std::ostream& os, const ANSI& ansi) {
                return os << "\033[" << ansi.code << "m";
            }
    };
    
    struct color {
        static map<std::string, ANSI> init() {
            map<std::string, ANSI> _color_map = {
                { "reset",          ANSI(FM_RESET) },
                { "default",        ANSI(FG_DEFAULT_COLOR) },
                { "black",          ANSI(FG_BLACK) },
                { "red",            ANSI(FG_RED) },
                { "green",          ANSI(FG_GREEN) },
                { "yellow",         ANSI(FG_YELLOW) },
                { "blue",           ANSI(FG_BLUE) },
                { "magenta",        ANSI(FG_MAGENTA) },
                { "cyan",           ANSI(FG_CYAN) },
                { "lightred",       ANSI(FG_LIGHTRED) },
                { "lightgreen",     ANSI(FG_LIGHTGREEN) },
                { "lightyellow",    ANSI(FG_LIGHTYELLOW) },
                { "lightblue",      ANSI(FG_LIGHTBLUE) },
                { "lightmagenta",   ANSI(FG_LIGHTMAGENTA) },
                { "lightcyan",      ANSI(FG_LIGHTCYAN) },
                { "lightgray",      ANSI(FG_LIGHTGRAY) },
                { "darkgray",       ANSI(FG_DARKGRAY) },
                { "white",          ANSI(FG_WHITE) },
            };
            return _color_map;
        }
        static const map<std::string, ANSI> idx;
    };
    
    const ANSI reset = ANSI(FM_RESET);
    const ANSI termdefault = ANSI(FG_DEFAULT_COLOR);
    
    const ANSI red = ANSI(FG_RED);
    const ANSI green = ANSI(FG_GREEN);
    const ANSI yellow = ANSI(FG_YELLOW);
    const ANSI blue = ANSI(FG_BLUE);
    const ANSI magenta = ANSI(FG_MAGENTA);
    const ANSI cyan = ANSI(FG_CYAN);
    
    const ANSI lightred = ANSI(FG_LIGHTRED);
    const ANSI lightgreen = ANSI(FG_LIGHTGREEN);
    const ANSI lightyellow = ANSI(FG_LIGHTYELLOW);
    const ANSI lightblue = ANSI(FG_LIGHTBLUE);
    const ANSI lightmagenta = ANSI(FG_LIGHTMAGENTA);
    const ANSI lightcyan = ANSI(FG_LIGHTCYAN);
    
    const ANSI lightgray = ANSI(FG_LIGHTGRAY);
    const ANSI darkgray = ANSI(FG_DARKGRAY);
    const ANSI white = ANSI(FG_WHITE);
    

}

#endif /// LIBIMREAD_ANSICOLORS_HH_