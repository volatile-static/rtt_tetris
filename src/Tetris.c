#include "Tetris.h"
#include <stdlib.h>

#define At(x, y) Tetris[Location.Y + x][Location.X + y]

typedef unsigned short u16;
typedef enum { T, O, I, L, J, Z, S } TFigure;

static TFigure Figure;
static u16 Tetris[SizeY][SizeX], HitBox[4][4], Direction;
static TPos Location;

/// @brief 画点
static void PushP(u16 y, u16 x, TCmd *Messenger) {
    TPos tmp;
    tmp.Y = Location.Y + y;
    tmp.X = Location.X + x;
    Messenger->Paint[Messenger->cnt[1]++] = tmp;
}

/// @brief 擦点
static void PushE(u16 y, u16 x, TCmd *Messenger) {
    TPos tmp;
    tmp.X = Location.X + x;
    tmp.Y = Location.Y + y;
    Messenger->Erase[Messenger->cnt[0]++] = tmp;
}

static void ClearLines(TCmd *Messenger) {
    u16 i, j, flag;
    for (i = 0; i < SizeY; ++i) {
        flag = 1;
        for (j = 0; j < SizeX; ++j)
            if (!Tetris[i][j]) {
                flag = 0;
                break;
            }
        if (flag)
            for (j = 0; j < SizeX; ++j)
                Tetris[i][j] = 0, PushE(i, j, Messenger);
    }
}

static void Create(TCmd *Messenger) {
    u16 i, j;
    Figure = (TFigure)(rand() % 7);
    Direction = 0;
    Location.X = 0;
    Location.Y = -1;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            HitBox[i][j] = 0;
    switch (Figure) {
    case T:
        HitBox[1][0] = HitBox[1][1] = HitBox[1][2] = HitBox[2][1] = 1;
        At(1, 0) = At(1, 1) = At(1, 2) = At(2, 1) = 1;
        PushP(1, 0, Messenger), PushP(1, 1, Messenger), PushP(1, 2, Messenger),
            PushP(2, 1, Messenger);
        break;
    case O:
        HitBox[1][1] = HitBox[1][2] = HitBox[2][1] = HitBox[2][2] = 1;
        At(1, 1) = At(1, 2) = At(2, 1) = At(2, 2) = 1;
        PushP(1, 1, Messenger), PushP(1, 2, Messenger), PushP(2, 1, Messenger),
            PushP(2, 2, Messenger);
        break;
    case I:
        for (i = 0; i < 4; ++i)
            HitBox[1][i] = At(1, i) = 1, PushP(1, i, Messenger);
        break;
    case L:
        HitBox[1][0] = HitBox[1][1] = HitBox[1][2] = HitBox[2][0] = 1;
        At(1, 0) = At(1, 1) = At(1, 2) = At(2, 0) = 1;
        PushP(1, 0, Messenger), PushP(1, 1, Messenger), PushP(1, 2, Messenger),
            PushP(2, 0, Messenger);
        break;
    case J:
        HitBox[1][0] = HitBox[1][1] = HitBox[1][2] = HitBox[2][2] = 1;
        At(1, 0) = At(1, 1) = At(1, 2) = At(2, 2) = 1;
        PushP(1, 0, Messenger), PushP(1, 1, Messenger), PushP(1, 2, Messenger),
            PushP(2, 2, Messenger);
        break;
    case Z:
        HitBox[1][0] = HitBox[1][1] = HitBox[2][1] = HitBox[2][2] = 1;
        At(1, 0) = At(1, 1) = At(2, 1) = At(2, 2) = 1;
        PushP(1, 0, Messenger), PushP(1, 1, Messenger), PushP(2, 1, Messenger),
            PushP(2, 2, Messenger);
        break;
    case S:
        HitBox[1][1] = HitBox[1][2] = HitBox[2][0] = HitBox[2][1] = 1;
        At(1, 1) = At(1, 2) = At(2, 0) = At(2, 1) = 1;
        PushP(1, 1, Messenger), PushP(1, 2, Messenger), PushP(2, 0, Messenger),
            PushP(2, 1, Messenger);
        break;
    }
}

