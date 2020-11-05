from building import *

cwd     = GetCurrentDir()
src     = Glob('*.c')
CPPPATH = [cwd]

group = DefineGroup('Tetris', src, depend = ['PKG_USING_TETRIS'], CPPPATH = CPPPATH)

Return('group')
