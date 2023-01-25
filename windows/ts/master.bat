rd /s /q master
md master
xcopy /y data\anime\dst\*.aaf master\data\anime\dst\
xcopy /y /s /e /i data\graphics\image master\data\graphics\image
xcopy /y /s /e /i data\quest master\data\quest
md master\data\script
for %%i in (data\script\*.teco) do (
	%WINDOWS_ROOT%\bin\Teco.exe %%i master\data\script\%%~ni.scene
)
md master\check
copy /y Release\ts.exe  master\check\“Œ•ûÕ“ËŒ€C93.exe

%WINDOWS_ROOT%\DxLib_VC\Tool\DXArchive\DxaEncode.exe -K:kotomi0715 master\data

move /y master\data.dxa master\check\
xcopy /y /i master\check master\ts
