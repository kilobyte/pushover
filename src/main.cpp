#include "graphicso.h"
#include "level.h"
#include "ant.h"

#include <SDL.h>

#include <vector>

#include <stdio.h>

const char * themes[] = {
  "AZTEC",
  "CASTLE",
  "CAVERN",
  "DUNGEON",
  "ELECTRO",
  "GREEK",
  "JAPANESE",
  "MECHANIC",
  "SPACE",
  "TOXCITY",
  "OPTION"
};

std::vector<int> recorder;
bool play;
unsigned int playpos;

void record(const char * level) {

    int num = 0;
    FILE * f = 0;

    do {
        num++;

        if (f) fclose(f);

        char fname[200];

        snprintf(fname, 200, "recordings/%05i.rec", num);
        f = fopen(fname, "r");

    } while (f);

    char fname[200];

    snprintf(fname, 200, "recordings/%05i.rec", num);
    f = fopen(fname, "w");

    fprintf(f, "%s\n", level);

    int val = recorder[0];
    int cnt = 1;
    unsigned int pos = 1;

    while (pos < recorder.size()) {

        if (recorder[pos] != val) {
            fprintf(f, "%i %i\n", cnt, val);
            val = recorder[pos];
            cnt = 1;
        }
        else
        {
            cnt++;
        }
        pos++;
    }

    fprintf(f, "%i %i\n", cnt, val);
    fclose(f);
}

const char * loadrecord(const char * file) {
    FILE * f = fopen(file, "r");

    static char level[200];
    fgets(level, 200, f);

    // remove the newline at the end
    level[strlen(level)-1] = 0;

    while (!feof(f))
    {
        int cnt, val;
        fscanf(f, "%i %i\n", &cnt, &val);
        for (int i = 0; i < cnt; i++)
            recorder.push_back(val);
    }

    printf("record builds on level %s\n", level);
    return level;
}

int main(int argn, char * argv[]) {

  graphics_c * gr = new graphicsO_c(".", 2);

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *video = SDL_SetVideoMode(gr->resolutionX(), gr->resolutionY(), 24, 0);

  gr->loadGraphics();

  level_c l;

  if (strcmp(argv[1], "-r") == 0)
  {
      l.load(loadrecord(argv[2]));
      play = true;
      playpos = 0;
  }
  else
  {
      l.load(argv[1]);
      play = false;
  }

  gr->setTheme(l.getTheme());

  ant_c a;

  a.init(&l, gr);

  l.updateBackground(gr);

  Uint32 ticks = SDL_GetTicks();

  bool exit = false;
  int tickDiv = 18;
  bool debug = false;
  bool finishCheckDone = false;
  bool pause = false;
  SDL_Rect rects[10*13];

  while (!exit) {

    ticks += 1000/tickDiv;

    unsigned int keyMask = 0;

    if (play && playpos < recorder.size())
    {
        keyMask = recorder[playpos++];
    }
    else
    {
        SDL_Event event; /* Event structure */

        while(SDL_PollEvent(&event) || pause) {  /* Loop until there are no events left on the queue */
            switch(event.type) { /* Process the appropiate event type */
                case SDL_KEYDOWN:  /* Handle a KEYDOWN event */

                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        exit = true;
                    if (event.key.keysym.sym == SDLK_s)
                        tickDiv = 19-tickDiv;
                    if (event.key.keysym.sym == SDLK_d)
                        debug = !debug;
                    if (event.key.keysym.sym == SDLK_r)
                        record(argv[1]);
                    if (event.key.keysym.sym == SDLK_p) {
                        pause = !pause;
                        ticks = SDL_GetTicks() + 1000/tickDiv;
                    }

                    break;
            }

        }
        Uint8 *keystate = SDL_GetKeyState(NULL);

        if ( keystate[SDLK_UP] ) keyMask |= KEY_UP;
        if ( keystate[SDLK_DOWN] ) keyMask |= KEY_DOWN;
        if ( keystate[SDLK_LEFT] ) keyMask |= KEY_LEFT;
        if ( keystate[SDLK_RIGHT] ) keyMask |= KEY_RIGHT;
        if ( keystate[SDLK_SPACE] ) keyMask |= KEY_ACTION;

        recorder.push_back(keyMask);
    }

    if (l.triggerIsFalln() && !finishCheckDone)
    {
      finishCheckDone = true;
      if (l.levelCompleted(0) && !a.carrySomething() && a.isLiving())
      {
        a.success();
      }
      else
      {
        a.fail();
      }
    }

    a.setKeyStates(keyMask);

    l.performDoors();
    a.performAnimation();
    l.performDominos();

    l.updateBackground(gr);
    l.drawDominos(video, gr, debug);
    a.draw(video);

    if (debug)
      l.print();

    if (debug)
    {
      SDL_Flip(video);
    }
    else
    {
      int numrects = 0;

      for (int y = 0; y < 13; y++)
      {
        int rowStart = -1;

        for (int x = 0; x < 21; x++)
        {
          if (l.isDirty(x, y) && (x < 20))
          {
            if (rowStart == -1)
              rowStart = x;
          }
          else if (rowStart != -1)
          {
            rects[numrects].y = gr->blockY()*y;
            rects[numrects].x = gr->blockX()*rowStart;

            if (y == 12)
              rects[numrects].h = gr->blockY()/2;
            else
              rects[numrects].h = gr->blockY();

            rects[numrects].w = gr->blockX()*(x-rowStart);
            numrects++;
            rowStart = -1;
          }
        }
      }
      SDL_UpdateRects(video, numrects, rects);
    }

    l.clearDirty();

    if (l.triggerIsFalln() && !a.isVisible()) {

      if (l.someTimeLeft())
      {
        if (!play)
          record(argv[1]);
      }
      else
      {
        printf("Sorry You've been too slow\n");
      }

      exit = true;
    }

    if (SDL_GetTicks() < ticks)
      SDL_Delay(ticks-SDL_GetTicks());
  }

  delete gr;

  return 0;
}
