# 加法準同型暗号デモ
暗号化したまま画像のエッジ検出を行う

# 動作環境
* x64 Windows + Visual Studio
* OpenCV 3.1.0

# ビルド

* OpenCVをインストールする

* ファイルを取得する
```
md work
git clone git@github.com:herumi/xbyak
git clone git@github.com:herumi/cybozulib
git clone git@github.com:herumi/cybozulib_ext
git clone git@github.com:herumi/mcl
git clone git@github.com:herumi/ade_demo
```

`mk.bat`の`OPENCV_INC`と`OPENCV_LIB`を自分がインストールしたディレクトリに変更する。
`ade_demo`ディレクトリに`opencv_world310.dll`をコピーする。

```
cd ahe-demo
mk.bat
```
でserver.exeとmain.exeができる。

# 動作方法

まず`server.exe`を実行する。次に`main.exe`を実行する。

`main.exe`のキー操作
* q : 終了
* t : 通常のエッジ出力（トグルで元に戻る）
* e : その瞬間の画像を暗号化してserverに送る。serverは暗号化された画像からエッジを取得してmainに戻す。mainは復号してエッジを表示して停止する。
* c : 動きを再開する。


# License

modified new BSD License
http://opensource.org/licenses/BSD-3-Clause

# Author

MITSUNARI Shigeo(herumi@nifty.com)
