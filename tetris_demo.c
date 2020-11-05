#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "Tetris.h"
#include "vt100.h"

#define KEY_UP 	  ('w')
#define KEY_LEFT  ('a')
#define KEY_DOWN  ('s')
#define KEY_RIGHT ('d')

static void update(TCmd* cmd, int color) {  // 每次有方块变化都要更新显示
	// 先把原先位置擦除
	vt_set_bg_color(VT_B_WHITE);
	for (int i = 0; i < cmd->cnt[0]; ++i)
		vt_draw_char_at(cmd->Erase[i].Y + 2, cmd->Erase[i].X + 2, ' ');
	fflush(NULL);  // 刷新缓冲区

	// 再画上新的
	vt_set_bg_color(color);
	for (int i = 0; i < cmd->cnt[1]; ++i)
		vt_draw_char_at(cmd->Paint[i].Y + 2, cmd->Paint[i].X + 2, ' ');
	fflush(NULL);
}

static void key_action(const rt_device_t con, TCmd* cmd, int color) {
	char key;  // 从FinSH控制台读一个字符
	if (rt_device_read(con, -1, &key, 1) > 0) {
		switch (key) {
		case KEY_UP:
			Transform(cmd);
			break;
		case KEY_DOWN:
			// MoveDown
			break;
		case KEY_LEFT:
			MoveLeft(cmd);
			break;
		case KEY_RIGHT:
			MoveRight(cmd);
			break;
		default:
			return;  // don't update
		}
		update(cmd, color);
	}
}

static int new_color(void) {
	return rand() % 6 + VT_B_RED;
}

static void init_ui(void) {
	vt_clear();
	vt_set_font_color(VT_F_BLACK);
	vt_set_bg_color(VT_B_WHITE);
	vt_draw_box(1, 1, SizeY + 2, SizeX + 2, '#', '#', '#');
	vt_fill_box(2, 2, SizeY, SizeX, ' ');
	vt_hide_cursor();
}

static void tetris_demo(int argc, char* argv[]) {
	const rt_device_t console = rt_device_find(RT_CONSOLE_DEVICE_NAME); // "uart0"
	const int fall_period_ms = (argc > 1 ? atoi(argv[1]) : 500);  // 多少毫秒下降一格
	int active_color = new_color();
	TCmd tetris_cmd;

	init_ui();
	srand(rt_tick_get());
	NewGame(&tetris_cmd);

	while (1) {
		update(&tetris_cmd, active_color);
		key_action(console, &tetris_cmd, active_color);
		if (!MoveDown(&tetris_cmd))
			if (!MoveDown(&tetris_cmd))
				break;
			else
				active_color = new_color();
		//  if (GameOver()) break;
		rt_thread_mdelay(fall_period_ms);
	}

	vt_clear_attr();
	vt_show_cursor();
	vt_clear();
	fflush(NULL);
}
MSH_CMD_EXPORT(tetris_demo, tetris demo)
