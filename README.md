# 安装
- 下载 Qt 地址： http://download.qt.io/archive/qt/ (选择 5.9 及以上版本)。
- 选择对应平台的安装包后安装，选择组件中，一定要勾选 MinGW 和 Qt Creator。
- 安装 git，下载地址：https://git-scm.com/ 。
- Windows 下安装 WinRAR 打包工具。

# 编译
- 下载代码 `git clone https://github.com/33cn/dplatform-qt.git`
- 双击打开 dplatform-qt.pro 文件，左下角选择 Release 版本，开始构建项，等待构建完成。
- 在 release 目录下 Windows 会生成 dplatform-qt.exe 程序，Linux 会生成 dplatform-qt 程序，MAC 会生成 dplatform-qt.app 文件包。

# 不同平台打包
## Windows
### 构建 dplatform.exe 程序
- 根据 https://github.com/33cn/dplatform 构建 dplatform.exe 和 dplatform-cli.exe。
- 用 gox.exe (在 \dplatform-qt\src\tool 下) 工具编译 x86 版本的 dplatform.exe 和 dplatform-cli.exe，步骤：
```
    ./gox -osarch="windows/386" -ldflags="-X github.com/33cn/dplatform/common/version.GitCommit=`git rev-parse --short=8 HEAD`" github.com/33cn/dplatform/cmd/dplatform
    
    ./gox -osarch="windows/386" github.com/33cn/dplatform/cmd/cli 
```
- 改名称为 dplatform-x86.exe 和 dplatform-cli-x86.exe。

### 准备
- 安装 WinRAR 工具，打包程序，可参考 [winrar 打包程序为 exe 文件](https://jingyan.baidu.com/article/6fb756ec9a9e09241858fbc1.html)
- 在 dplatform-qt.exe 所在目录下运行命令 `windeployqt.exe btyWallet.exe` 会拷贝所需要的对应 DLL 文件，如果运行时还提示 DLL 文件缺少，可以在 Qt 的安装目录 bin 目录下寻找（例如 C:\Qt\Qt5.10.0\5.10.0\mingw53_32\bin）。
- 同一目录下添加 dplatform.exe、dplatform-cli.exe、dplatform-x86.exe、dplatform-cli-x86.exe、dplatform.toml 这 5 个文件。

### 打包成可执行程序
- 全选所有文件，右击 "添加到压缩文件"；
- 压缩文件名改为：dplatform-qt-Win.exe；
- 点击高级，自解压缩项；
    - 常规，解压路径写为 dplatform-qt，选择在 “Prpgram Files” 中创建；
    - 设置，提取后运行 dplatform-qt.exe；
    - 高级，添加快捷方式；
    - 文本和图标，标题为：chian33-qt 安装，选择图标 novacoin.ico (在 \dplatform-qt\src\res 下)；

## Mac
### 准备 dplatform-qt.app
- 根据 https://github.com/33cn/dplatform 构建 dplatform 和 dplatform-cli。
- 拷贝 dplatform、dplatform-cli、dplatform.toml 这 3 个文件到 dplatform-qt.app/Contents/MacOS/ 目录下。
- 打开终端，到 dplatform-qt.app 所在目录，运行命令：
```
cd .../dplatform-qt/release
cp $GOPATH/src/github.com/33cn/dplatform/build/dplatform /release/dplatform-qt.app/Contents/MacOS/dplatform
cp $GOPATH/src/github.com/33cn/dplatform/build/dplatform-cli /release/dplatform-qt.app/Contents/MacOS/dplatform-cli
cp $GOPATH/src/github.com/33cn/dplatform/build/dplatform.toml /release/dplatform-qt.app/Contents/MacOS/dplatform.toml
otool -L dplatform-qt.app/Contents/MacOS/dplatform-qt 
macdeployqt dplatform-qt.app
```

### 把 dplatform-qt.app 打包成 dmg 文件
- 通过 Disk Utility 创建一个空白的 dmg 镜像, File –> New –> Blank Disk Image
- 默认设置 120MB，名称为 dplatform-qt-Mac，最终制作出来的 dmg 文件会自动裁剪掉空白的。
- 双击 “dplatform-qt-Mac” 出来的空白窗口
- 将上面制作好的 dplatform-qt.app 拽到 “dplatform-qt-Mac” 里面，再做一个 Applications 快捷方式 (右键–>Make Alias)
- 将做好的 dmg，转换一下就完成了，在 Disk Utility 中如果没有的话，请将它拽回去就可以了，然后右键 dplatform-qt-Mac.dmg –> Convert ”dplatform-qt-Mac”
- 可参考文档 [Qt Mac 下软件 Release 发布 dmg](https://blog.csdn.net/fox64194167/article/details/38441331)

## Ubuntu
### 打包 dplatform-qt-x86_64.AppImage
- 新建一个文件夹为 dplatform-qt-Ubuntu，把 dplatform 、dplatform-cli、dplatform-qt、chian33.toml 这 4 个文件，都放在 dplatform-qt-Ubuntu 文件夹下。
- 运行命令 `linuxdeployqt ./dplatform-qt-Ubuntu/dplatform-qt -appimage`
- 改名为 dplatform-qt-x86_64.AppImage。

# 配置文件
* 默认不设置，用程序默认配置，src/config/StyleConfig.ini 有模板。
* 字段解析：
        
| 字段名称 | 默认配置 | 备注 |
|---|---|---|
| DplatformName | dplatform | 底层运行链的名称 |
| AppName | dplatform-qt |  |
| AppName_zh | dplatform-qt |  |
| AppName_en | dplatform-qt |  |
| StyleType | yellow | 只支持两种样式：yellow 和 blue |
| NetworkUrl | http://localhost:8801/ |  |
