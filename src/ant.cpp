#include "ant.h"
#include "graphics.h"
#include "level.h"

typedef enum {
  AntAnimWalkLeft,
  AntAnimWalkRight,
  AntAnimJunpUpLeft,
  AntAnimJunpUpRight,
  AntAnimJunpDownLeft,
  AntAnimJunpDownRight,
  AntAnimLadder1,
  AntAnimLadder2,
  AntAnimLadder3,
  AntAnimLadder4,
  AntAnimCarryLeft,
  AntAnimCarryRight,
  AntAnimCarryUpLeft,
  AntAnimCarryUpRight,
  AntAnimCarryDownLeft,
  AntAnimCarryDownRight,
  AntAnimCarryLadder1,
  AntAnimCarryLadder2,
  AntAnimCarryLadder3,
  AntAnimCarryLadder4,
  AntAnimCarryStopLeft,
  AntAnimCarryStopRight,
  AntAnimPullOutLeft,
  AntAnimPullOutRight,
  AntAnimPushInLeft,
  AntAnimPushInRight,
  AntAnimXXX1,
  AntAnimXXX2,
  AntAnimXXX3,
  AntAnimXXX4,
  AntAnimLoosingDominoRight,
  AntAnimLoosingDominoLeft,
  AntAnimXXX7,
  AntAnimStop,
  AntAnimTapping,
  AntAnimYawning,
  AntAnimEnterLeft,
  AntAnimEnterRight,
  AntAnimPushLeft,
  AntAnimPushRight,
  AntAnimPushDelayLeft,
  AntAnimPushDelayRight,
  AntAnimPushRiserLeft,
  AntAnimPushRiserRight,
  AntAnimXXX5,
  AntAnimXXX6,
  AntAnimSuddenFallRight,
  AntAnimSuddenFallLeft,
  AntAnimFalling,
  AntAnimInFrontOfExploder,
  AntAnimInFrontOfExploderWait,
  AntAnimLanding,
  AntAnimGhost1,
  AntAnimGhost2,
  AntAnimLeaveDoorEnterLevel,
  AntAnimStepAsideAfterEnter,
  AntAnimEnterDoor,
  AntAnimXXX9,
  AntAnimStruggingAgainsFallLeft,
  AntAnimStruggingAgainsFallRight,
  AntAnimVictory,
  AntAnimShrugging,
  AntAnimNoNo,
  AntAnimXXXA,
  AntAnimDominoDying,
  AntAnimLandDying,
  AntAnimNothing

} AntAnimationState;



// init the ant state for level entering
// the level is saved and used later on for dirty block
// marking, and level modification
void ant_c::init(level_c * l, graphics_c * graph) {

  // ant invisible, outside the screen
  blockX = blockY = 200;
  subBlock = screenBlock = 0;

  state = animation = AntAnimLeaveDoorEnterLevel;
  animationImage = 0;
  animationTimer = 0;
  carriedDomino = 0;
  inactiveTimer = 0;
  numPushsLeft = 1;
  fallingHight = 0;

  level = l;
  gr = graph;
}

void ant_c::draw(SDL_Surface * video) {

  SDL_Rect dst;

  dst.x = (blockX-2)*gr->blockX();
  dst.y = (blockY)*gr->blockY()-gr->getAnt(animation, animationImage)->h+screenBlock + gr->antDisplace();
  dst.w = gr->blockX();
  dst.h = gr->blockY();
  SDL_BlitSurface(gr->getAnt(animation, animationImage), 0, video, &dst);
}

void ant_c::setKeyStates(unsigned int km) {
  keyMask = km;
}

// do one animation step for the ant
void ant_c::performAnimation(void) {

  if (state == AntAnimNothing) {
    state = callStateFunction(state);
  }

  if (state != AntAnimNothing) {
    state = callStateFunction(state);
  }

  // TODO: we need to check for changes and create dirty blocks
  //
  // for the moment a simple version that marks some blocks aroud the ant
  level->markDirty(blockX, blockY);
  level->markDirty(blockX-1, blockY);
  level->markDirty(blockX+1, blockY);

  level->markDirty(blockX, blockY+1);
  level->markDirty(blockX, blockY-1);

  level->markDirty(blockX-1, blockY-1);
  level->markDirty(blockX-1, blockY+1);
  level->markDirty(blockX+1, blockY-1);
  level->markDirty(blockX+1, blockY+1);
}

