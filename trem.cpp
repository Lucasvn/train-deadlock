#include "trem.h"
#include <QtCore>
#include <QtDebug>
#include <string>

#include <semaphore.h>

#define N_REGIONS 7 /* Número de regiões críticas */
#define N_TRAINS 5 /* Número de trens */

/* Definindo um valor numérico para cada região crítica */
#define REGION1 0
#define REGION2 1
#define REGION3 2
#define REGION4 3
#define REGION5 4
#define REGION6 5
#define REGION7 6

/*
 * Definindo um valor numérico
 * para cada estado que uma
 * região crítica pode assumir.
 */
#define LIVRE 0
#define OCUPADO 1

sem_t s[N_REGIONS]; //
sem_t semaforo_T1;
sem_t semaforo_T2;
sem_t semaforo_T3;


/* Guarda o status de um trem em relação a uma região */
int statusTrens[N_REGIONS][N_TRAINS] = {};
int statusRegion[N_REGIONS] = {};

//Construtor
Trem::Trem(int ID, int x, int y){
    this->ID = ID;
    this->x = x;
    this->y = y;
    this->velocidade = 50;
}

void Trem::setVelocidade(int velocidade){
    this->velocidade = velocidade;
}

int Trem::getSemaforo(int x, int y){
    if (x == 200) {
        if (y == 30){
            return 0; // semaforo da região 1
        } else if (y == 150) {
            return 1; // semaforo das regiões 1 - 3 - 4
        }
    } else if (x == 340) {
        if (y == 30){
            return 2; // semaforo da região 2
        } else if (y == 150) {
            return 3; // semaforo das regiões 2 - 5 - 6
        }
    } else if (x == 270) {
        if (y == 150) {
            return 4; // semaforo das regiões 4 - 5 - 7
        } else if (y == 280) {
            return 5; // semaforo da região 7
        }
    } else if (y == 150){
        if (x == 130){
            return 6; // semaforo da região 3
        } else if (x == 410) {
            return 7; // semaforo da região 6
        }
    }
    return -1;
}

int Trem::getRegion(int x, int y){
    if (y == 150 || y == 160 || y == 140) {
        if (x >= 130 && x < 180) {
            /* Região crítica 3 */
            return REGION3;
        } else if (x > 200 && x < 250) {
            /* Região crítica 4 */
            return REGION4;
        } else if (x > 270 && x < 320) {
            /* Região crítica 5 */
            return REGION5;
        } else if (x > 340 && x <= 420) {
            /* Região crítica 6 */
            return REGION6;
        }
    }
    if ((x == 200 || x == 190) && y >= 30 && y <= 150) {
        /* Região crítica 1 */
        return REGION1;
    } else if ((x == 340 || x == 330) && y >= 30 && y <= 150) {
        /* Região crítica 2 */
        return REGION2;
    } else if ((x == 270 || x == 260 || x == 280) && y >= 150 && y <= 280) {
        /* Região crítica 7 */
        return REGION7;
    }
    return -1;
}

void Trem::right(){
    this->x += 10;
}

void Trem::left(){
    this->x -= 10;
}

void Trem::up(){
    this->y -= 10;
}

void Trem::down(){
    this->y += 10;
}

