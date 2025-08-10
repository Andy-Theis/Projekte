/*
 tetris_win.c
 Windows-native Tetris (single-file)
 Uses: <conio.h> for input, WinAPI (windows.h) for console cursor + colors
 Compile with: gcc -O2 -o tetris_win.exe tetris_win.c   (MinGW)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>    /* _kbhit, _getch */
#include <windows.h>  /* WinAPI: console control */
#include <stdint.h>

#define BOARD_W 10
#define BOARD_H 20

/* console handle global */
static HANDLE hConsole;

/* Tetrominos: 7 types, 4 rotations, 4x4 grid */
static const int pieces[7][4][4][4] = {
    /* I */
    {
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
        {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}
    },
    /* J */
    {
        {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}}
    },
    /* L */
    {
        {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}},
        {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    /* O */
    {
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}
    },
    /* S */
    {
        {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
        {{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}},
        {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    /* T */
    {
        {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}}
    },
    /* Z */
    {
        {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
        {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}},
        {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}}
    }
};

typedef struct {
    int x, y;
    int type;   /* 0..6 */
    int rot;    /* 0..3 */
} Piece;

/* Spielzustand */
static int board[BOARD_H][BOARD_W]; /* 0 = leer, >0 = type+1 */
static Piece cur, nextp;
static int score = 0, level = 1, lines_cleared = 0;
static int game_over = 0;

/* console drawing helpers */
void set_cursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}
void set_color(int color) {
    /* color: 0..15 (foreground) */
    SetConsoleTextAttribute(hConsole, (WORD)color);
}

/* spawn next piece into current */
void spawn_piece() {
    cur = nextp;
    cur.x = (BOARD_W/2) - 2;
    cur.y = -1; /* allow initial y negative so top pieces can be partially above */
    nextp.type = rand() % 7;
    nextp.rot = rand() % 4;
    nextp.x = 0; nextp.y = 0;
    /* immediate collision => game over */
    /* check collision */
    for (int py=0; py<4; ++py)
        for (int px=0; px<4; ++px) {
            if (!pieces[cur.type][cur.rot][py][px]) continue;
            int bx = cur.x + px;
            int by = cur.y + py;
            if (by >= 0 && by < BOARD_H && bx >=0 && bx < BOARD_W)
                if (board[by][bx]) { game_over = 1; }
            if (by >= BOARD_H) game_over = 1;
            if (bx < 0 || bx >= BOARD_W) { /* don't immediately kill for side overflow; will be handled by collision check */
            }
        }
}

/* check collision for given piece state; returns 1 on collision */
int check_collision(const Piece *p) {
    for (int py=0; py<4; ++py) {
        for (int px=0; px<4; ++px) {
            if (!pieces[p->type][p->rot][py][px]) continue;
            int bx = p->x + px;
            int by = p->y + py;
            if (bx < 0 || bx >= BOARD_W) return 1;
            if (by >= BOARD_H) return 1;
            if (by >= 0 && board[by][bx]) return 1;
        }
    }
    return 0;
}

/* lock piece into board */
void lock_piece() {
    for (int py=0; py<4; ++py)
        for (int px=0; px<4; ++px) {
            if (!pieces[cur.type][cur.rot][py][px]) continue;
            int bx = cur.x + px;
            int by = cur.y + py;
            if (by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W)
                board[by][bx] = cur.type + 1;
        }
    /* clear lines */
    int lines = 0;
    for (int y = BOARD_H-1; y >= 0; --y) {
        int full = 1;
        for (int x = 0; x < BOARD_W; ++x) if (!board[y][x]) { full = 0; break; }
        if (full) {
            ++lines;
            for (int yy = y; yy > 0; --yy) memcpy(board[yy], board[yy-1], sizeof(board[yy]));
            memset(board[0], 0, sizeof(board[0]));
            ++y; /* recheck same row after shift */
        }
    }
    if (lines) {
        lines_cleared += lines;
        int points = 0;
        if (lines == 1) points = 40 * level;
        else if (lines == 2) points = 100 * level;
        else if (lines == 3) points = 300 * level;
        else if (lines >= 4) points = 1200 * level;
        score += points;
        level = 1 + (lines_cleared / 10);
    }
    spawn_piece();
}

/* rotate with simple wallkick attempts */
void rotate_piece(Piece *p) {
    Piece tmp = *p;
    tmp.rot = (tmp.rot + 1) & 3;
    const int kicks[5] = {0, -1, 1, -2, 2};
    for (int i = 0; i < 5; ++i) {
        tmp.x = p->x + kicks[i];
        if (!check_collision(&tmp)) { *p = tmp; return; }
    }
}

/* move piece, if dy > 0 and collision -> lock */
void move_piece(Piece *p, int dx, int dy) {
    Piece tmp = *p;
    tmp.x += dx; tmp.y += dy;
    if (!check_collision(&tmp)) { *p = tmp; }
    else if (dy > 0) { /* hit bottom */
        lock_piece();
    }
}

