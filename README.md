# LittleHttpd



## 基本介绍

LittleHttpd是用底层C/C++实现的玩具级WebServer，以下是其特点：

- 实现了接收并响应Http的GET和简单POST请求
- 单线程
- 没使用任何框架，通过C的socket、字符串处理、文件处理实现



## 思路图

<img src="/Volumes/T11/Sreenshot/Xnip2020-12-14_14-44-43.jpg" alt="Xnip2020-12-14_14-44-43" style="zoom: 40%;" />



## 基本功能

### 	GET

<img src="/Volumes/T11/Sreenshot/Xnip2020-12-14_14-50-52.jpg" alt="Xnip2020-12-14_14-50-52" style="zoom:20%;" />

### 	POST

<img src="/Volumes/T11/Sreenshot/Xnip2020-12-14_14-51-11.jpg" alt="Xnip2020-12-14_14-51-11" style="zoom:20%;" />

<img src="/Volumes/T11/Sreenshot/Xnip2020-12-14_14-51-16.jpg" alt="Xnip2020-12-14_14-51-16" style="zoom:20%;" />



## 文件结构

<img src="/Volumes/T11/Sreenshot/Xnip2020-12-14_15-01-28.jpg" alt="Xnip2020-12-14_15-01-28" style="zoom: 50%;" />



## 踩坑总结

1. 值传递and引用传递（指针）：在写HttpResponse类时，我想让HttpResponse对象能够获取                                                    对象的属性和方法并且能够改变该对象的属性值，在参数传递时，没有想就直接值传递了。导致在改变HttpRequest对象的属性时，只是改变了其副本。所以，传参前，请三思。

2. gcc版本：在本机上运行成功的项目，放到服务器centos上跑不起来。因为Centos默认yum install gcc最高版本是2.5，本机下用的gcc是3.16。解决办法：升级centos下的gcc版本。

3. 读取到GET某文件的请求后，直接返回文件了，没有将http请求后面的header部分读完，导致下次读请求出现错误

   

## TODO

- 多线程、线程池，提高并发量 ==>使用 epoll 作为 IO 多路复用（盲点）
- Timer定时器，处理超时请求
- 日志 and so on



## 参考资料

https://github.com/cbsheng/tinyhttpd

https://github.com/linw7/

https://github.com/rongweihe/WebServer

https://github.com/markparticle/WebServer

https://github.com/qinguoyi/TinyWebServer

https://github.com/waninkoko/micro-httpd

https://blog.csdn.net/programvae/article/details/71036295

https://github.com/IdiotXue/MultiThreadHttpd