for /R "%WINDOWS_ROOT%/yb/data/graphics//uvd/" %%I in (*.uvd) do (
	ruby %WINDOWS_ROOT%/bin/UvdSplitter.rb -f %%I -i OBJ_ID -o %WINDOWS_ROOT%/yb/data/anime/src/split_data
)
exit 0