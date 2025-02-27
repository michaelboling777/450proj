#include "sprite.hpp"
#include <iostream>

Sprite ::Sprite(const char *filename, SDL_Renderer *rend, int row, int col)
{
    sheetRow = row;
    sheetCol = col;

    spriteSheet = IMG_Load(filename);
    spriteSheetTexture = SDL_CreateTextureFromSurface(rend, spriteSheet);
    SDL_SetSurfaceBlendMode(spriteSheet, SDL_BLENDMODE_BLEND);

    sprite.w = spriteSheet->w / col;
    sprite.h = spriteSheet->h / row;

    SDL_FreeSurface(spriteSheet);
}

Sprite ::~Sprite()
{
    SDL_DestroyTexture(spriteSheetTexture);
}


void Sprite ::selectSprite(int x, int y, int spriteSize)
{
    sprite.x = x * sprite.w;
    sprite.y = y * sprite.h;

    toScreen.x = 0;
    toScreen.y = 0;
    toScreen.w = spriteSize;
    toScreen.h = spriteSize;
}


void Sprite::changeSprite(int x, int y)
{
    sprite.x = x * sprite.w;
    sprite.y = y * sprite.h;
}

void Sprite ::drawSelectedSprite(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, spriteSheetTexture, &sprite, &toScreen);
}

void Sprite ::editToScreen(int x, int y, int size, int size2)
{
    toScreen.x += x;
    toScreen.y += y;
    toScreen.w += size;
    toScreen.h += size2;
}

SDL_Rect Sprite ::accessToScreen()
{
    return toScreen;
}

void Sprite ::spriteGrav()
{
    if (this->yVelocity > 0)
        editToScreen(0, yVelocity, 0, 0);
}

void Sprite ::spriteMove()
{
    const float accelerationRate = 1.1f;
    if (xMaxSpeed > 0)
    {
        if (xVelocity < xMaxSpeed)
        {
            if(xVelocity==0)
                xVelocity+=2;
            xVelocity += accelerationRate;
            editToScreen(xVelocity, 0, 0, 0);
        }
    }

    else
    {
        if (xVelocity > xMaxSpeed)
        {
            if(xVelocity==0)
                xVelocity-=2;
            xVelocity -= accelerationRate;
            editToScreen(xVelocity, 0, 0, 0);
        }
    }

}


void Sprite ::spriteJump()
{
    if (this->jumpVelocity < 0)
        editToScreen(0, jumpVelocity, 0, 0);
}



void Sprite ::spriteRespawn()
{
    toScreen.x = 0;
    toScreen.y = 0;
}



void Sprite ::x_spriteRespawn()
{
    int temp = 0;
    toScreen.y = temp;
    editToScreen(0, toScreen.y, 0, 0);
}   



void Sprite ::setState()
{
    if(xVelocity != 0 && !stateJump)
    {
        stateWalk = true;
        stateIdle = false;
        stateJump = false;
    }
    else if(xVelocity == 0 && !stateJump)
    {
        stateWalk = false;
        stateIdle = true;
        stateJump = false;
    }
    else  if(jumpVelocity != 0)
    {
        stateWalk = false;
        stateIdle = false;
        stateJump = true;
    }
}



void Sprite ::deccelerate()
{
    const float decelerationRate = .1f;

    if (xVelocity > 0)
    {
        xVelocity -= decelerationRate;
        if (xVelocity < 0)
        {
            xVelocity = 0; // Ensure velocity doesn't go negative
        }
        editToScreen(xVelocity, 0, 0, 0);
    }
    else if (xVelocity < 0)
    {
        xVelocity += decelerationRate;
        if (xVelocity > 0)
        {
            xVelocity = 0; // Ensure velocity doesn't go positive
        }
        editToScreen(xVelocity, 0, 0, 0);
    }
    if (jumpVelocity > 0)
    {
        jumpVelocity -= decelerationRate;
    }
}

void Sprite::animate(int idleRow, int walkRow, int jumpRow, int numberOfColumns)
{
    double xVal, yVal;
    if(stateIdle)
    {
        animationCounter++;
        if (animationCounter < 15) 
        {
            // Don't update the animation if the counter hasn't reached the desired speed
            return;
        }

        animationCounter = 0;
        xVal = toScreen.x;
        yVal = toScreen.y;
        selectSprite(idleVal, idleRow, 64);
        editToScreen(xVal, yVal, 0, 0);
        idleVal++;
        if(idleVal == numberOfColumns)
            idleVal = 0;
    }

    if(stateWalk)
    {
        animationCounter++;
        if (animationCounter < 15) 
        {
            // Don't update the animation if the counter hasn't reached the desired speed
            return;
        }

        animationCounter = 0;
        xVal = toScreen.x;
        yVal = toScreen.y;
        selectSprite(idleVal, walkRow, 64);
        editToScreen(xVal, yVal, 0, 0);
        idleVal++;
        if(idleVal == numberOfColumns)
            idleVal = 0;
    }


    if(stateJump)
    {
        animationCounter++;
        if (animationCounter < 15) 
        {
            // Don't update the animation if the counter hasn't reached the desired speed
            return;
        }

        animationCounter = 0;
        xVal = toScreen.x;
        yVal = toScreen.y;
        selectSprite(idleVal, walkRow, 64);
        editToScreen(xVal, yVal, 0, 0);
        idleVal++;
        if(idleVal == numberOfColumns)
            idleVal = 0;
    }
}


void Sprite :: reverseHorizontalMomentum()
{
    editToScreen(-xVelocity, 0, 0, 0);
}

void Sprite :: reverseVerticalMomentum()
{
    editToScreen(0, xMaxSpeed, 0,0);
}

