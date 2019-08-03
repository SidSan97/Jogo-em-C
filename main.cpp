#include <SDL.h>
#include "SDL_opengl.h"
#include <stdio.h>
#include <SDL_image.h>
#include "string"
#define BLOCOS 20  //QUANTIDADE DE BLOCOS

GLuint loadTexture(const std::string&fileName)
{
    SDL_Surface *imagem = IMG_Load(fileName.c_str()); //carrega imagem
    printf("Oh My Goodness, an error : %s", IMG_GetError());
    SDL_DisplayFormatAlpha(imagem); //formato alpha = transparência
    unsigned objetct(0); //cria textura
    glGenTextures(1, &objetct); //gera textura
    glBindTexture(GL_TEXTURE_2D, objetct); //tipo de textura 2D

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagem->w, imagem->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagem->pixels);

    SDL_FreeSurface(imagem);

    return objetct;
}

    // A - elemento 1 - inimigo
    // B - elemento 2 - personagem

    bool colisao(float Ax, float Ay, float Acomp, float Aalt, float Bx, float By, float Bcomp, float Balt)
    {
        if (Ay+Aalt < By) return false;
        else if (Ay > By+Balt) return false;
        else if (Ax+Acomp < Bx) return false;
        else if (Ax > Bx+Bcomp) return false;

        return true;  // houve a colisão
    }
    struct Bloco
    {
        float x;
        float y;
        float comp;
        float alt;
        bool vivo;
    };