unsigned int ant_c::callStateFunction(unsigned int state) {

  switch (state) {

    // 0
    case AntAnimWalkLeft:                  return SFWalkLeft();
    case AntAnimWalkRight:                 return SFWalkRight();
    case AntAnimJunpUpLeft:                return SFJumpUpLeft();
    case AntAnimJunpUpRight:               return SFJumpUpRight();
    case AntAnimJunpDownLeft:              return SFJumpDownLeft();
    case AntAnimJunpDownRight:             return SFJumpDownRight();
    case AntAnimLadder1:                   return SFLadder1();
    case AntAnimLadder2:                   return SFLadder2();
    case AntAnimLadder3:                   return SFLadder3();
    case AntAnimLadder4:                   return SFLadder3();
    case AntAnimCarryLeft:                 return SFWalkLeft();
    case AntAnimCarryRight:                return SFWalkRight();
    // 12
    case AntAnimCarryUpLeft:               return SFJumpUpLeft();
    case AntAnimCarryUpRight:              return SFJumpUpRight();
    case AntAnimCarryDownLeft:             return SFJumpDownLeft();
    case AntAnimCarryDownRight:            return SFJumpDownRight();
    case AntAnimCarryLadder1:              return SFLadder1();
    case AntAnimCarryLadder2:              return SFLadder2();
    case AntAnimCarryLadder3:              return SFLadder3();
    case AntAnimCarryLadder4:              return SFLadder3();
    case AntAnimCarryStopLeft:             return SFInactive();
    case AntAnimCarryStopRight:            return SFInactive();
    case AntAnimPullOutLeft:               return SFPullOutLeft();
    case AntAnimPullOutRight:              return SFPullOutRight();
    // 24
    case AntAnimPushInLeft:                return SFPushInLeft();
    case AntAnimPushInRight:               return SFPushInRight();
    case AntAnimXXX1:                      return SFEnterLadder();
    case AntAnimXXX2:                      return SFEnterLadder();
    case AntAnimXXX3:                      return SFLeaveLadderRight();
    case AntAnimXXX4:                      return SFLeaveLadderLeft();
    case AntAnimLoosingDominoRight:        return SFLooseRight();
    case AntAnimLoosingDominoLeft:         return SFLooseLeft();
    case AntAnimXXX7:                      return SFXXX7();
    case AntAnimStop:                      return SFInactive();
    case AntAnimTapping:                   return SFLazying();
    case AntAnimYawning:                   return SFLazying();
    // 36
    case AntAnimEnterLeft:                 return SFEnterDominosLeft();
    case AntAnimEnterRight:                return SFEnterDominosRight();
    case AntAnimPushLeft:                  return SFPushLeft();
    case AntAnimPushRight:                 return SFPushRight();
    case AntAnimPushDelayLeft:             return SFPushSpecialLeft();
    case AntAnimPushDelayRight:            return SFPushSpecialRight();
    case AntAnimPushRiserLeft:             return SFPushSpecialLeft();
    case AntAnimPushRiserRight:            return SFPushSpecialRight();
    case AntAnimXXX5:                      return SFXXX5();
    case AntAnimXXX6:                      return SFXXX6();
    case AntAnimSuddenFallRight:           return SFStartFallingRight();
    case AntAnimSuddenFallLeft:            return SFStartFallingLeft();
    // 48
    case AntAnimFalling:                   return SFFalling();
    case AntAnimInFrontOfExploder:         return SFInFrontOfExploder();
    case AntAnimInFrontOfExploderWait:     return SFInactive();
    case AntAnimLanding:                   return SFLanding();
    case AntAnimGhost1:                    return SFGhost1();
    case AntAnimGhost2:                    return SFGhost2();
    case AntAnimLeaveDoorEnterLevel:       return SFLeaveDoor();
    case AntAnimStepAsideAfterEnter:       return SFStepAside();
    case AntAnimEnterDoor:                 return SFEnterDoor();
    case AntAnimXXX9:                      return SFXXX9();
    case AntAnimStruggingAgainsFallLeft:   return SFFlailing();
    case AntAnimStruggingAgainsFallRight:  return SFFlailing();
    // 60
    case AntAnimVictory:                   return SFVictory();
    case AntAnimShrugging:                 return SFShrugging();
    case AntAnimNoNo:                      return SFNoNo();
    case AntAnimXXXA:                      return SFNextAction();
    case AntAnimDominoDying:               return SFStruck();
    case AntAnimLandDying:                 return SFGhost2();
    case AntAnimNothing:                   return SFNextAction();
    default:                               return AntAnimNothing;
  }
}