static void TransformT(TCmd *Messenger) {
    switch (Direction) {
    case 0:
        if (Location.Y < 0 || At(0, 1))
            return;
        HitBox[0][1] = !(HitBox[1][2] = 0);
        At(0, 1) = !(At(1, 2) = 0);
        PushE(1, 2, Messenger);
        PushP(0, 1, Messenger);
        break;
    case 1:
        if (SizeX - Location.X < 3 || At(1, 2))
            return;
        HitBox[1][2] = !(HitBox[2][1] = 0);
        At(1, 2) = !(At(2, 1) = 0);
        PushP(1, 2, Messenger);
        PushE(2, 1, Messenger);
        break;
    case 2:
        if (SizeY - Location.Y < 3 || At(2, 1))
            return;
        HitBox[2][1] = !(HitBox[1][0] = 0);
        At(2, 1) = !(At(1, 0) = 0);
        PushE(1, 0, Messenger);
        PushP(2, 1, Messenger);
        break;
    case 3:
        if (Location.X < 0 || At(1, 0))
            return;
        HitBox[1][0] = !(HitBox[0][1] = 0);
        At(1, 0) = !(At(0, 1) = 0);
        PushE(0, 1, Messenger);
        PushP(1, 0, Messenger);
        break;
    default:
        break;
    }
    ++Direction;
    Direction %= 4;
}

static void TransformI(TCmd *Messenger) {
    u16 i;
    if (Direction) {
        if (Location.X < 0 || SizeX - Location.X < 4 || At(1, 0) || At(1, 2) ||
            At(1, 3))
            return;
    } else if (Location.Y < 0 || SizeY - Location.Y < 4 || At(0, 1) ||
               At(2, 1) || At(3, 1))
        return;
    At(1, 0) = HitBox[1][0] = At(1, 2) = HitBox[1][2] = At(1, 3) =
        HitBox[1][3] = Direction;
    At(0, 1) = HitBox[0][1] = At(2, 1) = HitBox[2][1] = At(3, 1) =
        HitBox[3][1] = !Direction;
    for (i = 0; i < 4; ++i)
        if (Direction)
            PushE(i, 1, Messenger), PushP(1, i, Messenger);
        else
            PushE(1, i, Messenger), PushP(i, 1, Messenger);
    ++Direction;
    Direction %= 2;
}

static void TransformL(TCmd *Messenger) {
    switch (Direction) {
    case 0:
        if (Location.Y < 0 || At(0, 1) || At(0, 2) || At(2, 2))
            return;
        HitBox[0][2] = HitBox[2][2] = HitBox[0][1] =
            !(HitBox[1][0] = HitBox[1][1] = HitBox[2][0] = 0);
        At(0, 2) = At(2, 2) = At(0, 1) = !(At(1, 0) = At(1, 1) = At(2, 0) = 0);
        PushE(1, 0, Messenger), PushE(1, 1, Messenger), PushE(2, 0, Messenger);
        PushP(0, 2, Messenger), PushP(2, 2, Messenger), PushP(0, 1, Messenger);
        break;
    case 1:
        if (Location.X < 0 || At(2, 0) || At(2, 1))
            return;
        HitBox[2][0] = HitBox[2][1] = !(HitBox[0][1] = HitBox[0][2] = 0);
        At(2, 0) = At(2, 1) = !(At(0, 1) = At(0, 2) = 0);
        PushP(2, 0, Messenger), PushP(2, 1, Messenger);
        PushE(0, 1, Messenger), PushE(0, 2, Messenger);
        break;
    case 2:
        if (Location.Y < 0 || At(0, 1) || At(1, 1))
            return;
        HitBox[0][1] = HitBox[1][1] = !(HitBox[1][2] = HitBox[2][0] = 0);
        At(0, 1) = At(1, 1) = !(At(1, 2) = At(2, 0) = 0);
        PushE(1, 2, Messenger), PushE(2, 0, Messenger);
        PushP(1, 1, Messenger), PushP(0, 1, Messenger);
        break;
    case 3:
        if (Location.X < 0 || At(1, 0) || At(1, 2) || At(2, 0))
            return;
        HitBox[1][0] = HitBox[1][2] = HitBox[2][0] =
            !(HitBox[2][1] = HitBox[0][1] = HitBox[2][2] = 0);
        At(1, 0) = At(1, 2) = At(2, 0) = !(At(0, 1) = At(2, 1) = At(2, 2) = 0);
        PushE(0, 1, Messenger), PushE(2, 1, Messenger), PushE(2, 2, Messenger);
        PushP(1, 0, Messenger), PushP(1, 2, Messenger), PushP(2, 0, Messenger);
        break;
    default:
        break;
    }
    ++Direction;
    Direction %= 4;
}

