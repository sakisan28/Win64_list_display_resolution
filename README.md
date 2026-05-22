# Win64_list_display_resolution
Windows x64 環境で、設定可能なディスプレイ解像度を列挙するアプリケーション

将来的には変更も

### ビルド
Visual Studio Community Edition の C++ が必要です

cmd で
```
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl list_resolution.cpp User32.lib
```

### 出力
```
Enumerating available display resolutions...
Width x Height   Refresh Rate    Color Depth
---------------------------------------------------
640 x 480        60 Hz   32 bit
800 x 600        60 Hz   32 bit
1024 x 768       60 Hz   32 bit
1152 x 864       60 Hz   32 bit
1280 x 720       60 Hz   32 bit
1280 x 768       60 Hz   32 bit
1280 x 1024      60 Hz   32 bit
1360 x 768       60 Hz   32 bit
1400 x 1050      60 Hz   32 bit
1600 x 900       60 Hz   32 bit
1600 x 1200      60 Hz   32 bit
1680 x 1050      60 Hz   32 bit
1920 x 1080      60 Hz   32 bit
2048 x 1536      60 Hz   32 bit
3200 x 1800      60 Hz   32 bit
3840 x 1620      60 Hz   32 bit
3840 x 2160      60 Hz   32 bit
---------------------------------------------------
Enumeration complete.

```