bool ant_c::animateAnt(unsigned int delay) {

  if (animationTimer > 0) {
    animationTimer--;
    return false;
  }

  animationImage++;
  if (animationImage >= gr->getAntImages(animation)) {
    animationImage = 0;
    screenBlock = subBlock;
    animationTimer = 0;
    return true;
  }

  screenBlock = subBlock + gr->getAntOffset(animation, animationImage);
  animationTimer = delay;

  return false;
}

unsigned int ant_c::SFStruck(void) {
  if (animateAnt(2))
    ;

  return animation;
}

unsigned int ant_c::SFNoNo(void) {
  if (animateAnt(3))
    return AntAnimNothing;

  return animation;
}

unsigned int ant_c::SFShrugging(void) {
  if (animateAnt(3))
    return AntAnimNothing;

  return animation;
}

unsigned int ant_c::SFVictory(void) {
  if (animateAnt(3))
    return AntAnimNothing;

  return animation;
}

unsigned int ant_c::SFXXX9(void) {
  if (animateAnt(0)) {
    animation = AntAnimEnterDoor;
    blockX--;
  }

  return animation;
}

unsigned int ant_c::SFEnterDoor(void) {

  if (!animateAnt(0)) {
    blockX = 200;
    blockY = 200;

    level->openExitDoor(false);
  }

  return animation;
}

unsigned int ant_c::SFGhost1(void) {
  if (animateAnt(0))
    animation = AntAnimLandDying;

  return animation;
}

unsigned int ant_c::SFGhost2(void) {

  if (animateAnt(2))
    animationImage = 12;

  return animation;
}

unsigned int ant_c::SFXXX5(void) {
  if (animateAnt(5) || level->pushDomino(blockX-1, blockY, -1)) {
    animationImage = 9;
    animation = AntAnimPushLeft;
  }

  return animation;
}

unsigned int ant_c::SFXXX6(void) {
  if (animateAnt(5) || level->pushDomino(blockX+1, blockY, -1)) {
    animationImage = 9;
    animation = AntAnimPushRight;
  }

  return animation;
}

unsigned int ant_c::SFPushSpecialLeft(void) {
  if (animateAnt(5)) {
    animationImage = 9;
    animation = AntAnimPushLeft;
  }

  return animation;
}

unsigned int ant_c::SFPushSpecialRight(void) {
  if (animateAnt(5)) {
    animationImage = 9;
    animation = AntAnimPushRight;
  }

  return animation;
}

unsigned int ant_c::SFPushLeft(void) {
  if (animationImage == 1) {
    if (level->pushDomino(blockX-1, blockY, -1)) {
      if (pushDelay == 0) {

        switch(level->getDominoType(blockX, blockY)) {
          case level_c::DominoTypeBlocker:
            pushDelay = 5;
            pushAnimation = AntAnimPushDelayLeft;
            break;

          case level_c::DominoTypeExploder:
            animationImage = 9;
            break;

          case level_c::DominoTypeDelay:
            pushDelay = 5;
            pushAnimation = AntAnimXXX5;
            break;

          case level_c::DominoTypeRiser:
            pushDelay = 2;
            pushAnimation = AntAnimPushRiserLeft;
            break;
        }

      }
    }
  }

  if (pushDelay) {
    pushDelay--;
    if (pushDelay == 0)
      animation = pushAnimation;

  } else {
    if (animateAnt(0)) {
      blockX--;
      animation = AntAnimStop;
      return AntAnimNothing;
    }
  }

  return animation;
}

unsigned int ant_c::SFPushRight(void) {
  if (animationImage == 1) {
    if (level->pushDomino(blockX+1, blockY, 1)) {
      if (pushDelay == 0) {

        switch(level->getDominoType(blockX+1, blockY)) {
          case level_c::DominoTypeBlocker:
            pushDelay = 5;
            pushAnimation = AntAnimPushDelayRight;
            break;

          case level_c::DominoTypeExploder:
            animationImage = 9;
            break;

          case level_c::DominoTypeDelay:
            pushDelay = 5;
            pushAnimation = AntAnimXXX6;
            break;

          case level_c::DominoTypeRiser:
            pushDelay = 2;
            pushAnimation = AntAnimPushRiserRight;
            break;
        }

      }
    }
  }

  if (pushDelay) {
    pushDelay--;
    if (pushDelay == 0)
      animation = pushAnimation;

  } else {
    if (animateAnt(0)) {
      blockX++;
      animation = AntAnimStop;
      return AntAnimNothing;
    }
  }

  return animation;
}

unsigned int ant_c::SFEnterDominosLeft(void) {
  if (!animateAnt(0))
    return animation;

  animation = AntAnimPushLeft;

  // TODO: this is really mysterious why that strange animation???
  // we her animation 36 here....
  screenBlock = subBlock + gr->getAntOffset(38, animationImage);

  return AntAnimNothing;
}

