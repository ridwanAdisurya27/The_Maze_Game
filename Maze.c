#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// ===============================================================
// TASK : Create entrance screen with name input (done)
// Task : Create stage 1 of maze (done)
// Task : buat player bergerak up, down, left, right (done)
// Task : buat player memiliki vision yang rendah 1 blok dari arah mata angin (done)
// Task : buat fungsi Narator (done)
// Task : buat fungsi jika player mencapai titik akhir, maka muncul text "congratulations, you have finished the maze!"
// ===============================================================

char* Narator_text();

void entrance(int *row, int *col, char *ply_str){
    char greetings[] = "Hello, Player!";
    char name[] = "Enter your name: ";
    getmaxyx(stdscr,*row,*col);
    mvprintw(*row/2,(*col - strlen(greetings))/2,"%s To the Maze Game!",greetings);
    mvprintw(*row/2 + 1,(*col - strlen(name))/2,"%s", name);
    while (1)
    {
        getstr(ply_str);
        if(strlen(ply_str) == 0){
        mvprintw(*row/2,(*col - strlen(greetings))/2,"Enter the name To the Maze Game!");
        wmove(stdscr, *row/2 + 1,(*col - strlen(name))/2 + strlen(name));
        } else {
        mvprintw(*row/2 + 2,(*col - strlen("Welcome To the Maze Game! click your F4 to start the game"))/2,"%s, Welcome To the Maze Game! click your F4 to start the game", ply_str);
        break;
        }
    }
}
// ===============================================================
// TASK : Create stage 
// ===============================================================
char** readStageFile(const char* filename, int* rows, int* cols, int *level, int *p_row, int *p_col) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }

    // Read level from first line
    fscanf(file, "%d", level);
    // Read dimensions from first line
    fscanf(file, "%d %d", rows, cols);
    // Read player starting position
    fscanf(file, "%d %d", p_row, p_col);

    // Allocate memory for the 2D array
    char** map = (char**)malloc(*rows * sizeof(char*));
    for (int i = 0; i < *rows; i++) {
        map[i] = (char*)malloc(*cols * sizeof(char));
    }

    // Skip the newline after dimensions
    fgetc(file);

    // Read the map
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            map[i][j] = fgetc(file);
        }
        // Skip newline
        fgetc(file);
    }

    fclose(file);
    return map;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{ 
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0 , 0); /* 0, 0 gives default characters
    * for the vertical and horizontal
    * lines */
    wrefresh(local_win); /* Show that box */
    return local_win;
}

void printMap(char** stage, int* rows, int* cols, int* p_row, int* p_col, int* row_w, int* col_w, int level) {
    int y,x;
    getyx(stdscr,y,x);
    if (stage != NULL) {
        WINDOW *tutor_win;
        // WINDOW *narator_win;
        // =========================================================
        // Create position for tutor window
        int tutorx, tutory, tutor_w = 20, tutor_h = 10;
        tutory = 0; /* Calculating for a center placement */
        tutorx = ((COLS - tutor_w) / 2) + 15 + *cols; 
        // ========================================================
        // ========================================================
        // Create position for narator window
        // int narator_x, narator_y, narator_w = 20, narator_h = 10;
        // narator_y = 0 ; /* Calculating for a center placement */
        // narator_x = ((COLS - narator_w) / 2) - 15; 
        refresh();
        // printw("Player starting position: (%d, %d)\n", *p_row, *p_col);
        // printw("Map dimensions: %d rows, %d cols\n", LINES, COLS);
        // printw("window size: %d rows, %d cols\n", *row_w, *col_w);
        // printw("cursor position: %d rows, %d cols\n", y, x);
        // printw("windows position: %d rows, %d cols\n", tutory, tutorx);
        tutor_win = create_newwin(tutor_h, tutor_w, tutory, tutorx);
        // narator_win = create_newwin(narator_h, narator_w, narator_y, narator_x);
        
        // Tambahkan konten ke window
        mvwprintw(tutor_win,1,1,"W to up");
        mvwprintw(tutor_win,2,1,"A to left");
        mvwprintw(tutor_win,3,1,"S to down");
        mvwprintw(tutor_win,4,1,"D to right");
        mvwprintw(tutor_win,5,1,"F4 to quit");
        mvwprintw(tutor_win,6,1,"Find E to win");
        wrefresh(tutor_win);  // Tampilkan perubahan di window

        // mvwprintw(narator_win,1,1,"I cant see anything");
        // wrefresh(narator_win);  // Tampilkan perubahan di window
        
        if(level == 1){
            for (int i = 0; i < *rows; i++) {
                for (int j = 0; j < *cols; j++) {
                    /* Draw the static map only. Player will be drawn separately */
                    mvprintw(i,(*col_w/2)+j,"%c", stage[i][j]);
                }
            }
        }
    }
}