//Função a ser executada após executar trem->START
void Trem::run(){
    sem_init(&semaforo_T1, 0, 3);
    sem_init(&semaforo_T2, 0, 3);
    sem_init(&semaforo_T3, 0, 2); //cruzamento 3
    for (int i=0; i < sizeof(&s); i++) {
        sem_init(&s[i], 0, 1);
    }
    while(true){
        switch(ID){
        case 1:     //Trem 1
            if (y == 30 && x < 200){
                if (x == 180){
                    sem_wait(&semaforo_T1); // semaforo do cruzamento em T 1
                    sem_wait(&s[REGION1]);
                }
                right();
            }else if (x == 200 && y < 150){
                if (y == 130){
                    sem_wait(&s[REGION3]);
                }
                down();
            }else if (x > 60 && y == 150){
                if (x == 180){
                    sem_post(&s[REGION1]);
                }
                if (x == 110){
                    sem_post(&s[REGION3]);
                    sem_post(&semaforo_T1); // semaforo do cruzamento em T 1
                }
                left();
            }else{
                up();
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 2: //Trem 2
            if (y == 30 && x < 340){
                if (x == 220) {
                    sem_post(&semaforo_T1);
                    sem_post(&s[REGION1]);
                }
                if (x == 320){
                    sem_wait(&semaforo_T2);
                    sem_wait(&s[REGION2]);
                }
                right();
            }else if (x == 340 && y < 150){
                if (y == 130){
//                    sem_wait(&semaforo_T3);
                    sem_wait(&s[REGION5]);
                }
                down();
            }else if (x > 200 && y == 150){
                if (x == 320){
                    sem_post(&s[REGION2]);
                }
                if (x == 290){
                    sem_wait(&semaforo_T1);
                    sem_wait(&s[REGION4]);
                }
                if (x == 250){
                    sem_post(&s[REGION5]);
                    sem_post(&semaforo_T2);
                }
                if (x == 220){
                    sem_wait(&s[REGION1]);
                }
                left();
            }else{
                if (x == 200 && y == 130){
//                    sem_post(&semaforo_T3);
                    sem_post(&s[REGION4]);
                }
                up();
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 3: //Trem 3
            if (y == 30 && x < 480){
                if(x == 360){
                    sem_post(&s[REGION2]);
                    sem_post(&semaforo_T2);
                }
                right();
            }
            else if (x == 480 && y < 150)
                down();
            else if (x > 340 && y == 150){ // semaforo 6
                if(x == 430){
                    sem_wait(&semaforo_T2);
                    sem_wait(&s[REGION6]);
                }
                if(x == 360){
//                    sem_wait(&semaforo_T3);
                    sem_wait(&s[REGION2]);
                }
                left();
            }
            else{
                if(x == 340 && y == 130){
//                    sem_post(&semaforo_T3);
                    sem_post(&s[REGION6]);
                }
                up();
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 4: //Trem 4
            if (y == 150 && x < 270){
                if(x==180){
                    sem_wait(&s[REGION4]);
//                    sem_wait(&semaforo_T3);
                }
                if(x == 250){
                    sem_wait(&s[REGION7]);
                }
                if (x == 220){
                    sem_post(&s[REGION3]);
                }
                right();
            }else if (x == 270 && y < 280){
                if (y == 170){
                    sem_post(&s[REGION4]);
                    sem_post(&semaforo_T1);
                }
                down();
            }else if (x > 130 && y == 280){
                if(x==250){
//                    sem_post(&semaforo_T3);
                    sem_post(&s[REGION7]);
                }
                left();
            }else{
                if(x==130 && y==170){
                    sem_wait(&semaforo_T1);
                    sem_wait(&s[REGION3]);
                }
                up();
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        case 5: //Trem 5
            if (y == 150 && x < 410){
                if (x == 290){
                    sem_post(&s[REGION7]);
                }
                if (x == 320){
                    sem_wait(&s[REGION6]);
                }
                if (x == 360){
                    sem_post(&s[REGION5]);
//                    sem_post(&semaforo_T3);
                }
                right();
            }else if (x == 410 && y < 280){
                if (y == 170){
                    sem_post(&s[REGION6]);
                    sem_post(&semaforo_T2);
                }
                down();
            }else if (x > 270 && y == 280){
                if (x == 290){
//                    sem_wait(&semaforo_T3);
                    sem_wait(&s[REGION7]);
                }
                left();
            }else{
                if (x == 270 && y == 170){
                    sem_wait(&semaforo_T2);
                    sem_wait(&s[REGION5]);
                }
                up();
            }
            emit updateGUI(ID, x,y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}




