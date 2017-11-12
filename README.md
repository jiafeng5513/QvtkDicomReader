# QvtkDicomReader

使用Qt和VTK编写的Dicom阅读器
===========================

#### Coming Soon
<br>1.显示VTK缩略图,并能从缩略图中直接引导渲染器渲染对应的Series或者Image
<br>2.在renderwindow中显示4份图像
<br>3.在绘图区域中显示影像扫描时的人体方位
<br>4.添加面积测量工具
<br>5.常见组织器官的窗宽窗位
<br>6.绘图区域中的文字添加显示/隐藏开关

#### 已知BUG
<br>1.负片功能不好使
<br>2.自动播放功能由于涉及到对界面的操作,播放不流畅且会阻塞界面导致停止按钮不太好使
<br>3.当读取到有问题的DICOM图片时会出现VTK运行时错误窗口,但是并不会导致程序崩溃,任然可以继续正常使用
<br>4.数据库里面有一些字段的值是空的,考虑给数据那边增加日志输出,并通过DCMTK的日志确定这些字段的确切位置

#### 编程参考
<br>编程中参考的软件:
<br>1.Pmsdview, http://pmsdview-12.updatestar.com/
<br>2.RadiAnt, https://www.radiantviewer.com/
<br>Pmsdview是由飞利浦开发的一个程序;RadiAnt是效率极高且功能完善的Dicom浏览器和PACS客户端
<br>这两个程序都是不开源的,本程序的编写中只是使用了这两个软件并借鉴了界面和一些功能.

#### Features:
<br>1.DCMTK-64读取DICOMDIR数据库并构造四层类结构存储结构
<br>2.用树视图显示数据库的结构
<br>3.移除了ITK的全部依赖
<br>4.DCMTK和VTK的依赖经过了整理
<br>5.控件的添加方式已经明确
<br>6.更新部分界面

#### Build:
<br>1.依赖:DCMTK3.6.2,64位;vtk8.0.0,64位;Qt5.9.1,64位
<br>2.依赖包放在解决方案文件夹的Libraries中
<br>3.依赖下载地址:链接：http://pan.baidu.com/s/1nuDGbKP 密码：nwzp
<br>3.环境:Visual Studio 2017,v141工具集
