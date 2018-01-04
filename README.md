# QvtkDicomReader

使用Qt和VTK编写的Dicom阅读器
===========================

#### Coming Soon
1.显示VTK缩略图,并能从缩略图中直接引导渲染器渲染对应的Series或者Image<br>
2.在renderwindow中显示4份图像<br>
3.在绘图区域中显示影像扫描时的人体方位<br>
4.添加面积测量工具<br>
5.调整依赖库的组织形式<br>
6.绘图区域中的文字添加显示/隐藏开关<br>
7.程序外观的调整<br>
8.依赖要优化一下,现在的debug依赖到了Examples上,重新编译的itk至关重要<br>

#### 已知BUG
请看Docs/BUGs.md<br>

#### 编程参考
编程中参考的软件:<br>
1.[Pmsdview](http://pmsdview-12.updatestar.com/)<br/>
2.[RadiAnt](https://www.radiantviewer.com/)<br/>
Pmsdview是由飞利浦开发的一个程序;RadiAnt是效率极高且功能完善的Dicom浏览器和PACS客户端<br>
这两个程序都是不开源的,本程序的编写中只是使用了这两个软件并借鉴了界面和一些功能.<br>

#### Features:
1.DCMTK-64读取DICOMDIR数据库并构造四层类结构存储结构<br>
2.用树视图显示数据库的结构<br>
3.移除了ITK的全部依赖<br>
4.DCMTK和VTK的依赖经过了整理<br>
5.控件的添加方式已经明确<br>
6.更新部分界面<br>
7.增加窗宽窗位功能,共有8种窗宽窗位模式可以选择<br>
8.添加三维重建功能,入口在:文件树->series节点的右键菜单<br>
9.添加图像分割功能,入口在:文件树->Image节点的右键菜单<br>
10.添加图像配准功能<br>
11.测试数据增加<br>
12.添加打开单张图片和打开一个series目录的功能<br>
13.update vtkMyDICOMImageViewer,现在能够识别一个文件是不是合法的DICOM图片文件<br>

#### Build:
1.依赖:DCMTK3.6.2,64位;vtk8.0.0,64位;Qt5.9.1,64位;itk4.12,64位 <br>
2.依赖包放在解决方案文件夹的Libraries中<br>
3.[依赖下载地址](https://pan.baidu.com/s/1o8dyOFw) 密码：4c9l<br>
4.环境:Visual Studio 2017,v141工具集<br>
5.若运行时缺少DLL,请检查相关的环境变量是否正确,或者找到这个dll并放在解决方案文件夹的x64/debug或x64/release文件夹中<br>
6.[测试数据](https://pan.baidu.com/s/1i5xROTB) 密码：qepn<br>
