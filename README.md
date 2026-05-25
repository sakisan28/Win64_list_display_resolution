# Win64_list_display_resolution

An application to enumerate available display resolutions in a Windows x64 environment.

Running without arguments displays a list of resolutions.

Specifying a setting number from the 'Index' column as an argument changes the resolution to that setting.

By combining it with tools like imniko / SetDPI, you can change the resolution via batch files before and after running games.

Windows x64 環境で、設定可能なディスプレイ解像度を列挙するアプリケーション

引数無しで実行すると、一覧を表示。

引数として Index にある設定番号を指定すると、その解像度に変更します。

imniko / SetDPI などと組み合わせれば、ゲームの実行前後にバッチで解像度を変更したりできます。

### Build

Visual Studio Community Edition with C++ required.

Use build.bat

### Output
```
PS C:\> list_resolution.exe
======================================================================
                 Windows Display Resolution Enumerator
======================================================================

======================================================================
 Monitor: \\.\DISPLAY1 [Primary]
======================================================================
  Virtual Bounds: Left=0, Top=0, Right=1536, Bottom=1024
  Work Area:      Left=0, Top=0, Right=1536, Bottom=976

  Current Active Mode:
    Resolution:   1920 x 1280 (3:2)
    Color Depth:  32 bits per pixel
    Refresh Rate: 60 Hz
    LCD Stretch:  Default

  Supported Graphic Modes:
  -------------------------------------------------------------------------------------------------
  Index  Resolution                 Color Depth  Refresh Rate   LCD Stretch (Fixed Output)
  -------------------------------------------------------------------------------------------------
  0      320 x 200 (16:10)          32 bpp       48 Hz          N/A
  1      320 x 200 (16:10)          32 bpp       48 Hz          Stretch to Fill
  2      320 x 200 (16:10)          32 bpp       48 Hz          Center (No Stretch)
  3      320 x 200 (16:10)          32 bpp       60 Hz          N/A
  4      320 x 200 (16:10)          32 bpp       60 Hz          Stretch to Fill
  5      320 x 200 (16:10)          32 bpp       60 Hz          Center (No Stretch)
             :
             :
  140    1920 x 1200 (16:10)        32 bpp       48 Hz          Center (No Stretch)
  141    1920 x 1200 (16:10)        32 bpp       60 Hz          N/A
  142    1920 x 1200 (16:10)        32 bpp       60 Hz          Stretch to Fill
  143    1920 x 1200 (16:10)        32 bpp       60 Hz          Center (No Stretch)
  144    1920 x 1280 (3:2)          32 bpp       48 Hz          N/A
  145    1920 x 1280 (3:2)          32 bpp       60 Hz          N/A
  -------------------------------------------------------------------------------------------------
  Total modes for this monitor: 146 (Global index limit: 145)

```