void Narator(){
    // Create a new window for narator
    int narator_x, narator_y, narator_w = 20, narator_h = 10;
    narator_y = 0 ; /* Calculating for a center placement */
    narator_x = ((COLS - narator_w) / 2) - 30; 
    refresh();
    WINDOW *narator_win = create_newwin(narator_h, narator_w, narator_y, narator_x);
    char* narator_msg = Narator_text();
    int newline_counter = 1;
    for(int i = 0; i < strlen(narator_msg); i++){
        if(i%20 == 19){
            newline_counter++;
        }
        mvwprintw(narator_win,newline_counter,1 + i,"%c", narator_msg[i]);
        wrefresh(narator_win);  // Tampilkan perubahan di window
        napms(100); // delay 100 milliseconds
    }
    // Tambahkan konten ke window
    wrefresh(narator_win);  // Tampilkan perubahan di window
}

char* Narator_text(){
    static char* narator_msg[] = {
        "I cant See Anything!",
        "Be careful!",
        "Watch your step!",
        "Have your Sleep before?",
        "Why I am here?"
    };
    int index = rand() % 5; // Random index between 0 and 2
    return narator_msg[index];
}

void bersih(char*** stage, int rows, int cols) {
    if (*stage != NULL) {
        // Free memory
        for (int i = 0; i < rows; i++) {
            free((*stage)[i]);
        }
        free(*stage);
        *stage = NULL;  // Set pointer to NULL after freeing
    }
}

// ===============================================================
// END TASK
// ===============================================================

// ===============================================================
// Task : buat player bergerak up, down, left, right
// ===============================================================

int moveChar(int* p_row, int* p_col, int row_level, int col_level,
             int delta_row, int delta_col, char** stage,
             int *old_row, int *old_col) {
    int new_row = *p_row + delta_row;
    int new_col = *p_col + delta_col;
    *old_row = *p_row;
    *old_col = *p_col;

    /* boundary + collision check (anggap '#'=wall) */
    if (new_row >= 0 && new_row < row_level && new_col >= 0 && new_col < col_level && stage[new_row][new_col] != '#') {
        *p_row = new_row;
        *p_col = new_col;
        return 1; /* berhasil pindah */
    }
    return 0; /* tidak pindah */
}

void read_move(int* moved, int ch, int* p_row, int* p_col, int rows, int cols, char** stage, int *old_r, int *old_c, int *confirm, int *narator_counter, int* row){
    switch (ch) {
            case 'a': case 'A':
                *moved = moveChar(p_row, p_col, rows, cols, 0, -1, stage, old_r,old_c);
                if(!*moved) {
                    (*narator_counter)++;  // Increment counter
                    if(*narator_counter % 5 == 0){  // Check AFTER increment
                        Narator();
                    }
                }
                break;
            case 'd': case 'D':
                *moved = moveChar(p_row, p_col, rows, cols, 0, 1, stage, old_r, old_c);
                if(!*moved) {
                    (*narator_counter)++;
                    if(*narator_counter % 5 == 0){
                        Narator();
                    }
                }
                break;
            case 'w': case 'W':
                *moved = moveChar(p_row, p_col, rows, cols, -1, 0, stage, old_r, old_c);
                if(!*moved) {
                    (*narator_counter)++;
                    if(*narator_counter % 5 == 0){
                        Narator();
                    }
                }
                break;
            case 's': case 'S':
                *moved = moveChar(p_row, p_col, rows, cols, 1, 0, stage, old_r, old_c);
                if(!*moved) {
                    (*narator_counter)++;
                    if(*narator_counter % 5 == 0){
                        Narator();
                    }
                }
                break;
            case KEY_F(4):
                if (*confirm == 0) {
                    mvprintw(*row - 2, 0, "Are you sure to quit? Press F4 again to confirm.");
                    *confirm = 1;
                }
                break;
            default:
                *confirm = 0;
                break;
        }
}

