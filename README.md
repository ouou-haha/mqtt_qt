# mqtt_qt

1.使用前先按照教程编译qt官方mqtt库
https://blog.csdn.net/luoyayun361/article/details/104671603
注意 在其中一步你的情况可能和教程不一样 你的为.a文件 原因是vs编译器和mingw出来的动态库不一样，vscode出来的是 .dll + .lib文件，你用 qt 编译出来的是 .dll + .a 文件，用法同理

测试：
在命令行输入如下命令
1. pip install requests
2. python
3. import requests
4. requests.post("http://123.207.9.26:5000/health", json ={"mBP":"1111", "mBH":"2222"});
   
