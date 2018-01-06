# QvtkDicomReader

使用Qt和VTK编写的Dicom阅读器
===========================

#### Coming Soon
1.显示VTK缩略图,并能从缩略图中直接引导渲染器渲染对应的Series或者Image<br>
2.在renderwindow中显示4份图像<br>
3.在绘图区域中显示影像扫描时的人体方位<br>
4.添加面积测量工具<br>
5.程序外观的调整<br>
6.绘图区域中的文字添加显示/隐藏开关<br>

#### 已知BUG
请看Docs/BUGs.md<br>

#### 编程参考
编程中参考的软件:<br>
1.[Pmsdview](http://pmsdview-12.updatestar.com/)<br/>
2.[RadiAnt](https://www.radiantviewer.com/)<br/>
Pmsdview是由飞利浦开发的一个程序;RadiAnt是效率极高且功能完善的Dicom浏览器和PACS客户端<br>
这两个程序都是不开源的,本程序的编写中只是使用了这两个软件并借鉴了界面和一些功能.<br>

#### Features:
1.修复了若干bug.具体请看Docs/BUGs.md

#### Build:
1.依赖:DCMTK3.6.2,64位;vtk8.0.0,64位;Qt5.9.1,64位;itk4.12,64位 <br>
2.依赖包放在解决方案文件夹的Libraries中<br>
3.最新依赖的更新日期:2018年1月6日10:28:57,该日期记录在依赖包中的文档下,编译之前请核对<br>
3.[依赖下载地址](https://pan.baidu.com/s/1c1ADO3I) 密码：8e2h<br>
4.环境:Visual Studio 2017,v141工具集<br>
5.若运行时缺少DLL,请检查相关的环境变量是否正确,或者找到这个dll并放在解决方案文件夹的x64/debug或x64/release文件夹中<br>
6.[测试数据](https://pan.baidu.com/s/1kURu74b) 密码：zjlg<br>