unsigned int ant_c::SFEnterDominosRight(void) {
  if (!animateAnt(0))
    return animation;

  animation = AntAnimPushRight;

  // TODO: this is really mysterious why that strange animation???
  // we her animation 37 here....
  screenBlock = subBlock + gr->getAntOffset(39, animationImage);

  return AntAnimNothing;
}

unsigned int ant_c::SFXXX7(void) {
  if (animateAnt(0))
    animation = AntAnimLoosingDominoRight;
  return animation;
}

unsigned int ant_c::SFLooseRight(void) {
  if (animateAnt(0)) {
    animation = AntAnimFalling;
    blockY++;
    if (blockY > 12) blockY = 12;
    fallingHight++;
    return AntAnimNothing;
  }

  if (animationImage == 4) {
    level->putDownDomino(blockX+1, blockY, carriedDomino);
    level->fallingDomino(blockX+1, blockY);
    carriedDomino = 0;
  } else if (animationImage == 6) {
    blockX++;
  }

  return animation;
}

unsigned int ant_c::SFLooseLeft(void) {
  if (animateAnt(0)) {
    animation = AntAnimFalling;
    blockY++;
    if (blockY > 12) blockY = 12;
    fallingHight++;
    return AntAnimNothing;
  }

  if (animationImage == 4) {
    blockX--;
  } else if (animationImage == 6) {
    level->putDownDomino(blockX, blockY, carriedDomino);
    level->fallingDomino(blockX, blockY);
    carriedDomino = 0;
  }

  return animation;
}

unsigned int ant_c::SFLeaveLadderRight(void) {

  if (!animateAnt(0))
    return animation;

  animation = AntAnimCarryRight;
  return AntAnimNothing;

}

unsigned int ant_c::SFLeaveLadderLeft(void) {

  if (!animateAnt(0))
    return animation;

  animation = AntAnimCarryLeft;
  return AntAnimNothing;
}

unsigned int ant_c::SFEnterLadder(void) {

  if (!animateAnt(0))
    return animation;

  if (direction == -20) {
    animation = AntAnimCarryLadder1;

  } else {
    animation = AntAnimCarryLadder4;
  }

  return animation;
}

unsigned int ant_c::SFPushInLeft(void) {

  if (!animateAnt(0))
    return animation;

  blockX--;
  animation = AntAnimWalkLeft;
  level->putDownDomino(blockX, blockY, carriedDomino);
  carriedDomino = 0;

  return AntAnimNothing;
}

unsigned int ant_c::SFPushInRight(void) {
  if (!animateAnt(0))
    return animation;

  blockX++;
  animation = AntAnimWalkRight;
  level->putDownDomino(blockX, blockY, carriedDomino);
  carriedDomino = 0;

  return AntAnimNothing;
}

unsigned int ant_c::SFPullOutLeft(void) {

  if (!animateAnt(0))
    return animation;

  animation = AntAnimCarryLeft;
  return AntAnimNothing;
}

unsigned int ant_c::SFPullOutRight(void) {

  if (!animateAnt(0))
    return animation;

  animation = AntAnimCarryRight;
  return AntAnimNothing;

}

unsigned int ant_c::SFLadder1(void) {

  if (!animateAnt(0))
    return animation;

  blockY--;
  return AntAnimNothing;
}

unsigned int ant_c::SFLadder2(void) {

  if (!animateAnt(0))
    return animation;

  if (carriedDomino)
  {
    animation = AntAnimCarryLadder4;
  } else {
    animation = AntAnimLadder4;
  }

  blockY--;
  return AntAnimNothing;
}

unsigned int ant_c::SFLadder3(void) {

  if (!animateAnt(0))
    return animation;

  blockY++;
  return AntAnimNothing;
}

