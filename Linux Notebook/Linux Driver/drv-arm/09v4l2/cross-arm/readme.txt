交叉编译说明
Author: zht
Date: 2016-05-10

针对摄像头设备的例子除了可以在pc上运行，也可以在开发板上运行；
但例子要用到jpeg库，需要下载源码交叉编译；
经过测试，目前的交叉编译器中已经包含了jpeg库(v8版)，可以直接用


1.jpeg库的交叉编译
=======================
(1)解压缩库
----------------------
$>tar xzvf jpegsrc.v9b.tar.gz
解压后的文件在目录jpeg-9b/中


(2)配置和安装
---------------------
需要提前准备好库和头文件的安装目录，配置时最好用绝对路径
$>mkdir jpeg-so/
$>cd jpeg-9b/
$>./configure --prefix=/home/09v4l2/cross-arm/jpeg-so/ 
	CC=arm-linux-gcc --host=arm-linux 
	--enable-static --enable-shared
$>make 
$>make install
//如一切正常，编译完的库和头文件等都安装到cross-arm/jpeg-so/目录下的lib/，include/等子目录


3. 编译并安装应用程序
=======================
应采用静态安装，过程和配置参数可参见Makefile


