/* game 2048 */
#include<stdio.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<conio.h>

#define N 4
#define GOAL 512

/* game table */
int tab[N][N];

/* direction command */
enum { RIGHT, LEFT, UP, DOWN };

/* keep track of blanks */
struct blank{
    int row;
    int col;
} blanks[N*N];

/* direction infomation */
struct dir{
    const char direction;
    const int rowmin;
    const int rowmax;
    const int colmin;
    const int colmax;
    const int rdir;
    const int cdir;
    const int rstep;
    const int cstep;
} dirs[4] = {
    {'d', 0, N, N - 2, -1, 0, 1, 1, -1},
    {'a', 0, N, 1, N, 0, -1, 1, 1},
    {'w', 1, N, 0, N, -1, 0, 1, 1},
    {'s', N - 2, -1, 0, N, 1, 0, -1, 1}
};

/* flags */
struct flag{
    unsigned is_full : 1;
    unsigned is_stuck : 1;
    unsigned win : 1;
} flags;

void print_tab(void);
int check_full(void);
void seed(int k);
void move(unsigned index);
void check_win(void);
void swap(int *a, int *b);

int main(void)
{
    srand((unsigned)time(NULL));
    int k = 0;
    char cmd;
    //fclose(stdout);
    while(true)
    {
        if((k = check_full()) != 0)
            seed(k);
        check_win();
        if(flags.is_stuck && flags.is_full)
        {
            print_tab();
            printf("****YOU LOST****\n");
            break;
        }
        else if(flags.win)
        {
            print_tab();
            printf("****YOU WIN****\n");
            break;
        }
        print_tab();
        printf("enter w(up),s(down),a(left),d(right)\n");
        switch(cmd = getch())
        {
            case 'd':
                move(RIGHT);break;
            case 'a':
                move(LEFT);break;
            case 'w':
                move(UP);break;
            case 's':
                move(DOWN);break;
            default:
                printf("BAD\n");
                sleep(1);
        }
        // while((cmd = getchar()) != '\n')
            // if(cmd == EOF) exit(1);
    }

}

/* print_tab: print out the game table */
void print_tab(void)
{
    system("cls");
    printf("2048\n");
    for(int i = 0 ; i < N ; i++)
    {
        putchar('+');
        for(int j = 0 ; j < N ; j++)
            printf("---+");
        putchar('\n');
        for(int j = 0 ; j < N ; j++)
        {
            if(tab[i][j])
                printf("|%3d", tab[i][j]);
            else
                printf("|   ");
        }
        printf("|\n");
    }
    putchar('+');
    for(int j = 0 ; j < N ; j++)
        printf("---+");
    putchar('\n');
    //printf("stuck: %d full: %d\n", flags.is_stuck, flags.is_full);
}

/* check_full: check the whole table, return number of blanks, update blanks table */
int check_full(void)
{
    int k = 0;
    int cnt = 0;

    for(int i = 0 ; i < N ; i++)
        for(int j = 0 ; j < N ; j++)
            if(!tab[i][j])
            {
                blanks[cnt].row = i;
                blanks[cnt++].col = j;
                k++;
            }
    if(k > 1)
        flags.is_full = 0;
    else
        flags.is_full = 1;
    return k;
}

/* seed: among the blanks, seed 2 or 4 randomly */
void seed(int k)
{
    int r = rand() % k;
    int s = (rand() % 2 + 1) * 2;
    tab[blanks[r].row][blanks[r].col] = s;
}

/* move:
    1.move numbers in the table as long as there is a space
    2.combine two numbers if they are equal
*/
void move(unsigned index)
{
    if(index > 3)
        return;
    int rn = 0, cn = 0, r = 0, c = 0;
    for (int i = dirs[index].rowmin ; i != dirs[index].rowmax ; i += dirs[index].rstep)
        for (int j = dirs[index].colmin ; j != dirs[index].colmax ; j += dirs[index].cstep)
        {
            if(tab[i][j])
            {
                rn = i + dirs[index].rdir;
                cn = j + dirs[index].cdir;
               while(rn < N && cn < N && rn >= 0 && cn >= 0 && tab[rn][cn] == 0)
               {
                   rn += dirs[index].rdir;
                   cn += dirs[index].cdir;
               }
                r = rn - dirs[index].rdir;
                c = cn - dirs[index].cdir;
                swap(&tab[i][j], &tab[r][c]);

            }
        }
    for (int i = dirs[index].rowmin ; i != dirs[index].rowmax ; i += dirs[index].rstep)
        for (int j = dirs[index].colmin ; j != dirs[index].colmax ; j += dirs[index].cstep)
            if(tab[i][j] && tab[i][j] == tab[i + dirs[index].rdir][j + dirs[index].cdir])
            {
                tab[i + dirs[index].rdir][j + dirs[index].cdir] *= 2;
                tab[i][j] = 0;
            }
            else if(tab[i + dirs[index].rdir][j + dirs[index].cdir] == 0)
                swap(&tab[i][j], &tab[i + dirs[index].rdir][j + dirs[index].cdir]);
}

/* check_win: win or lose */
void check_win(void)
{
    for(int i = 0 ; i < N ; i++)
        for(int j = 0 ; j < N ; j++)
        {
            if(tab[i][j] == GOAL)
            {
                flags.win = 1;
                return;
            }
            for(int k = 0 ; k < 4 ; k++)
            {
                if(i + dirs[k].rdir >= 0 && i + dirs[k].rdir < N)
                    if(j + dirs[k].cdir >= 0 && j + dirs[k].cdir <N)
                        if(tab[i][j] == tab[i + dirs[k].rdir][j + dirs[k].cdir])
                        {
                            flags.is_stuck = 0;
                            return;
                        }
            }
        }

    flags.is_stuck = 1;
    return;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