// ===============================================================
// END TASK
// ===============================================================

// ===============================================================
// TASK : Create Position for STAGE 3
// ===============================================================
void ppos_rnd(int* p_row, int* p_col, char** stage, int* rows, int* cols, int offset){
    // base case 
    while(stage[*p_row][*p_col] == '#'){
        *p_row = rand()% *rows + offset;
        *p_col = rand()% *cols + offset;
    }
}

void epos_rnd(int* e_row, int* e_col, char** stage, int* rows, int* cols, int* p_row, int* p_col){
    while((abs(*p_row - *e_row) + abs(*p_col - *e_col)) < 7 || stage[*e_row][*e_col] == '#'){
        *e_row = rand()% *rows;
        *e_col = rand()% *cols;
    }
}

void empos_rnd(int* e_row, int* e_col, char** stage, int* rows, int* cols, int* p_row, int* p_col, int* em_pos, int em_row_size, int em_col_size){
    const int MAX_TRIES_PER_SLOT = 10000;
    const int MIN_DISTANCE = 9;
    
    if (em_col_size < 2) return;
    
    for (int i = 0; i < em_row_size; ++i) {
        int tries = 0;
        int found = 0;
        
        while (tries++ < MAX_TRIES_PER_SLOT) {
            int x = rand() % (*rows);
            int y = rand() % (*cols);
            
            // skip walls and distance
            if (stage[x][y] == '#' || (abs(*p_row - x) + abs(*p_col - y)) < MIN_DISTANCE) continue;
            
            // check duplicate against previously chosen positions
            int dup = 0;
            for (int j = 0; j < i; ++j) {
                int px = em_pos[j * em_col_size + 0];
                int py = em_pos[j * em_col_size + 1];
                if (px == x && py == y) { 
                    dup = 1; 
                    break; 
                }
            }
            if (dup) continue;  // generate ulang x,y
            
            // OK — assign position
            em_pos[i * em_col_size + 0] = x;
            em_pos[i * em_col_size + 1] = y;
            found = 1;
            break;
        }
        
        if (!found) {
            // mark failure
            em_pos[i * em_col_size + 0] = -1;
            em_pos[i * em_col_size + 1] = -1;
        }
    }
}

void emmv_rnd(int* e_row, int* e_col, char** stage, int* rows, int* cols, int* p_row, int* p_col, int* em_pos, int em_row_size, int em_col_size){
    for(int i = 0; i < em_row_size; i++){
        // Cek apakah musuh sudah dibunuh
        if(em_pos[i * em_col_size + 1] == -1) continue;
        // init posisi awal
        int e_current_x = em_pos[i * em_col_size];
        int e_current_y = em_pos[i * em_col_size + 1];
        // simpan perubahan 
        int x = em_pos[i * em_col_size];
        int y = em_pos[i * em_col_size + 1];
        int manhattan = (abs(*p_row - x) + abs(*p_col - y));
        // cek langkah atas
        if(stage[e_current_x-1][e_current_y] != '#' && (abs(*p_row - e_current_x + 1) + abs(*p_col - e_current_y)) < manhattan){
            x = e_current_x - 1;
            y = e_current_y;
            manhattan = (abs(*p_row - x) + abs(*p_col - y));
        }
        // cek langkah bawah
        if(stage[e_current_x+1][e_current_y] != '#' && (abs(*p_row - e_current_x - 1) + abs(*p_col - e_current_y)) < manhattan){
            x = e_current_x + 1;
            y = e_current_y;
            manhattan = (abs(*p_row - x) + abs(*p_col - y));
        }
        // cek langkah kiri
        if(stage[e_current_x][e_current_y-1] != '#' && (abs(*p_row - e_current_x) + abs(*p_col - e_current_y + 1)) < manhattan){
            x = e_current_x;
            y = e_current_y - 1;
            manhattan = (abs(*p_row - x) + abs(*p_col - y));
        }
        // cek langkah kanan
        if(stage[e_current_x][e_current_y+1] != '#' && (abs(*p_row - e_current_x) + abs(*p_col - e_current_y - 1)) < manhattan){
            x = e_current_x;
            y = e_current_y + 1;
            manhattan = (abs(*p_row - x) + abs(*p_col - y));
        }
        em_pos[i * em_col_size] = x;
        em_pos[i * em_col_size + 1] = y;
    }
}

