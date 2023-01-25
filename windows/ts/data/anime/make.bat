
set WIN_BIN=%WINDOWS_ROOT%\bin
set ANIMATION_SRC=.\src
set ANIMATION_DST=.\dst
set IMAGE_DATA=%WINDOWS_ROOT%\ts\data\src\image_data
set SOUND_DATA=%WINDOWS_ROOT%\ts\data\sound

ruby %WIN_BIN%\AnimationConverter.rb -ob %IMAGE_DATA%\obj_main_3d_data.h -bat %ANIMATION_SRC%\anime_data.txt -o %ANIMATION_DST%\anime_data -c