unsigned int ant_c::SFWalkLeft(void) {

  if (animateAnt(0)) {
    blockX--;
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFWalkRight(void) {

  if (animateAnt(0)) {
    blockX++;
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFJumpUpLeft(void) {
  if (!animateAnt(0)) {
    return animation;
  }

  if (carriedDomino) {
    animation = AntAnimCarryStopLeft;
  } else {
    animation = AntAnimWalkLeft;
  }

  blockX--;

  if (screenBlock == 0) {
    blockY--;
    screenBlock = subBlock = gr->halveBlockDisplace();
  } else {
    screenBlock = subBlock = 0;
  }

  return AntAnimNothing;
}

unsigned int ant_c::SFJumpUpRight(void) {
  if (!animateAnt(0)) {
    return animation;
  }

  if (carriedDomino) {
    animation = AntAnimCarryStopRight;
  } else {
    animation = AntAnimWalkRight;
  }

  blockX++;

  if (screenBlock == 0) {
    blockY--;
    screenBlock = subBlock = gr->halveBlockDisplace();
  } else {
    screenBlock = subBlock = 0;
  }

  return AntAnimNothing;
}

unsigned int ant_c::SFJumpDownLeft(void) {
  if (!animateAnt(0)) {
    return animation;
  }

  if (carriedDomino) {
    animation = AntAnimCarryStopLeft;
  } else {
    animation = AntAnimWalkLeft;
  }

  blockX--;

  if (screenBlock == 0) {
    screenBlock = subBlock = gr->halveBlockDisplace();
  } else {
    blockY++;
    screenBlock = subBlock = 0;
  }

  return AntAnimNothing;
}

unsigned int ant_c::SFJumpDownRight(void) {
  if (!animateAnt(0)) {
    return animation;
  }

  if (carriedDomino) {
    animation = AntAnimCarryStopRight;
  } else {
    animation = AntAnimWalkRight;
  }

  blockX++;

  if (screenBlock == 0) {
    screenBlock = subBlock = gr->halveBlockDisplace();
  } else {
    blockY++;
    screenBlock = subBlock = 0;
  }

  return AntAnimNothing;
}

unsigned int ant_c::SFLeaveDoor(void) {
  level->openEntryDoor(true);

  if (!level->isEntryDoorOpen())
    return state;

  if (animationImage == 0) {
    blockX = level->getEntryDoorPosX();
    blockY = level->getEntryDoorPosY()+1;

    screenBlock = subBlock + gr->getAntOffset(animation, 0);
  }

  if (animateAnt(0)) {
    animation = AntAnimStepAsideAfterEnter;
  }

  return animation;
}

unsigned int ant_c::SFStepAside(void) {

  if (animateAnt(0)) {
    animation = AntAnimStop;
    level->openEntryDoor(false);
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFInFrontOfExploder(void) {

  if (animateAnt(3)) {
    animation = AntAnimInFrontOfExploderWait;
  }

  return animation;
}

unsigned int ant_c::SFLazying(void) {

  if (animateAnt(3)) {
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFInactive(void) {
  return AntAnimNothing;
}

unsigned int ant_c::SFFlailing(void) {
  if (animateAnt(0)) {
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFStartFallingLeft(void) {

  if (animateAnt(0)) {
    blockX--;
    animation = AntAnimFalling;
    blockY = blockY+1 < 12 ? blockY+1 : 12;
    fallingHight++;
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFStartFallingRight(void) {

  if (animateAnt(0)) {
    blockX++;
    animation = AntAnimFalling;
    blockY = blockY+1 < 12 ? blockY+1 : 12;
    fallingHight++;
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFFalling(void) {

  if (animateAnt(0)) {
    blockY++;
    fallingHight++;
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::SFLanding(void) {
  if (animateAnt(0)) {
    animation = AntAnimStop;
    return AntAnimNothing;
  }

  return animation;
}

unsigned int ant_c::checkForNoKeyActions(void) {
  return AntAnimStop;
}

static void SaveLevelState(void) {
  // TODO: we need to fill this function
}

bool ant_c::CanPlaceDomino(int x, int y, int ofs) {
  return true;
}

bool ant_c::PushableDomino(int x, int y, int ofs) {
  return true;
}


unsigned int ant_c::SFNextAction(void) {

  animationImage = 0;
  unsigned int returnState;

  bool onLadder = animation >= AntAnimLadder1 && animation <= AntAnimLadder4 ||
                  animation >= AntAnimCarryLadder1 && animation <= AntAnimCarryLadder4;

  if (level->noGround(blockX, blockY, onLadder))
  {
    fallingHight++;
    if (blockY == 13)
    {
      animation = returnState = AntAnimLandDying;
      animationImage = 12;
      return returnState;
    }
    animation = returnState = AntAnimFalling;
    return returnState;
  }
  if (animation == AntAnimPushLeft &&
      animationImage == 0)
  {
    if (level->getDominoType(blockX, blockY) != 0 &&
        level->getDominoState(blockX, blockY) < 8 ||
        level->getDominoType(blockX-1, blockY) != 0 &&
        level->getDominoState(blockX-1, blockY) > 8)
    {
      animation = returnState = AntAnimDominoDying;
      return returnState;
    }
  }
  if (animation == AntAnimPushRight &&
      animationImage == 0)
  {
    if (level->getDominoType(blockX, blockY) != 0 &&
        level->getDominoState(blockX, blockY) > 8 ||
        level->getDominoType(blockX+1, blockY) != 0 &&
        level->getDominoState(blockX+1, blockY) < 8)
    {
      if (blockX < 19)
      {
        blockX++;
        animation = returnState = AntAnimDominoDying;
        return returnState;
      }
      animation = returnState = AntAnimDominoDying;
      return returnState;
    }
  }
  returnState = AntAnimStop;
  if (!(keyMask & KEY_DOWN))
  {
    downChecker = false;
  }
  if (!(keyMask & KEY_UP))
  {
    upChecker = false;
  }
  if (fallingHight != 0)
  {
    if (fallingHight > 3)
    {
      animation = returnState = AntAnimLandDying;
    }
    else
    {
      animation = returnState = AntAnimLanding;
      if (level->getFg(blockX, blockY) != level_c::FgElementPlatformStep2 &&
          level->getFg(blockX, blockY) != level_c::FgElementPlatformStep5)
      {
      }
      else
      {
        subBlock = screenBlock = 8;
      }
    }
  }
  else if (keyMask & KEY_LEFT)
  {
    if (blockX <= 0)
    {
      direction = 1;
    }
    else if (animation == AntAnimPushLeft)
    {
      downChecker = true;
      if (numPushsLeft == 0)
      {
      }
      else if (!(keyMask & KEY_ACTION))
      {
      }
      else
      {
        SaveLevelState();
        numPushsLeft--;
        returnState = AntAnimPushLeft;
        direction = -1;
      }
    }
    else if (animation == AntAnimPushRight)
    {
      downChecker = true;
      if (numPushsLeft && keyMask & KEY_ACTION)
      {
        SaveLevelState();
        blockX++;
        animation = returnState = AntAnimPushLeft;
        if (!PushableDomino(blockX+1, blockY, -1))
        {
          animationImage = 9;
          direction = -1;
        }
        else
        {
          numPushsLeft--;
          direction = -1;
        }
      }
      else if (numPushsLeft == 0)
      {
      }
      else if (!PushableDomino(blockX+1, blockY, -1))
      {
      }
      else
      {
        blockX++;
        animation = AntAnimPushLeft;
        direction = -1;
      }
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformStep2)
    {
      animation = returnState = AntAnimJunpUpLeft;
      direction = -1;
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformStep5)
    {
      animation = returnState = AntAnimJunpDownLeft;
      direction = -1;
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementLadder)
    {
    }
    else if (level->getFg(blockX-1, blockY) == 0 ||
        level->getFg(blockX-1, blockY) == 5)
    {
      if (carriedDomino != 0)
      {
        animation = returnState = AntAnimLoosingDominoLeft;
        direction = -1;
      }
      else if (animation == AntAnimStruggingAgainsFallLeft)
      {
        animation = returnState = AntAnimSuddenFallLeft;
        direction = -1;
      }
      else
      {
        animation = returnState = AntAnimStruggingAgainsFallLeft;
        direction = -1;
      }
    }
    else if (level->getFg(blockX-1, blockY) == 0x0A)
    {
      animation = returnState = AntAnimJunpUpLeft;
      direction = -1;
    }
    else if (level->getFg(blockX-1, blockY) == 0x0B)
    {
      animation = returnState = AntAnimJunpDownLeft;
      direction = -1;
    }
    else if (carriedDomino != 0 &&
        (direction == 20 || direction == -20))
    {
      animation = returnState = AntAnimXXX4;
      direction = -1;
    }
    else if ((keyMask & KEY_UP) && PushableDomino(blockX, blockY, -1))
    {
      animation = returnState = AntAnimEnterLeft;
      direction = -1;
    }
    else
    {
      animation = returnState = 0;
      direction = -1;
    }
  }
  else if (keyMask & KEY_RIGHT)
  {
    if (blockX >= 19)
    {
      direction = -1;
    }
    else if (animation == AntAnimPushRight)
    {
      downChecker = true;
      if (numPushsLeft == 0)
      {
      }
      else if (!(keyMask & KEY_ACTION))
      {
      }
      else
      {
        SaveLevelState();
        numPushsLeft--;
        returnState = AntAnimPushRight;
        direction = 1;
      }
    }
    else if (animation == AntAnimPushLeft)
    {
      downChecker = true;
      if (numPushsLeft != 0 && keyMask & KEY_ACTION)
      {
        SaveLevelState();
        blockX--;
        animation = returnState = AntAnimPushRight;
        if (!PushableDomino(blockX-1, blockY, 1))
        {
          animationImage = 9;
          direction = 1;
        }
        else
        {
          numPushsLeft--;
          direction = 1;
        }
      }
      else if (numPushsLeft == 0)
      {
      }
      else if (!PushableDomino(blockX-1, blockY, 1))
      {
      }
      else
      {
        blockX--;
        animation = AntAnimPushRight;
        direction = 1;
      }
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformStep2)
    {
      animation = returnState = AntAnimJunpDownRight;
      direction = 1;
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformStep6)
    {
      animation = returnState = AntAnimJunpUpRight;
      direction = 1;
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementLadder)
    {
    }
    else if (level->getFg(blockX+1, blockY) == level_c::FgElementEmpty ||
        level->getFg(blockX+1, blockY) == level_c::FgElementLadder)
    {
      if (carriedDomino != 0)
      {
        returnState = AntAnimXXX7;
        animation = AntAnimCarryRight;
        direction = 1;
      }
      else if (animation == AntAnimStruggingAgainsFallRight)
      {
        animation = returnState = AntAnimSuddenFallRight;
        direction = 1;
      }
      else
      {
        animation = returnState = AntAnimStruggingAgainsFallRight;
        direction = 1;
      }
    }
    else if (level->getFg(blockX+1, blockY) == level_c::FgElementPlatformStep2)
    {
      animation = returnState = AntAnimJunpDownRight;
      direction = 1;
    }
    else if (level->getFg(blockX+1, blockY) == level_c::FgElementPlatformStep7)
    {
      animation = returnState = AntAnimJunpUpRight;
      direction = 1;
    }
    else if (carriedDomino != 0 &&
        (direction == 20 || direction == -20))
    {
      animation = returnState = AntAnimXXX3;
      direction = 1;
    }
    else if ((keyMask & KEY_UP) && PushableDomino(blockX, blockY, 1))
    {
      animation = returnState = AntAnimEnterRight;
      direction = 1;
    }
    else
    {
      animation = returnState = AntAnimWalkRight;
      direction = 1;
    }
  }
  else if (keyMask & KEY_UP)
  {
    if ((level->getFg(blockX, blockY-1) == level_c::FgElementDoor3) &&
        ((level->getDominoType(blockX, blockY) == 0 ||
            level->getDominoState(blockX, blockY) > 8) &&
          (level->getDominoType(blockX-1, blockY) == 0) ||
          level->getDominoState(blockX-1, blockY) <= 8)
        )
    {

      if (direction == -1)
      {
        animation = returnState = AntAnimEnterDoor;
        blockX--;
        upChecker = true;
      }
      else
      {
        animation = returnState = AntAnimXXX9;
        upChecker = true;
      }
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformLadderUp)
    {
      if (carriedDomino == 0)
      {
        animation = returnState = AntAnimLadder1;
        direction = -20;
        upChecker = true;
      }
      else if (direction == -1)
      {
        animation = returnState = AntAnimXXX2;
        direction = -20;
        upChecker = true;
      }
      else
      {
        animation = returnState = AntAnimXXX1;
        direction = -20;
        upChecker = true;
      }
    }
    else if (level->getFg(blockX, blockY-1) == level_c::FgElementLadder)
    {
      animation = returnState = AntAnimLadder1;
      direction = -20;
      upChecker = true;
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformLadderDown)
    {
      animation = returnState = AntAnimLadder2;
      direction = -20;
      upChecker = true;
    }
    else if (animation == AntAnimPushLeft
        || animation == AntAnimPushRight)
    {
      returnState = AntAnimNothing;
      upChecker = true;
    }
    else if (carriedDomino != 0)
    {
      returnState = AntAnimStop;
      upChecker = true;
    }
    else if (direction == -1 &&
        PushableDomino(blockX, blockY, -1))
    {
      animation = returnState = AntAnimEnterLeft;
      upChecker = true;
      goto FinalChecks;
    }
    else if (direction == 1 &&
      PushableDomino(blockX, blockY, 1))
    {
      animation = returnState = AntAnimEnterRight;
      upChecker = true;
    }
    else if (!upChecker)
    {
      if (!PushableDomino(blockX, blockY, -1))
      {
        animation = returnState = AntAnimEnterLeft;
        upChecker = true;
      }
      else if (!PushableDomino(blockX, blockY, 1))
      {
        animation = returnState = AntAnimEnterRight;
        upChecker = true;
      }
      else
      {
        upChecker = true;
      }
    }
  }
  else if (keyMask & KEY_DOWN)  //down
  {
    if (animation == AntAnimPushLeft ||
        animation == AntAnimPushRight)
    {
      returnState = animation ;
      animationImage = 9;
      downChecker = true;
    }
    else if (level->getFg(blockX, blockY) == level_c::FgElementPlatformLadderDown)
    {
      if (carriedDomino != 0)
      {
        if (direction == -1)
        {
          animation = returnState = AntAnimXXX2;
          direction = 20;
        }
        else
        {
          animation = returnState = AntAnimXXX1;
          direction = 20;
        }
      }
      else if (downChecker)
      {
      }
      else
      {
        animation = returnState = AntAnimLadder4;
        direction = 20;
      }
    }
    else if (level->getFg(blockX, blockY+1) == level_c::FgElementLadder)
    {
      animation = returnState = AntAnimLadder3;
      direction = 20;
    }
    else if (level->getFg(blockX, blockY+1) == 6 ||
        (level->getFg(blockX, blockY+1) == 4 &&
         level->getFg(blockX, blockY) == 5))

    {
      animation = returnState = AntAnimLadder3;
      direction = 20;
    }
  }
  else if (keyMask & KEY_ACTION)
  {
    if (animation == AntAnimPushRight)
    {
    }
    else if (animation == AntAnimPushLeft)
    {
    }
    else if ((carriedDomino == 0)
        && (level->getDominoType(blockX, blockY) != 0)
        && (level->getDominoState(blockX, blockY) == 8)
        && (level->getDominoType(blockX, blockY) != level_c::DominoTypeRiser || level->getDominoExtra(blockX, blockY) != 0x60)
        && (level->getFg(blockX, blockY) != level_c::FgElementLadder)
        )
    {
      if (level->getDominoType(blockX, blockY) == level_c::DominoTypeTrigger)
      {
        animation = returnState = AntAnimNoNo;
      }
      else
      {
        carriedDomino = level->pickUpDomino(blockX, blockY);

        if (direction == -1)
        {
          animation = returnState = AntAnimPullOutLeft;
          level->removeDomino(blockX, blockY);
        }
        else
        {
          animation = returnState = AntAnimPullOutRight;
          level->removeDomino(blockX, blockY);
        }
      }
    }
    else if (carriedDomino == 0)
    {
    }
    else if (CanPlaceDomino(0, blockX, blockY))
    {
      if (direction == -1)
      {
        blockX++;
        animation = returnState = AntAnimPushInLeft;
        animationImage = 4;
      }
      else
      {
        blockX--;
        animation = returnState = AntAnimPushInRight;
        animationImage = 4;
      }
    }
    else if (!CanPlaceDomino(direction, blockX, blockY))
    {
    }
    else if (direction == -1)
    {
      animation = returnState = AntAnimPushInLeft;
    }
    else if (direction != 1)
    {
    }
    else
    {
      animation = returnState = AntAnimPushInRight;
    }
  }
  else if (direction == -20)
  {
    if (level->getFg(blockX, blockY-1) == 5)
    {
      animation = returnState = AntAnimLadder1;
      direction = -20;
    }
    else if (level->getFg(blockX, blockY-1) != 4)
    {
      direction = -1;
    }
    else
    {
      animation = returnState = AntAnimLadder2;
      direction = -1;
    }
  }
  else if (direction == 20)
  {
    if (level->getFg(blockX, blockY+1) == 5)
    {
      if (level->getFg(blockX, blockY) == 4)
      {
        direction = 20;
      }
      else
      {
        animation = returnState = AntAnimLadder3;
        direction = 20;
      }
    } else if (level->getFg(blockX, blockY+1) == 6 ||
        level->getFg(blockX, blockY+1) == 4 &&
        level->getFg(blockX, blockY) == 5)
    {
      animation = returnState = AntAnimLadder3;
      direction = 20;
    }
  }

FinalChecks:

  if (returnState != AntAnimStop)
  {
    inactiveTimer = 0;
  }
  else if (carriedDomino != 0 || level->getFg(blockX, blockY) == level_c::FgElementLadder || finalAnimationPlayed)
  {
    inactiveTimer++;
    returnState = checkForNoKeyActions();
  }
  else if (false /*TODO LevelCompletedSuccessfully*/)
  {
    animation = returnState = AntAnimVictory;
    finalAnimationPlayed = 1;
  }
  else if (0 /* TODO LevelFailReason*/)
  {
    animation = returnState = AntAnimShrugging;
    finalAnimationPlayed = 1;
  }
  else
  {
    inactiveTimer++;
    returnState = checkForNoKeyActions();
  }

  if (carriedDomino && animation < AntAnimCarryLeft)
  {
    animation += 10;
  }

  fallingHight = 0;
  return returnState;

}