static void TransformJ(TCmd *Messenger) {
    switch (Direction) {
    case 0:
        if (Location.Y < 0 || At(0, 2) || At(2, 1))
            return;
        HitBox[0][2] = HitBox[2][1] = !(HitBox[1][0] = HitBox[1][1] = 0);
        At(0, 2) = At(2, 1) = !(At(1, 0) = At(1, 1) = 0);
        PushE(1, 0, Messenger), PushE(1, 1, Messenger);
        PushP(0, 2, Messenger), PushP(2, 1, Messenger);
        break;
    case 1:
        if (Location.Y < 0 || At(2, 0) || At(1, 0))
            return;
        HitBox[2][0] = HitBox[1][0] = !(HitBox[1][2] = HitBox[0][2] = 0);
        At(2, 0) = At(1, 0) = !(At(1, 2) = At(0, 2) = 0);
        PushP(2, 0, Messenger), PushP(1, 0, Messenger);
        PushE(1, 2, Messenger), PushE(0, 2, Messenger);
        break;
    case 2:
        if (Location.Y < 0 || At(0, 1) || At(0, 2) || At(1, 1))
            return;
        HitBox[0][1] = HitBox[1][1] = HitBox[0][2] =
            !(HitBox[2][2] = HitBox[2][0] = HitBox[1][0] = 0);
        At(0, 1) = At(1, 1) = At(0, 2) = !(At(2, 2) = At(2, 0) = At(1, 0) = 0);
        PushE(2, 2, Messenger), PushE(2, 0, Messenger), PushE(1, 0, Messenger);
        PushP(1, 1, Messenger), PushP(0, 2, Messenger), PushP(0, 1, Messenger);
        break;
    case 3:
        if (Location.X < 0 || At(1, 0) || At(1, 2) || At(2, 2))
            return;
        HitBox[1][0] = HitBox[1][2] = HitBox[2][2] =
            !(HitBox[2][1] = HitBox[0][1] = HitBox[0][2] = 0);
        At(1, 0) = At(1, 2) = At(2, 2) = !(At(0, 1) = At(2, 1) = At(0, 2) = 0);
        PushE(0, 1, Messenger), PushE(2, 1, Messenger), PushE(0, 2, Messenger);
        PushP(1, 0, Messenger), PushP(1, 2, Messenger), PushP(2, 2, Messenger);
        break;
    default:
        break;
    }
    ++Direction;
    Direction %= 4;
}

static void TransformZ(TCmd *Messenger) {
    if (Direction) {
        if (Location.X < 0 || At(1, 0) || At(2, 2))
            return;
    } else if (Location.Y < 0 || At(0, 2) || At(1, 2))
        return;
    At(0, 2) = HitBox[0][2] = At(1, 2) = HitBox[1][2] = !Direction;
    At(1, 0) = HitBox[1][0] = At(2, 2) = HitBox[2][2] = Direction;
    if (Direction)
        PushE(0, 2, Messenger), PushE(1, 2, Messenger), PushP(1, 0, Messenger),
            PushP(2, 2, Messenger);
    else
        PushE(1, 0, Messenger), PushE(2, 2, Messenger), PushP(0, 2, Messenger),
            PushP(1, 2, Messenger);
    ++Direction;
    Direction %= 2;
}

