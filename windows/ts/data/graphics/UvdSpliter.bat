for /R "%WINDOWS_ROOT%/ts/data/graphics//uvd/" %%I in (*.uvd) do (
	ruby %WINDOWS_ROOT%/bin/UvdSplitter.rb -f %%I -i OBJ_ID -o %WINDOWS_ROOT%/ts/data/anime/src/split_data
)