void killAction(int* p_row, int* p_col, int* em_pos, int em_row_size, int em_col_size, int* K, bool* p_cond){
    bool isKilled = false;
    bool isDead = false;
    for(int i = 0; i < em_row_size; i++){
        if(em_pos[i * em_col_size] == *p_row && em_pos[i * em_col_size + 1] == *p_col){
            if(*K != 0){
                em_pos[i * em_col_size] = -1;
                em_pos[i * em_col_size + 1] = -1;
                isKilled = true;
            } else {
                isKilled = false;
                isDead = true;
                break;
            }
        }
    }
    if(isKilled){
        move(0,0);
        printw("You killed");
        *K = *K - 1;
        *p_cond = true;
    } else if(isDead){
        move(0,0);
        printw("You Dead");
        *p_cond = false;
    }
}

// ================================================================
// Special Ending
// ================================================================
char** readEndingFile(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }

    // Read dimensions: COLS ROWS
    fscanf(file, "%d %d", cols, rows);

    // Allocate memory for the 2D array
    char** map = (char**)malloc(*rows * sizeof(char*));
    for (int i = 0; i < *rows; i++) {
        map[i] = (char*)malloc((*cols + 1) * sizeof(char));
    }

    // Skip newline
    fgetc(file);

    // Read map lines
    for (int i = 0; i < *rows; i++) {
        fgets(map[i], *cols + 1, file);
        fgetc(file); // skip newline
    }

    fclose(file);
    return map;
}

void renderView(char** map, int rows, int cols, int view_top) {
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    for (int y = 0; y < max_y; y++) {
        int map_y = view_top + y;
        if (map_y >= rows) break;

        for (int x = 0; x < cols && x < max_x; x++) {
            mvaddch(y, x, map[map_y][x]);
        }
    }

    refresh();
}