static void TransformS(TCmd *Messenger) {
    if (Direction) {
        if (Location.X < 0 || At(2, 0) || At(2, 1))
            return;
    } else if (Location.Y < 0 || At(0, 1) || At(2, 2))
        return;
    At(0, 1) = HitBox[0][1] = At(2, 2) = HitBox[2][2] = !Direction;
    At(2, 0) = HitBox[2][0] = At(2, 1) = HitBox[2][1] = Direction;
    if (Direction)
        PushE(2, 2, Messenger), PushE(0, 1, Messenger), PushP(2, 0, Messenger),
            PushP(2, 1, Messenger);
    else
        PushE(2, 1, Messenger), PushE(2, 0, Messenger), PushP(0, 1, Messenger),
            PushP(2, 2, Messenger);
    ++Direction;
    Direction %= 2;
}

void NewGame(TCmd *Messenger) {
    u16 i, j;
    for (i = 0; i < SizeY; ++i)
        for (j = 0; j < SizeX; ++j)
            Tetris[i][j] = 0;
    Messenger->cnt[0] = Messenger->cnt[1] = 0;
    Create(Messenger);
}

bool GameOver(void) {
    u16 i;
    for (i = 0; i < SizeX; ++i)
        if (Tetris[0][i])
            return true;
    return false;
}

void MoveLeft(TCmd *Messenger) {
    u16 i, j;
    Messenger->cnt[0] = Messenger->cnt[1] = 0;
    if (!Location.X)
        return;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            if (HitBox[i][j]) {
                if (At(i, j - 1))
                    return;
                else
                    break;
            }
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            if (HitBox[i][j]) {
                At(i, j - 1) = !(At(i, j) = 0);
                PushE(i, j, Messenger), PushP(i, j - 1, Messenger);
            }
    --Location.X;
}

void MoveRight(TCmd *Messenger) {
    short i, j;
    Messenger->cnt[0] = Messenger->cnt[1] = 0;
    if (Location.X == SizeX - 3)
        return;
    for (i = 0; i < 4; ++i)
        for (j = 3; j >= 0; --j)
            if (HitBox[i][j]) {
                if (At(i, j + 1))
                    return;
                else
                    break;
            }
    for (i = 0; i < 4; ++i)
        for (j = 3; j >= 0; --j)
            if (HitBox[i][j]) {
                At(i, j + 1) = !(At(i, j) = 0);
                PushE(i, j, Messenger), PushP(i, j + 1, Messenger);
            }
    ++Location.X;
}

bool MoveDown(TCmd *Messenger) {
    short i, j;
    Messenger->cnt[0] = Messenger->cnt[1] = 0;
    for (i = 0; i < 4; ++i)
        for (j = 3; j + 1; --j)
            if (HitBox[j][i]) {
                if (Location.Y + j >= SizeY - 1 || At(j + 1, i)) {
                    ClearLines(Messenger);
                    Create(Messenger);
                    return false;
                } else
                    break;
            }
    for (i = 0; i < 4; ++i)
        for (j = 3; j + 1; --j)
            if (HitBox[j][i]) {
                At(j + 1, i) = !(At(j, i) = 0);
                PushE(j, i, Messenger), PushP(j + 1, i, Messenger);
            }
    ++Location.Y;
    return true;
}

void Transform(TCmd *Messenger) {
    Messenger->cnt[0] = Messenger->cnt[1] = 0;
    switch (Figure) {
    case T:
        TransformT(Messenger);
        break;
    case I:
        TransformI(Messenger);
        break;
    case L:
        TransformL(Messenger);
        break;
    case J:
        TransformJ(Messenger);
        break;
    case Z:
        TransformZ(Messenger);
        break;
    case S:
        TransformS(Messenger);
        break;
    case O:
    default:
        break;
    }
}
