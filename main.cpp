#include <cstdlib>
#include <conio.h>
#include <iostream>
#include <time.h>
#include <thread> //-std=gnu+11
#include <limits.h>
#include <direct.h>
#include <windows.h>
#include <string>
#include <ctype.h>

using namespace std;

char state;

class player_stats{
public:
int score, is_alive;
int score_multiplier;       //Начисление очков в зависимости со сложностью
short position;
short does_jump;            //В прыжке ли игрок
int detect_score(int speed){       //Вычисление количества начисляемыъ очков
    score_multiplier = 5 - (speed/100);
}
void check_position(int field[5][40]);
};

class game_screen{
public:
int field[5][40]  =   {
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
                        {0,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,}
                       };
int speed = 200;
};

void player_stats::check_position(int field[5][40]){
    if (position == 2)
        {
            does_jump = 0;
            field[2][1] = 0;
            field[3][1] = 3;

            if(field[4][1] == 1)
                score = score + score_multiplier;
        }

    if (position == 3)
        {
            if(field[4][1] == 1)
                score = score + score_multiplier;

            field[3][1] = 0;
            field[4][1] = 3;

            position = 0;
        }

    if ((position != 0)&&(does_jump != 1)&&(position !=3))
        position++;


}

void render_block(int field[5][40])     //Функция генерации блоков
{
    int block_position = 0, i;
        block_position = rand()%3;

        if (((block_position % 3)==0)&&(field[4][38]!=1)&&(field[4][37]!=1)&&(field[4][36]!=1)&&(field[4][35]!=1))
        {
            field[4][39] = 1;
        }

        for(i = 1; i < 40; i++)
        {
            if (field[4][i] == 1)
            {
                field[4][i-1] = 1;
                field[4][i] = 2;
            }
        }
}

void print_field(int field[5][40], int &alive)  //Функция выведения поля на экран
{
int i,j;
char str_buf[40];
     for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 40; j++)
            {
                if (field[i][j] == 1)
                    {
                        str_buf[j] = static_cast<char>(219);
                    }

                if (field[i][j] == 2)
                    {
                        str_buf[j] = static_cast<char>(22);
                    }

                if (field[i][j] == 0)
                    {
                        str_buf[j] = static_cast<char>(255);
                    }

                if (field[i][j] == 3)
                    {
                        str_buf[j] = static_cast<char>(64);
                        alive=1;
                    }
            }

            printf("%s\n",str_buf);

    }
}

void game_tick(int field[5][40], short &position, short &does_jump, int &score, int &score_multiplier)
{
    int i, j, alive;

    if (position == 1)
    {
        field[3][1]=0;
        field[2][1]=3;
        position = 2;
        if(field[4][1] == 1)
            score = score + score_multiplier;
    }


    if ((state == 'w')&&(position == 0)&&(field[4][1]==3))
        {
            position = 1;
            does_jump = 1;

            field[3][1]=3;
            field[4][1]=2;
        }

    print_field(field, alive);
    render_block(field);
    field[4][0] = 0;
    state = 0;

    printf("\n\n");
    printf("Score : %i", score);
    printf("\n");
    printf("Maintain the speed in-game(1 - 4)");

    if (alive != 1)
        {
            printf("\n\n");
            printf("**********************************Game Over**********************************");
            exit(0);
        }

}

void player_move(){
    while(1)
    {
    Sleep(260);
    state = getch();
    }
}


int main() {
    player_stats player;
    game_screen screen;
    player.score = 0;

    std::thread player_thread;
    player_thread = std::thread(player_move);


    while(1)
    {
        game_tick(screen.field, player.position, player.does_jump, player.score, player.score_multiplier);

        Sleep(screen.speed);
        system("cls");

        if(isdigit(state) && (int)state<53)
            {
            screen.speed = (400 - ((int(state)-49) * 100));
            }

        player.detect_score(screen.speed);

        if(state == 'q')
            return 0;

        player.check_position(screen.field);

    }

    system("pause");
    return 0;
}