/* hard drop */
void hard_drop(Piece *p) {
    Piece tmp = *p;
    while (1) {
        tmp.y++;
        if (check_collision(&tmp)) {
            tmp.y--;
            *p = tmp;
            lock_piece();
            break;
        }
    }
}

/* draw helpers - draw full board and info; we reposition cursor to top-left to avoid flicker */
void draw() {
    set_cursor(0,0);
    /* draw border and board */
    printf("+");
    for (int i=0;i<BOARD_W*2;i++) putchar('-');
    printf("+   Score: %d  Level: %d  Lines: %d\n", score, level, lines_cleared);
    for (int y=0;y<BOARD_H;++y) {
        putchar('|');
        for (int x=0;x<BOARD_W;++x) {
            int v = board[y][x];
            if (v) {
                /* choose a color & draw two chars to look squareish */
                set_color( (WORD)( (v % 7) + 9) ); /* bright colors 9..15 */
                printf("[]");
                set_color(7);
            } else {
                /* empty */
                printf("  ");
            }
        }
        putchar('|');
        /* info area */
        if (y == 2) printf("   Next:");
        if (y >= 6 && y < 10) {
            /* draw next piece preview */
            int np = nextp.type, nr = nextp.rot;
            int py = y - 6;
            for (int px = 0; px < 4; ++px) {
                if (pieces[np][nr][py][px]) { set_color( (WORD)( (np % 7) + 9) ); printf("[]"); set_color(7); }
                else printf("  ");
            }
        } else {
            printf("\n");
        }
    }
    printf("+");
    for (int i=0;i<BOARD_W*2;i++) putchar('-');
    printf("+\n");
    printf("Controls: ← → rotate↑ ↓ soft drop space hard drop  p pause  q quit\n");
    /* draw current piece on top by reprinting affected rows (simple way) */
    for (int py = 0; py < 4; ++py) {
        for (int px = 0; px < 4; ++px) {
            if (!pieces[cur.type][cur.rot][py][px]) continue;
            int bx = cur.x + px;
            int by = cur.y + py;
            if (by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W) {
                /* compute console coord: top border + line offset */
                int console_x = 1 + bx*2;
                int console_y = 1 + by;
                set_cursor(console_x, console_y);
                set_color( (WORD)( (cur.type % 7) + 9) );
                printf("[]");
                set_color(7);
            }
        }
    }
    fflush(stdout);
}

/* init console: disable cursor blinking (hide) */
void init_console() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    /* hide cursor */
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hConsole, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cci);
}

/* restore cursor visible on exit */
void restore_console() {
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hConsole, &cci);
    cci.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &cci);
}

/* non-blocking input handling */
void handle_input() {
    if (!_kbhit()) return;
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        /* arrow or function key: read second code */
        int code = _getch();
        if (code == 75) { /* left */
            move_piece(&cur, -1, 0);
        } else if (code == 77) { /* right */
            move_piece(&cur, 1, 0);
        } else if (code == 72) { /* up */
            rotate_piece(&cur);
        } else if (code == 80) { /* down */
            move_piece(&cur, 0, 1);
        }
    } else {
        if (ch == 'q' || ch == 'Q') { game_over = 1; }
        else if (ch == 'p' || ch == 'P') {
            /* simple pause loop */
            set_cursor(0, BOARD_H + 3);
            printf("PAUSED - press any key to continue...");
            while (!_kbhit()) Sleep(50);
            _getch();
        } else if (ch == ' ') {
            hard_drop(&cur);
        }
    }
}

/* initialize game */
void init_game() {
    memset(board, 0, sizeof(board));
    score = 0; level = 1; lines_cleared = 0; game_over = 0;
    srand((unsigned)time(NULL));
    nextp.type = rand() % 7;
    nextp.rot = rand() % 4;
    spawn_piece();
    init_console();
}

/* main loop */
int main(void) {
    init_game();
    unsigned int drop_interval_ms = 700;
    DWORD last_tick = GetTickCount();
    while (!game_over) {
        handle_input();
        DWORD now = GetTickCount();
        if (now - last_tick >= drop_interval_ms) {
            Piece tmp = cur;
            tmp.y++;
            if (!check_collision(&tmp)) {
                cur.y++;
            } else {
                lock_piece();
            }
            last_tick = now;
            /* adjust speed based on level */
            drop_interval_ms = 700 - (level - 1) * 50;
            if (drop_interval_ms < 80) drop_interval_ms = 80;
        }
        draw();
        Sleep(15); /* small sleep to reduce CPU */
    }
    /* game over */
    set_cursor(0, BOARD_H + 2);
    set_color(7);
    printf("GAME OVER! Score: %d    Press any key to exit...\n", score);
    _getch();
    restore_console();
    return 0;
}
/* End of tetris_win.c */
/* Compile with: gcc -O2 -o tetris_win.exe tetris_win.c */