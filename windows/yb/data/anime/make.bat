
set WIN_BIN=%WINDOWS_ROOT%\bin
set ANIMATION_SRC=.\src
set ANIMATION_DST=.\dst
set IMAGE_DATA=%WINDOWS_ROOT%\yb\data\src\image_data
set SOUND_DATA=%WINDOWS_ROOT%\yb\data\sound

ruby %WIN_BIN%\AnimationConverter.rb -ob %IMAGE_DATA%\obj_main_3d_data.h -bat %ANIMATION_SRC%\animation_data.txt -o %ANIMATION_DST%\anime_data -se %SOUND_DATA%/sound_data.sadl -c