int main(int argc, char* args[]){

    SDL_Init(SDL_INIT_EVERYTHING);  //inicializa tudo!!
    //IMG_Init(IMG_INIT_JPEG);
    SDL_Surface *tela = NULL;
    tela = SDL_SetVideoMode(600, 400, 32, SDL_SWSURFACE);

    //LIMITADOR DE MEMÓRIA
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //NOME DA JANELA
    SDL_WM_SetCaption("Meu primeiro jogo SDL", NULL);

    //TAMANHO DA JANELA (LARGURA, ALTURA, BITS POR QUADRO, TIPO JANELA
    SDL_SetVideoMode(600, 400, 32, SDL_OPENGL);

    //COR DA JANELA
    glClearColor(1,1,1,1);

    //aréa de exibição da janela
    glViewport(0,0,600,400);

    //sombra
    glShadeModel(GL_SMOOTH);

    //MODO 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); //desenho geométrico

    //3D
    glDisable(GL_DEPTH_TEST);

    //uso da imagem
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_Surface *imagem, *carregaImagem;
    carregaImagem = IMG_Load("ceu.png");
    imagem = SDL_DisplayFormat(carregaImagem);

    bool executando = true;

    SDL_Event eventos;

    //personagem
    float personX     = 300;
    float personY     = 350;
    float personComp  = 50;
    float personAlt   = 30;

    //inimigo
    float inimigoX  = 50;
    float inimigoY  = 100;
    float inimigoCA = 30;

    float velX = 0.05;
    float velY = 0.05;

    Bloco blocos[BLOCOS];

    //criação dos blocos
    for(int n = 0, x = 4, y = 10; n < BLOCOS; n++, x += 61)
    {
        if (x > 560)
        {
            x = 4;
            y += 23;
        }

        blocos[n].x     = x;
        blocos[n].y     = y;
        blocos[n].comp  = 60;
        blocos[n].alt   = 20;
        blocos[n].vivo  = true;
    }

    bool esq = false, dir = false;

    unsigned int person_textura = 0;
    person_textura = loadTexture("retangulo.png");

    //loop do jogo
    while(executando)
        {
        //eventos
        while(SDL_PollEvent(&eventos))
        {
        if(eventos.type==SDL_QUIT)
                {  //fecha com o X da janela
                executando = false;
                }
             //fehcar com o ESC}
        if(eventos.type==SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE)
           {
            executando = false;
           }
           //ANDA OU NÃO
         if(eventos.type==SDL_KEYDOWN)  //movimento com a tecla pressionada
        {
             if (eventos.key.keysym.sym == SDLK_LEFT)
             {
                 esq = true;
             }
             else if (eventos.key.keysym.sym == SDLK_RIGHT)
             {
                 dir = true;
             }

        }
          else if (eventos.type == SDL_KEYUP)
         {
             if (eventos.key.keysym.sym == SDLK_LEFT)
             {
                 esq = false;
             }
          else if (eventos.key.keysym.sym == SDLK_RIGHT)
             {
                 dir = false;
             }
         }
        }

    //movimento do inimigo
     inimigoX += velX;
     for (int n = 0; n < BLOCOS; n++)
     {
         if (blocos[n].vivo == true)
         {
             if (colisao(inimigoX, inimigoY, inimigoCA, inimigoCA, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt) == true)
             {
                 velX = -velX;
                 blocos[n].vivo = false;
                 break;
             }
         }
     }

     inimigoY += velY;
     for (int n = 0; n < BLOCOS; n++)
     {
         if (blocos[n].vivo == true)
         {
             if (colisao(inimigoX, inimigoY, inimigoCA, inimigoCA, blocos[n].x, blocos[n].y, blocos[n].comp, blocos[n].alt) == true)
             {
                 velY = -velY;
                 blocos[n].vivo = false;
                 break;
             }
         }
     }

     //LOGICA

     //logica do inimigo
     if(inimigoX < 0)
     {
         velX = -velX;
     }
     else if (inimigoX + inimigoCA > 600)
     {
         velX = -velX;
     }
     else if (inimigoY < 0)
     {
         velY = -velY;
     }
     else if (inimigoY + inimigoCA > 400)
     {
        executando = false;
         printf("\n\t\t\t------------------------------\n");
         printf("GAME OVER!!\n");
         printf("\n\t\t\t------------------------------\n");

     }

     if (colisao(inimigoX, inimigoY, inimigoCA, inimigoCA, personX, personY, personComp, personAlt)== true)
     {
         velY = -velY;
        // velX = -velX;
     }

     //movimento do personagem
     if(esq == true) //se seta esquerda pressionada
     {
         personX -= 0.1;
     }
     else if(dir == true)  //se seta direita pressionada
     {
         personX += 0.1;
     }

     if(personX < 0)
     {
         personX = 0;;
     }
     else if (personX + personComp > 600)
     {
         personX = 600 - personComp;
     }

    //RENDERIZAÇÃO
    glClear(GL_COLOR_BUFFER_BIT);  //LIMPEZA DE BUFFER

    //inicia a matriz
    glPushMatrix();

    //dimensões da matriz
    glOrtho(0, 600, 400, 0, -1, 1);

    //cor
    glColor4ub(255, 255, 255, 255); //branco

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, person_textura);

    //inicia o desenho do personagem
    glColor4ub(0, 255, 0, 255);
    glBegin(GL_QUADS);

    glTexCoord2d(0, 0);   glVertex2f(personX, personY);   //primeiro ponto
    glTexCoord2d(1, 0);   glVertex2f(personX+personComp, personY);  //segundo ponto
    glTexCoord2d(1, 1);   glVertex2f(personX+personComp, personY+personAlt);
    glTexCoord2d(0, 1);   glVertex2f(personX, personY+personAlt);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    //inicia desenho do inimigo
    glColor4ub(0, 0, 255, 255);
    glBegin(GL_QUADS);

    glVertex2f(inimigoX, inimigoY);   //primeiro ponto
    glVertex2f(inimigoX+inimigoCA, inimigoY);  //segundo ponto
    glVertex2f(inimigoX+inimigoCA, inimigoY+inimigoCA);
    glVertex2f(inimigoX, inimigoY+inimigoCA);

    glEnd();

    //DESENHO DOS BLOCOS
    glBegin(GL_QUADS);

    glColor4ub(0, 0, 255, 255);
    for (int n = 0; n < BLOCOS; n++)

    {   if(blocos[n].vivo == true)
        {
        glVertex2f (blocos[n].x, blocos[n].y);
        glVertex2f (blocos[n].x + blocos[n].comp, blocos[n].y);
        glVertex2f (blocos[n].x + blocos[n].comp, blocos[n].y + blocos[n].alt);
        glVertex2f (blocos[n].x, blocos[n].y + blocos[n].alt);
        }
    }

    //fecha o desenho
    glEnd();

    //fecha a matriz
    glPopMatrix();


    //ANIMAÇÃO
    SDL_GL_SwapBuffers();

    SDL_FillRect(tela, NULL, 0);
    SDL_BlitSurface(imagem, NULL, tela, NULL);

    SDL_Flip(tela);

    }

    glDisable(GL_BLEND);  //EXIBIÇÃO DA JANELA EM OPENGL

    printf("\n Executando \n");

    //SDL_Delay(5000);
    SDL_FreeSurface(imagem);
    IMG_Quit();
    SDL_Quit();  //fecha todas as inicializações
  return 0;
}
