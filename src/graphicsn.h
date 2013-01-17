/* Pushover
 *
 * Pushover is the legal property of its developers, whose
 * names are listed in the COPYRIGHT file, which is included
 * within the source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA
 */

#ifndef __GRAPHICS_N_H__
#define __GRAPHICS_N_H__

#include "graphics.h"
#include "screen.h"
#include "ant.h"

#include <string>
#include <vector>

class pngLoader_c;
class levelData_c;
class ant_c;

/* implementation for graphics class using the original graphics */
class graphicsN_c : public graphics_c {

  public:

    graphicsN_c(const std::string & path);
    ~graphicsN_c(void);

    virtual unsigned int resolutionX(void) const { return 800; }
    virtual unsigned int resolutionY(void) const { return 600; }

    /* to get the block size of one block */
    virtual unsigned int blockX(void) const { return 40; }
    virtual unsigned int blockY(void) const { return 48; }

    void markAllDirty(void) { dirty.markAllDirty(); }
    const bitfield_c & getDirty(void) const { return dirty; }
    void clearDirty(void) { dirty.clearDirty(); }

    void setPaintData(const levelData_c * l, const ant_c * a, surface_c * t);

    void drawLevel(void);

    SDL_Surface * getBoxBlock(unsigned int num) { return boxBlocks[num]; }
    // get domino animation image
    SDL_Surface * getDomino(unsigned int domino, unsigned int image) { return dominos[domino][image]; }

  private:

    // return numbers of images for a given ant animation
    int getAntOffset(unsigned int animation, unsigned int step) { return step<antImages[animation].size()?antImages[animation][step].ofs:0; }

    // add an image to a specific ant animation, the new image is added at the end
    // you must also provide an y-offset used when animating to displace the image
    // if free is false it is assumed that the surface is used elsewhere and not freed
    // on deletion of object
    void addAnt(unsigned int anim, unsigned int img, signed char yOffset, SDL_Surface * v, bool free = true);

    // get ant animation image
    SDL_Surface * getAnt(unsigned int animation, unsigned int step) { return antImages[animation][step].v; }

  private:

    std::string dataPath;

    void getAnimation(AntAnimationState anim, pngLoader_c * png);

    signed int getCarryOffsetX(unsigned int animation, unsigned int image) const;
    signed int getCarryOffsetY(unsigned int animation, unsigned int image) const ;
    signed int getMoveOffsetX(unsigned int animation, unsigned int image) const;
    signed int getMoveOffsetY(unsigned int animation, unsigned int image) const;
    signed int getMoveImage(unsigned int animation, unsigned int image) const;

    int Min, Sec;   // number of minutes and seconds shown in display

    /* this surface contains the background. It is only updated when necessary
     * the content it used to restore stuff behind the sprites
     */
    surface_c * background;
    surface_c * target;

    const ant_c * ant;
    const levelData_c * level;

    bitfield_c dirty, dirtybg;

    std::vector<std::string> themeNames;

    std::vector<std::vector<SDL_Surface *> > bgTiles;
    std::vector<std::vector<SDL_Surface *> > fgTiles;

    std::vector<std::vector<SDL_Surface *> > dominos;
    std::vector<std::vector<SDL_Surface *> > carriedDominos;

    std::vector<SDL_Surface *> boxBlocks;

    typedef struct {
      SDL_Surface * v;
      int ofs;
      bool free;
    } antSprite;

    std::vector<std::vector<antSprite> > antImages;

    unsigned int curTheme;

    // get tile for currenly active theme
    SDL_Surface * getBgTile(unsigned int num) {
      if (num < bgTiles[curTheme].size())
        return bgTiles[curTheme][num];
      else
        throw std::exception();
    }
    SDL_Surface * getFgTile(unsigned int num) {
      if (num < fgTiles[curTheme].size())
        return fgTiles[curTheme][num];
      else
        throw std::exception();
    }


    // y offset to use when drawing the ant for a given animation


    // get image for a dmino carried by the ant
    SDL_Surface * getCarriedDomino(unsigned int image, unsigned int domino) { return carriedDominos[image][domino]; }


    static const unsigned char numDominoTypes;
    static const unsigned char numDominos[23];

    virtual int timeXPos(void) const { return 5*18/2; }
    virtual int timeYPos(void) const { return 3*186; }

    void loadTheme(const std::string & name);
    void setTheme(const std::string & name);

    virtual int getDominoYStart(void) const { return 3*4; }
    virtual int convertDominoX(int x) const { return 5*x/2; }
    virtual int convertDominoY(int y) const { return 3*y; }
    virtual int splitterY(void) const { return 3*12; }

    virtual void drawAnt(void);

    /* draw the changed stuff into the target surface */
    void drawDominos(void);

    void markDirty(int x, int y) { dirty.markDirty(x, y); }
    void markDirtyBg(int x, int y) { dirtybg.markDirty(x, y); dirty.markDirty(x, y); }
    bool isDirty(int x, int y) const { return dirty.isDirty(x, y); }


};

#endif

