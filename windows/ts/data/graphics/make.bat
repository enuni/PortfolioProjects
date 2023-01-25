
set WIN_BIN=%WINDOWS_ROOT%\bin
set GRAPHICS=%WINDOWS_ROOT%\ts\data\graphics
set IMAGE_BIN=%WINDOWS_ROOT%\ts\data\graphics\image
set IMAGE_DATA=%WINDOWS_ROOT%\ts\data\src\image_data

%WIN_BIN%\uvd_union_dxlib_130909 -d %GRAPHICS%\uvd -ob %IMAGE_BIN% -oi %IMAGE_DATA% -f main_3d -m
%WIN_BIN%\layout_union_directx -d %GRAPHICS%\lay -o %IMAGE_DATA% -f main_3d -m
call %GRAPHICS%\UvdSpliter.bat