int main(){
    int ch, direction = 0;
    char ply_str[80] = "";
    int row, col; // get screen size
    int p_row, p_col; // player position
    initscr(); // Start curses mode
    raw(); // Line buffering disabled
    keypad(stdscr, TRUE); // We get F1, F2 etc..
    echo(); 
    cbreak();
    srand(time(NULL)); // Create the Seed
    // ============================================================
    // Task : create entrance screen with name input
    // ============================================================
    entrance(&row, &col, ply_str);
    ch = getch();
    while (1)
    {
        getmaxyx(stdscr, row, col);
        noecho();
        curs_set(0);
        if(ch == KEY_F(4)){
            if(row < 40){
                clear();
                move(0,0);
                start_color();
                init_pair(1, COLOR_RED, COLOR_BLACK);
                attron(COLOR_PAIR(1));
                mvprintw(row/2,(col - strlen("your screen to small..."))/2,"your screen to small...");
                mvprintw(row/2+1, (col - 5 - row)/2, "row: %d", row);
                refresh();
                attroff(COLOR_PAIR(1));
            } else {
            clear(); // hapus semua text di layar
            move(0,0); // pindah kursor ke pojok kiri atas
            start_color(); 
            init_pair(1, COLOR_BLUE, COLOR_BLACK);
            attron(COLOR_PAIR(1));
            mvprintw(row/2,(col - strlen("starting the game..."))/2,"starting the game...");
            attroff(COLOR_PAIR(1));
            use_default_colors();
            refresh();
            
            // Tampilkan loading screen selama 3 detik
            napms(3000);
            
            break;
            }
        } else {
            ch = getch();
        }
    }
    // ============================================================
    // End Task
    // ============================================================


    // ============================================================
    // Task : create stage 1 of maze
    // ============================================================
    clear();
    move(0,0);
    int rows, cols, level;
    char** stage = readStageFile("STAGE1.txt", &rows, &cols, &level, &p_row, &p_col);
    printMap(stage, &rows, &cols, &p_row, &p_col, &row, &col, level);
    Narator();
    curs_set(0); // sembunyikan kursor

    // ============================================================
    // End Task
    // ============================================================

    // ============================================================
    // TASK : buat player bergerak up, down, left, right
    // ============================================================
    int map_y0 = 0;
    int map_x0 = col / 2;
    /* draw player initial position */
    mvaddch(map_y0 + p_row, map_x0 + p_col, 'O' | A_BOLD);
    refresh();
    ch = getch();
    int confirm = 0;
    int old_r, old_c;
    int narator_counter = 0;
    while (1) {
        int moved = 0;
        read_move(&moved, ch, &p_row, &p_col, rows, cols, stage, &old_r, &old_c, &confirm, &narator_counter, &row);
        move(0,0);
        if (moved) {
            // hapus posisi lama dan gambar posisi baru
            mvaddch(map_y0 + old_r, map_x0 + old_c, stage[old_r][old_c]);
            // vision range
            mvaddch(map_y0 + p_row, map_x0 + p_col, 'O' | A_BOLD);
            if(stage[p_row][p_col] == 'E') {
                clear();
                break;
            }
            int vision_range = 1;
            mvaddch(map_y0 + p_row, map_x0 + p_col - vision_range, stage[p_row][p_col - vision_range]); // kiri
            mvaddch(map_y0 + p_row, map_x0 + p_col + vision_range, stage[p_row][p_col + vision_range]); // kanan
            if(p_row + vision_range < rows && p_row + vision_range >= 0) {
                mvaddch(map_y0 + p_row - vision_range, map_x0 + p_col, stage[p_row - vision_range][p_col]); // atas
                mvaddch(map_y0 + p_row + vision_range, map_x0 + p_col, stage[p_row + vision_range][p_col]); // bawah
            }
            // buat posisi karakter baru
            refresh();
        }

        ch = getch();
        if (ch == KEY_F(4) && confirm == 1) {
            bersih(&stage, rows, cols);
            clear();
            refresh();
            endwin();
            printf("YOU TRAP ME HEREEEEEEEEEE!, I ALWAYS REMEMBER YOUR NAME %s!\n", ply_str);
            return 0;
            break;
        }
    }
    move(0,0);
    mvprintw(row/2, (col - strlen("Congratulations, you have finished the maze!"))/2, "Congratulations, you have finished the maze Level %d!", level);
    mvprintw(row/2+1, (col - strlen("Prepare for next Level"))/2, "Prepare for next Level %d!", level+1);
    mvprintw(row/2+2, (col - strlen("Press F4 to continue"))/2, "Press F4 to continue");
    ch = getch();
    while (1)
    {
        noecho();
        if(ch == KEY_F(4)){
            clear(); // hapus semua text di layar
            move(0,0); // pindah kursor ke pojok kiri atas
            start_color(); 
            init_pair(1, COLOR_BLUE, COLOR_BLACK);
            attron(COLOR_PAIR(1));
            mvprintw(row/2,(col - strlen("starting the game..."))/2,"starting the game...");
            attroff(COLOR_PAIR(1));
            use_default_colors();
            refresh();
            
            // Tampilkan loading screen selama 3 detik
            napms(3000);
            
            break;
        }
        ch = getch();
    }
    bersih(&stage, rows, cols);
    
    // ============================================================
    // TASK : STAGE 2 Create
    // ============================================================
    clear();
    move(0,0);
    stage = readStageFile("STAGE2.txt", &rows, &cols, &level, &p_row, &p_col);
    printMap(stage, &rows, &cols, &p_row, &p_col, &row, &col, level);
    Narator();
    curs_set(0);

    // ============================================================
    // Buat Player bergerak
    // ============================================================
    mvaddch(map_y0 + p_row, map_x0 + p_col, 'O' | A_BOLD);
    refresh();
    narator_counter = 0;
    ch = getch();
    while (1) {
        int moved = 0;
        read_move(&moved, ch, &p_row, &p_col, rows, cols, stage, &old_r, &old_c, &confirm, &narator_counter, &row);
        move(0,0);
        printw("%d", narator_counter);
        if (moved) {
            // hapus posisi lama dan gambar posisi baru
            mvaddch(map_y0 + old_r, map_x0 + old_c, stage[old_r][old_c]);
            // vision range
            mvaddch(map_y0 + p_row, map_x0 + p_col, 'O' | A_BOLD);
            if(stage[p_row][p_col] == 'E') {
                clear();
                break;
            }
            // create new vision
            int vision_range = 1;
            mvaddch(map_y0 + p_row, map_x0 + p_col - vision_range, stage[p_row][p_col - vision_range]); // kiri
            mvaddch(map_y0 + p_row, map_x0 + p_col + vision_range, stage[p_row][p_col + vision_range]); // kanan
            if(p_row + vision_range < rows && p_row + vision_range >= 0) {
                mvaddch(map_y0 + p_row - vision_range, map_x0 + p_col, stage[p_row - vision_range][p_col]); // atas
                mvaddch(map_y0 + p_row + vision_range, map_x0 + p_col, stage[p_row + vision_range][p_col]); // bawah
            }
            // buat posisi karakter baru
            refresh();
        }

        ch = getch();
        if (ch == KEY_F(4) && confirm == 1) {
            bersih(&stage, rows, cols);
            clear();
            refresh();
            endwin();
            printf("WEAK!");
            return 0;
            break;
        }
    }
    move(0,0);
    mvprintw(row/2, (col - strlen("Congratulations, you have finished the maze!"))/2, "Congratulations, you have finished the maze Level %d!", level);
    mvprintw(row/2+1, (col - strlen("Prepare for next Level"))/2, "Prepare for next Level %d!", level+1);
    mvprintw(row/2+2, (col - strlen("Press F4 to continue"))/2, "Press F4 to continue");
    ch = getch();
    while (1)
    {
        noecho();
        if(ch == KEY_F(4)){
            clear(); // hapus semua text di layar
            move(0,0); // pindah kursor ke pojok kiri atas
            start_color(); 
            init_pair(1, COLOR_BLUE, COLOR_BLACK);
            attron(COLOR_PAIR(1));
            mvprintw(row/2,(col - strlen("starting the game..."))/2,"starting the game...");
            attroff(COLOR_PAIR(1));
            use_default_colors();
            refresh();
            
            // Tampilkan loading screen selama 3 detik
            napms(3000);
            
            break;
        }
        ch = getch();
    }
    bersih(&stage, rows, cols);
    // ============================================================
    // TASK : STAGE 3 Create
    // ============================================================
    clear();
    move(0,0);
    stage = readStageFile("STAGE3.txt", &rows, &cols, &level, &p_row, &p_col);
    int K = 4;
    printMap(stage, &rows, &cols, &p_row, &p_col, &row, &col, level);
    Narator();
    curs_set(0);
    ppos_rnd(&p_row, &p_col, stage, &rows, &cols, 0); // set random position
    int e_row = p_row, e_col = p_col;
    epos_rnd(&e_row, &e_col, stage, &rows, &cols, &p_row, &p_col); // set exit random position
    // ============================================================
    // TASK : Randomize Exit Way (Pastikan berjarak 7 blok dari user) (done)
    // TASK : Summon 7 enemies Random (done)
    // TASK : Create movement of enemies (done)
    // Task : Summon 4 K point 
    // TASK : Create Player meet Enemies Condition (done)
    // TASK : Special Ending
    // ============================================================
    /* draw player initial position */
    mvaddch(map_y0 + p_row, map_x0 + p_col, 'O' | A_BOLD);
    stage[e_row][e_col] = 'E';
    // ============================================================
    // Create Enemies
    // ============================================================
    int em_row_size = 7, em_col_size = 2;
    int em_pos[em_row_size][em_col_size];
    empos_rnd(&e_row, &e_col, stage, &rows, &cols, &p_row, &p_col, (int *)em_pos, em_row_size, em_col_size);
    // ===========================================================
    // Create Swords and player Conditions
    // ===========================================================
    bool p_cond = true;

    refresh();
    ch = getch();
    narator_counter = 0;
    while (1) {
        int moved = 0;
        read_move(&moved, ch, &p_row, &p_col, rows, cols, stage, &old_r, &old_c, &confirm, &narator_counter, &row);
        move(0,0);
        if (moved) {
            // hapus posisi lama dan gambar posisi baru
            mvaddch(map_y0 + old_r, map_x0 + old_c, stage[old_r][old_c]);
            // dapatkan jarak manhattan
            int manhattan = 9 < (abs(e_row - p_row) + abs(e_col - p_col)) ? 9 : (abs(e_row - p_row) + abs(e_col - p_col));
            // vision range
            mvaddch(map_y0 + p_row, map_x0 + p_col, (manhattan + '0') | A_BOLD);
            if(stage[p_row][p_col] == 'E') {
                clear();
                break;
            }
            
            int vision_range = 1;
            // hapus vision posisi lama
            if(((map_x0 + old_c - vision_range != map_x0 + p_col))){
                mvaddch(map_y0 + old_r, map_x0 + old_c - vision_range, ' '); // kiri
            }
            if(((map_x0 + old_c + vision_range != map_x0 + p_col))){
                mvaddch(map_y0 + old_r, map_x0 + old_c + vision_range, ' '); // kanan
            }
            if(((map_y0 + old_r - vision_range != map_y0 + p_row))){
                mvaddch(map_y0 + old_r - vision_range, map_x0 + old_c, ' '); // atas
            }
            if(((map_y0 + old_r + vision_range != map_y0 + p_row))){
                mvaddch(map_y0 + old_r + vision_range, map_x0 + old_c, ' '); // bawah
            }
            // buat vision baru
            mvaddch(map_y0 + p_row, map_x0 + p_col - vision_range, stage[p_row][p_col - vision_range]); // kiri
            mvaddch(map_y0 + p_row, map_x0 + p_col + vision_range, stage[p_row][p_col + vision_range]); // kanan
            if(p_row + vision_range < rows && p_row + vision_range >= 0) {
                mvaddch(map_y0 + p_row - vision_range, map_x0 + p_col, stage[p_row - vision_range][p_col]); // atas
                mvaddch(map_y0 + p_row + vision_range, map_x0 + p_col, stage[p_row + vision_range][p_col]); // bawah
            }
            // Hapus posisi lama gerakkan musuh
            for(int i = 0; i < em_row_size; i++){
                if(em_pos[i][0] == -1) continue;
                stage[em_pos[i][0]][em_pos[i][1]] = '.';
            }
            // Buat posisi baru musuh
            emmv_rnd(&e_row, &e_col, stage, &rows, &cols, &p_row, &p_col, (int *)em_pos, em_row_size, em_col_size);
            for(int i = 0; i < em_row_size; i++){
                if(em_pos[i][0] == -1) continue;
                int r = em_pos[i][0], c = em_pos[i][1];
                if (r == p_row && c == p_col) {
                    stage[r][c] = '.'; // jangan timpa pemain
                } else {
                    stage[r][c] = 'C'; // atau gunakan konstanta tile kosong
                }
            }
            // cek posisi
            move(row-10,0);
            printw("Player Position : %d, %d ", p_row, p_col);
            printw("Sword Remaining : %d \n", K);
            // printw("Exit Position : %d %d\n", e_row, e_col);
            // for(int i = 0; i < em_row_size; i++){
            //     printw("C%d position : %d, %d \n ", i, em_pos[i][0], em_pos[i][1]);
            // }
            // Kill
            killAction(&p_row, &p_col, (int *)em_pos, em_row_size, em_col_size, &K, &p_cond);
            stage[e_row][e_col] = 'E';
            refresh();
        }
        if(!p_cond){
            bersih(&stage, rows, cols);
            clear();
            refresh();
            endwin();
            printf("Ouch \n");
            return 0;
            break;
        }
        ch = getch();
        if (ch == KEY_F(4) && confirm == 1) {
            bersih(&stage, rows, cols);
            clear();
            refresh();
            endwin();
            printf("YOU TRAP ME HEREEEEEEEEEE!, I ALWAYS REMEMBER YOUR NAME %s!\n", ply_str);
            return 0;
            break;
        }
    }
    move(0,0);
    // ============================================================
    // Render Ending
    // ============================================================
    clear();
    char** Ending = readEndingFile("ENDING.txt", &rows, &cols);
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // get Windows

    int view_top = 0;                           // posisi baris atas
    int view_bottom_limit = rows - max_y;       // batas scrolling bawah
    if (view_bottom_limit < 0) view_bottom_limit = 0;

    renderView(Ending, rows, cols, view_top);
    while ((ch = getch()) != KEY_F(4)) {

        switch (ch) {
        case KEY_DOWN:
            if (view_top < view_bottom_limit)
                view_top++;
            break;

        case KEY_UP:
            if (view_top > 0)
                view_top--;
            break;
        }

        renderView(Ending, rows, cols, view_top);
    }

    // Cleanup
    for (int i = 0; i < rows; i++)
        free(Ending[i]);
    free(Ending);

    // ============================================================
    // End Task
    // ============================================================

    refresh(); // Print it on the screen
    endwin(); // End curses mode
    return 0;
}