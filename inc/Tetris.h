#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>

// Horizon
#define SizeX 24
// Vertial
#define SizeY 32

typedef struct {
    short X, Y; 
} TPos;

//! @brief 传出指令，用于更新显示
typedef struct {
    /// 指令条数，[0]是Erase的数量，[1]是Paint的数量
    unsigned short cnt[2]; 
    /// 一次最多清4行，所以是<<2
    TPos Erase[SizeX << 2], Paint[9];
} TCmd;

#ifdef __cplusplus
extern "C" {
#endif

/// @brief 开始新一局
void NewGame(TCmd *Messenger);

/// @brief 向左移动方块
void MoveLeft(TCmd *Messenger);

/// @brief 向右移动方块
void MoveRight(TCmd *Messenger);

/// @brief 到底返回false，开始下一个方块
bool MoveDown(TCmd *Messenger);

/// @brief 旋转俄罗斯方块
void Transform(TCmd *Messenger);

/// @brief 最顶行非空返回true
bool GameOver(void); 

#ifdef __cplusplus
}
#endif

#endif
