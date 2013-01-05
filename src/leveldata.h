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

#ifndef __LEVEL_DATA_H__
#define __LEVEL_DATA_H__

#include <string>

class textsections_c;

class levelData_c {

  private:

    static const unsigned int version = 2;
    static const std::string dominoChars;
    static bool isDominoChar(char ch);
    static const unsigned char maxBg = 8;

    std::string name;
    std::string theme;
    std::string hint;

    std::string checksum;
    std::string checksumNoTime;

    // the number of 1/18 seconds that are left for solving the level
    int timeLeft;

    // the positions of the 2 doors
    // y position is the _lower_ block of the door, so the door
    // extends one block above that number
    unsigned char doorEntryX, doorEntryY, doorExitX, doorExitY;
    // open and close animation for door
    unsigned char doorEntryState, doorExitState;

    typedef struct levelEntry {
      unsigned short bg[maxBg];
      bool platform;
      bool ladder;
      unsigned char dominoType;
      unsigned char dominoState;
      char dominoDir;
      char dominoYOffset;
      char dominoExtra;  // this field contains a lot of information:
        // 0-20 for delay domino is the number of ticks left until it falls
        // 0-9  for splitter is the domino that splits the splitter (for display)
        // 0x40 for all dominos means it is falling of the edge and still turning, so
        //      please fall slower
        // 0x60 riser rising
        // 0x70 falling domino, pile of rubbish...
    } levelEntry;

    levelEntry level[27][20];  //TODO eigentlich nur 25 ebenen, die 26 nur fuer einfacheren Code...
    unsigned char numBg;

    bool triggerFalln;


  public:
    levelData_c(void);
    virtual ~levelData_c(void) {}

    virtual void load(const textsections_c & sections, const std::string & userString);
    void save(std::ostream & stream) const;
    bool operator==(const levelData_c & other) const;

    const std::string getName(void) const { return name; }
    const std::string getTheme(void) const { return theme; }
    const std::string getHint(void) const { return hint; }

    int getTimeLeft(void) const { return timeLeft; }
    void timeTick(void) { timeLeft--; }
    bool someTimeLeft(void) { return timeLeft > 0; }

    const std::string getChecksum(void) const { return checksum; }
    const std::string getChecksumNoTime(void) const { return checksumNoTime; }

    unsigned char getNumBgLayer(void) const { return numBg; }
    void setNumBgLayer(unsigned char n) { numBg = n; }


    // OLD INTERFACE, Deprecated

    /* Foreground elements */
    enum {
      FgElementEmpty,              // 0
      FgElementPlatformStart,
      FgElementPlatformMiddle,
      FgElementPlatformEnd,
      FgElementPlatformLadderDown,
      FgElementLadder,             // 5
      FgElementPlatformLadderUp,
      FgElementPlatformStep1,
      FgElementPlatformStep2,
      FgElementPlatformStep3,
      FgElementPlatformStep4,      // 10
      FgElementPlatformStep5,
      FgElementPlatformStep6,
      FgElementPlatformStep7,
      FgElementPlatformStep8,
      FgElementLadderMiddle,       //     used for ladder redraw but not in level
      FgElementPlatformStrip,
      FgElementLadder2,            //     used for ladder redraw but not in level
      FgElementDoor0,              // 20
      FgElementDoor1,              // used in door animation, but not in level
      FgElementDoor2,              // used in door animation, but not in level
      FgElementDoor3               // used in door animation, but not in level
    };

    enum {
      DominoTypeEmpty,
      DominoTypeStandard,
      DominoTypeStopper,
      DominoTypeSplitter,
      DominoTypeExploder,
      DominoTypeDelay,
      DominoTypeTumbler,
      DominoTypeBridger,
      DominoTypeVanish,
      DominoTypeTrigger,
      DominoTypeAscender,
      DominoTypeConnectedA,
      DominoTypeConnectedB,
      DominoTypeCounter1,
      DominoTypeCounter2,
      DominoTypeCounter3,
      DominoTypeCrash0,         // all yellow big pile
      DominoTypeCrash1,         // mixed big pile
      DominoTypeCrash2,         // all red big pile
      DominoTypeCrash3,         // all yellow little pile
      DominoTypeCrash4,         // mixed little pile
      DominoTypeCrash5,         // all red little pile
      DominoTypeRiserCont,
      DominoTypeQuaver,
    };

    unsigned short getBg(unsigned int x, unsigned int y, int layer) const;
    unsigned char getFg(unsigned int x, unsigned int y) const;

    unsigned char getDominoType(unsigned int x, unsigned int y) const { return level[2*y][x].dominoType; }
    unsigned char getDominoState(unsigned int x, unsigned int y) const { return level[2*y][x].dominoState; }
    signed char   getDominoDir(unsigned int x, unsigned int y) const { return level[2*y][x].dominoDir; }
    unsigned char getDominoExtra(unsigned int x, unsigned int y) const { return level[2*y][x].dominoExtra; }
    signed char getDominoYOffset(unsigned int x, unsigned int y) const { return level[2*y][x].dominoYOffset; }

    void setBg(unsigned int x, unsigned int y, int layer, int val);
    void setFg(unsigned int x, unsigned int y, int val);
    void setDominoType(unsigned int x, unsigned int y, int val) { level[2*y][x].dominoType = val; }
    void setDominoState(unsigned int x, unsigned int y, int val) { level[2*y][x].dominoState = val; }
    void setDominoDir(unsigned int x, unsigned int y, int val) { level[2*y][x].dominoDir = val; }
    void setDominoExtra(unsigned int x, unsigned int y, int val) { level[2*y][x].dominoExtra = val; }
    void setDominoYOffset(unsigned int x, unsigned int y, int val) { level[2*y][x].dominoYOffset = val; }

    void removeDomino(int x, int y);
    void clearDominoExtra(int x, int y) { level[2*y][x].dominoExtra = 0; }

    unsigned char getEntryDoor(void) const { return FgElementDoor0+doorEntryState; }
    unsigned char getExitDoor(void) const { return FgElementDoor0+doorExitState; }
    void openEntryDoorStep(void) { doorEntryState++; }
    void closeEntryDoorStep(void) { doorEntryState--; }
    void openExitDoorStep(void) { doorExitState++; }
    void closeExitDoorStep(void) { doorExitState--; }

    unsigned char getEntryX(void) const { return doorEntryX; }
    unsigned char getEntryY(void) const { return (doorEntryY-1)/2; }
    unsigned char getExitX(void) const { return doorExitX; }
    unsigned char getExitY(void) const { return (doorExitY-1)/2; }

    bool isEntryDoorOpen(void) { return doorEntryState == 3; }
    bool isExitDoorOpen(void) { return doorExitState == 3; }
    bool isExitDoorClosed(void) { return doorExitState == 0; }

    // query level information of certain places
    bool noGround(int x, int y, bool onLadder);  // returns true, if the ant can't stand
    bool isTherePlatform(int x, int y);

    void print(void);

    bool triggerIsFalln(void) const { return triggerFalln; }
    void setTriggerFalln(void) { triggerFalln = true; }
    void resetTriggerFalln(void) { triggerFalln = false; }

    // check, if the level has been successfully solved
    // if not the reason for failure is in fail
    bool levelCompleted(int & fail) const;

    // NEW INTERFACE please use this only

};


#endif

