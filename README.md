# Win64_list_display_resolution

An application to enumerate available display resolutions in a Windows x64 environment.

Running without arguments displays a list of resolutions.

Specifying a setting number from the 'Enum' column as an argument changes the resolution to that setting.

By combining it with tools like imniko / SetDPI, you can change the resolution via batch files before and after running games.

Windows x64 環境で、設定可能なディスプレイ解像度を列挙するアプリケーション

引数無しで実行すると、一覧を表示。

引数として Enum にある設定番号を指定すると、その解像度に変更します。

imniko / SetDPI などと組み合わせれば、ゲームの実行前後にバッチで解像度を変更したりできます。

### build

Visual Studio Community Edition with C++ required.

Visual Studio Community Edition の C++ が必要です

cmd
```
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl list_resolution.cpp User32.lib
```

### Output
```
C:\> list_resolution

Enumerating available display resolutions...
Enum     Width x Height          Refresh Rate    Color Depth
---------------------------------------------------
0        640 x 480       59 Hz   32 bit
1        640 x 480       60 Hz   32 bit
2        640 x 480       75 Hz   32 bit
3        720 x 400       60 Hz   32 bit
4        720 x 400       60 Hz   32 bit
5        720 x 400       60 Hz   32 bit
            :
            :
90       3840 x 1620     30 Hz   32 bit
91       3840 x 1620     30 Hz   32 bit
92       3840 x 1620     60 Hz   32 bit
93       3840 x 1620     60 Hz   32 bit
94       3840 x 1620     60 Hz   32 bit
95       3840 x 2160     30 Hz   32 bit
96       3840 x 2160     60 Hz   32 bit
---------------------------------------------------
Enumeration complete. Provide an 'Enum' index as an argument to change resolution.

C:\> list_resolution 96
Attempting to change resolution to: 3840x2160...
Display settings changed successfully.

```
