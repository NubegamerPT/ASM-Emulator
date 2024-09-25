#include "window.h"

WINDOW *create_newPanel(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0); /* 0, 0 dá caracteres padrão para as linhas verticais and horizontais	*/
    wrefresh(local_win);  /* Mostra aquela caixa 	*/

    return local_win;
}

WINDOW *create_newPanel(int height, int width, int starty, int startx, const char *title)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0); /* 0, 0 dá caracteres padrão para as linhas verticais and horizontais	*/
    mvwprintw(local_win, 0, 2, title);
    wrefresh(local_win); /* Mostra aquela caixa 	*/

    return local_win;
}

void destroy_Panel(WINDOW *local_win)
{
    /* box (local_win, '', ''); : Isso não produzirá o resultado
     *  desejado de apagar a janela. Vai deixar seus quatro cantos,
     * e uma lembrança feia da janela.
     */
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* Os parâmetros usados são
     * 1. win: a janela na qual operar
     * 2. ls: caractere a ser usado para o lado esquerdo da janela
     * 3. rs: caractere a ser usado para o lado direito da janela
     * 4. ts: caractere a ser usado na parte superior da janela
     * 5. bs: caractere a ser usado na parte inferior da janela
     * 6. tl: caractere a ser usado para o canto superior esquerdo da janela
     * 7. tr: caractere a ser usado no canto superior direito da janela
     * 8. bl: caractere a ser usado no canto inferior esquerdo da janela
     * 9. br: caractere a ser usado no canto inferior direito da janela
     */
    wrefresh(local_win);
    delwin(local_win);
}

void setTitle(WINDOW *local_win, const char *title)
{
    mvwprintw(local_win, 0, 2, title);
    wrefresh(local_win);
}

void print(WINDOW *local_win, int y, int x, const char *text)
{
    mvwprintw(local_win, y, x + 1, text);
    wrefresh(local_win);
}

void print(WINDOW *local_win, int y, int x, const char *text,  int num)
{
    mvwprintw(local_win, y, x + 1, "%s%d", text, num);
    wrefresh(local_win);
}
