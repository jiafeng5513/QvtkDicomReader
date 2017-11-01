# QvtkDicomReader

使用Qt和VTK编写的Dicom阅读器

0.0.6

未实现:



2.利用QT的布局管理器将左右设置为2:8

3.在renderwindow中显示4份图像

4.坐标改为飞利浦的坐标

5.圈定轮廓的功能显示面积

6.常见组织器官的窗宽窗位

7.尽可能添加VTKwidget中的功能

8.添加左键功能，实现dock信息和图片信息连动

9.加一个按键吧左上信息清除

10.少显示左上信息

11.负片功能有bug

12.实现快放慢放功能

13.窗宽窗位，利用render/renderwindow重新绘制

14.显示VTK缩略图

已实现:

1.选择用dcmtk读取病人信息

2.在功能条中设置按键将左边的dockwidget关掉

3.添加播放功能

4.VTK打开图像的功能应该由DCMTK获取的目录来初始化,这样才实现PmsDView中打开病例的效果

5.dcmtk解析dicomdir完成,四层类结构存储,返回绝对路径OK

6.使用VTK单图片播放实现series播放

7.添加树视图
