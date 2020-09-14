# 1 skynet介绍

​ Skynet 是一个基于C跟lua的开源服务端并发框架，这个框架是单进程多线程Actor模型。是一个轻量级的为在线游戏服务器打造的框架。

我在云风博客的基础上，把重要的知识点加上具体的example来讲解skynet如何去使用。前面的比较啰嗦一些，大家不想了解这些知识的，可以直接跳转到第二章节。

## 1.1 简介

​ 这个系统是单进程多线程模型。

​ 每个服务都是严格的被动的消息驱动的，以一个统一的 callback 函数的形式交给框架。框架从消息队列里调度出接收的服务模块，找到 callback 函数入口，调用它。服务本身在没有被调度时，是不占用任何 CPU 的。

​ skynet虽然支持集群，但是作者云风主张能用一个节点完成尽量用一个节点，因为多节点通信方面的开销太大，如果一共有 100 个 skynet 节点，在它们启动完毕后，会建立起 9900条通讯通道。

## 1.2 特点

Skynet框架做两个必要的保证：

一、一个服务的 callback 函数永远不会被并发。

二、一个服务向另一个服务发送的消息的次序是严格保证的。

​ 用多线程模型来实现它。底层有一个线程消息队列，消息由三部分构成：源地址、目的地址、以及数据块。框架启动固定的多条线程，每条工作线程不断从消息队列取到消息，调用服务的 callback 函数。

​ 线程数应该略大于系统的 CPU 核数，以防止系统饥饿。（只要服务不直接给自己不断发新的消息，就不会有服务被饿死）

​ 对于目前的点对点消息，要求发送者调用 malloc 分配出消息携带数据用到的内存；由接受方处理完后调用 free 清理（由框架来做）。这样数据传递就不需要有额外的拷贝了。

​ 做为核心功能，Skynet 仅解决一个问题：

​ 把一个符合规范的 C 模块，从动态库（so 文件）中启动起来，绑定一个永不重复（即使模块退出）的数字 id 做为其 handle 。模块被称为服务（Service），服务间可以自由发送消息。每个模块可以向 Skynet 框架注册一个 callback 函数，用来接收发给它的消息。每个服务都是被一个个消息包驱动，当没有包到来的时候，它们就会处于挂起状态，对 CPU 资源零消耗。如果需要自主逻辑，则可以利用 Skynet 系统提供的 timeout 消息，定期触发。

## 1.3 Actor模型

### 1.3.1 Actor模型介绍

​ **Actor模型内部的状态由它自己维护即它内部数据只能由它自己修改(通过消息传递来进行状态修改)，所以使用Actors模型进行并发编程可以很好地避免这些问题，Actor由状态(state)、行为(Behavior)和邮箱(mailBox)三部分组成**

1. 状态(state)：Actor中的状态指的是Actor对象的变量信息，状态由Actor自己管理，避免了并发环境下的锁和内存原子性等问题

2. 行为(Behavior)：行为指定的是Actor中计算逻辑，通过Actor接收到消息来改变Actor的状态

3. 邮箱(mailBox)：邮箱是Actor和Actor之间的通信桥梁，邮箱内部通过FIFO消息队列来存储发送方Actor消息，接受方Actor从邮箱队列中获取消息

Actor的基础就是消息传递，skynet中每个服务就是一个LUA虚拟机，就是一个Actor。

### 1.3.2 Actor模型好处

1. 事件模型驱动： Actor之间的通信是异步的，即使Actor在发送消息后也无需阻塞或者等待就能够处理其他事情。

2. 强隔离性： Actor中的方法不能由外部直接调用，所有的一切都通过消息传递进行的，从而避免了Actor之间的数据共享，想要观察到另一个Actor的状态变化只能通过消息传递进行询问。

3. 位置透明： 无论Actor地址是在本地还是在远程机上对于代码来说都是一样的。

4. 轻量性：Actor是非常轻量的计算单机，只需少量内存就能达到高并发。



# 2 在ubuntu上搭建skynet

## 2.1 获取skynet源代码

1. **安装git代码管理工具**​
   
   $ sudo apt-get update
   $ sudo apt-get install git 
   
   ​ *注意：如果安装失败，请先安装一下只支持库*
   
   $ sudo apt-get install build-essential libssl-dev libcurl4-gnutls-dev libexpat1-dev gettext unzip
   
   ​

2. **到github上面下载skynet的源代码**​ skynet的代码保存在[github](https://github.com/cloudwu/skynet)上面，大家可以去上面查看，现在我们用git把代码拷贝一份下来：
   
   $ git clone https://github.com/cloudwu/skynet.git
   
   ​

## 2.3 skynet代码目录结构

![](https://img-blog.csdn.net/20180303204144535)  

3rd #第三方支持库，包括LUA虚拟机，jmalloc等
lualib #lua语言封装的常用库，包括http、md5
lualib-src #将c语言实现的插件捆绑成lua库，例如数据库驱动、bson、加密算法等
service #使用lua写的Skynet的服务模块
service-src #使用C写的Skynet的服务模块
skynet-src #skynet核心代码目录
test #使用lua写的一些测试代码
examples #示例代码
Makefile #编译规则文件，用于编译
platform.mk #编译与平台相关的设置

## 2.4 编译与运行skynet服务器

1. **编译skynet**
   
   $ cd skynet  #今后我们所有的工作都在这个目录中进行
   $ make linux
   
   #如果报错： 
   ./autogen.sh: 5: ./autogen.sh: autoconf: not found
   #安装autoconf
   $ sudo apt-get install autoconf
   
   #如果报错：
   lua.c:83:31: fatal error: readline/readline.h: No such file or directory
   #安装libreadline-dev
   $ sudo apt-get install libreadline-dev
   
   #编译成功出现以下提示
   make[1]: Leaving directory '/home/ubuntu/workspace/skynet'
   #并且在目录里出现一个可执行文件skynet
   
   ​

2. **运行第一个skynet节点**​
   
   #启动一个skynet服务节点
   $ ./skynet examples/config 

## 2.5 运行客户端

我们要运行的的客户端是example/client.lua 这个lua脚本文件，那么首先你要有一个lua虚拟机程序。

1. **编译lua虚拟机**
   
   #打开另一个终端，开始编译虚拟机
   $ cd ./3rd/lua/ 
   $ make linux
   #编译成功则会在当前路径上面看到一个可执行文件lua

2. **运行客户端**
   
   #跑到skynet根目录
   $ cd ../../
   #运行client.lua这个脚本
   $ ./3rd/lua/lua examples/client.lua



# # 3 构建服务的基础API

local skynet = require "skynet" 
​
--conf配置信息已经写入到注册表中，通过该函数获取注册表的变量值
skynet.getenv(varName) 。
​
--设置注册表信息，varValue一般是number或string，但是不能设置已经存在的varname
skynet.setenv(varName, varValue) 
​
--打印函数
skynet.error(...)
​
--用 func 函数初始化服务，并将消息处理函数注册到 C 层，让该服务可以工作。
skynet.start(func) 
​
--若服务尚未初始化完成，则注册一个函数等服务初始化阶段再执行；若服务已经初始化完成，则立刻运行该函数。
skynet.init(func) 
​
--结束当前服务
skynet.exit() 
​
--获取当前服务的句柄handler
skynet.self()
​
--将handle转换成字符串
skynet.address(handler)
​
--退出skynet进程
require "skynet.manager"   --除了需要引入skynet包以外还要再引入skynet.manager包。
skynet.abort()
​
--强制杀死其他服务
skynet.kill(address) --可以用来强制关闭别的服务。但强烈不推荐这样做。因为对象会在任意一条消息处理完毕后，毫无征兆的退出。所以推荐的做法是，发送一条消息，让对方自己善后以及调用 skynet.exit 。注：skynet.kill(skynet.self()) 不完全等价于 skynet.exit() ，后者更安全。

## 3.1 编写一个test服务

1. **编写一个最简单的服务test.lua**
   
   --引入或者说是创建一个skynet服务
   local skynet = require "skynet" 
   --调用skynet.start接口，并定义传入回调函数
   skynet.start(function()
   skynet.error("Server First Test")
   end)

2. **修改exmaple/config文件中的start的值为test，表示启动test.lua，修改之前请备份**
   
   include "config.path"
   -- preload = "./examples/preload.lua" -- run preload.lua before every lua service run
   thread = 2
   logger = nil
   logpath = "."
   harbor = 1
   address = "127.0.0.1:2526"
   master = "127.0.0.1:2013"
   start = "test" -- main script  --将start的值修改为test
   bootstrap = "snlua bootstrap" -- The service for bootstrap
   standalone = "0.0.0.0:2013"
   -- snax_interface_g = "snax_g"
   cpath = root.."cservice/?.so"
   -- daemon = "./skynet.pid"

3. **通过skynet来运行test.lua**
   
   $ ./skynet examples/config
   
   运行结果
   
   $ ./skynet examples/config
   [:01000001] LAUNCH logger 
   [:01000002] LAUNCH snlua bootstrap
   [:01000003] LAUNCH snlua launcher
   [:01000004] LAUNCH snlua cmaster
   [:01000004] master listen socket 0.0.0.0:2013
   [:01000005] LAUNCH snlua cslave
   [:01000005] slave connect to master 127.0.0.1:2013
   [:01000004] connect from 127.0.0.1:52132 4
   [:01000006] LAUNCH harbor 1 16777221
   [:01000004] Harbor 1 (fd=4) report 127.0.0.1:2526
   [:01000005] Waiting for 0 harbors
   [:01000005] Shakehand ready
   [:01000007] LAUNCH snlua datacenterd
   [:01000008] LAUNCH snlua service_mgr
   [:01000009] LAUNCH snlua test
   [:01000009] Server First Test
   [:01000002] KILL self
   
   *注意：千万不要在skynet根目录以外的地方执行skynet，例如：*
   
   $ cd examples
   $ ../skynet config
   try open logger failed : ./cservice/logger.so: cannot open shared object file: No such file or directory
   Can't launch logger service
   $ 

​ 以上出现找不到logger.so的情况，其实不仅仅是这个模块找不到，所有的模块都找不到了，因为在config包含的路劲conf.path中，所有的模块路劲的引入全部依靠着相对路劲。一旦执行skynet程序的位置不一样了，相对路劲也会不一样。

![](https://img-blog.csdn.net/20180303204628318)

4. **添加自己的LUA脚本路劲**
   
   例如：添加my_workspace目录，则只需在luaservice值的基础上再添加一个`root.."my_workspace/？.lua;"`,注意：各个路劲通过一个`;` 隔开。
   
   ![](https://img-blog.csdn.net/20180303204640633)
   
   把我们的刚才写的test.lua丢到my_workspace中
   
   $ mv examples/test.lua my_workspace/
   
   顺便将example下的conf以及conf.path也拷贝一份到my_workspace
   
   $ cp examples/config my_workspace/
   $ cp examples/config.path my_workspace/
   
   这次运行的时候就可以这样了：
   
   $ ./skynet my_workspace/conf

## 3.2 另外一个种启动服务的方式

另一种方式启动想要的服务，可以在main.lua运行后，在console直接输入需要启动的服务名称.

1. **先启动main.lua服务，注意还原examples/conf默认配置，并且在example/conf.path添加自己的服务目录**。

 $ ./skynet examples/conf  #conf中start配置为启动main.lua
 [:01000001] LAUNCH logger 
 [:01000002] LAUNCH snlua bootstrap
 [:01000003] LAUNCH snlua launcher
 [:01000004] LAUNCH snlua cmaster
 [:01000004] master listen socket 0.0.0.0:2013
 [:01000005] LAUNCH snlua cslave
 [:01000005] slave connect to master 127.0.0.1:2013
 [:01000004] connect from 127.0.0.1:52698 4
 [:01000006] LAUNCH harbor 1 16777221
 [:01000004] Harbor 1 (fd=4) report 127.0.0.1:2526
 [:01000005] Waiting for 0 harbors
 [:01000005] Shakehand ready
 [:01000007] LAUNCH snlua datacenterd
 [:01000008] LAUNCH snlua service_mgr
 [:01000009] LAUNCH snlua main
 [:01000009] Server start
 [:0100000a] LAUNCH snlua protoloader
 [:0100000b] LAUNCH snlua console
 [:0100000c] LAUNCH snlua debug_console 8000
 [:0100000c] Start debug console at 127.0.0.1:8000
 [:0100000d] LAUNCH snlua simpledb
 [:0100000e] LAUNCH snlua watchdog
 [:0100000f] LAUNCH snlua gate
 [:0100000f] Listen on 0.0.0.0:8888
 [:01000009] Watchdog listen on 8888
 [:01000009] KILL self
 [:01000002] KILL self

​ 2. **在启动的main服务中，直接输入test，回车**

 $ ./skynet examples/config
 [:01000001] LAUNCH logger 
 [:01000002] LAUNCH snlua bootstrap
 [:01000003] LAUNCH snlua launcher
 [:01000004] LAUNCH snlua cmaster
 [:01000004] master listen socket 0.0.0.0:2013
 [:01000005] LAUNCH snlua cslave
 [:01000005] slave connect to master 127.0.0.1:2013
 [:01000004] connect from 127.0.0.1:52698 4
 [:01000006] LAUNCH harbor 1 16777221
 [:01000004] Harbor 1 (fd=4) report 127.0.0.1:2526
 [:01000005] Waiting for 0 harbors
 [:01000005] Shakehand ready
 [:01000007] LAUNCH snlua datacenterd
 [:01000008] LAUNCH snlua service_mgr
 [:01000009] LAUNCH snlua main
 [:01000009] Server start
 [:0100000a] LAUNCH snlua protoloader
 [:0100000b] LAUNCH snlua console
 [:0100000c] LAUNCH snlua debug_console 8000
 [:0100000c] Start debug console at 127.0.0.1:8000
 [:0100000d] LAUNCH snlua simpledb
 [:0100000e] LAUNCH snlua watchdog
 [:0100000f] LAUNCH snlua gate
 [:0100000f] Listen on 0.0.0.0:8888
 [:01000009] Watchdog listen on 8888
 [:01000009] KILL self
 [:01000002] KILL self
**** test    #终端输入****
 [:01000010] LAUNCH snlua test 
 [:01000010] Server First Test   #服务已经启动

## 3.3 环境变量

​ 1、预先加载的环境变量是在conf中配置的，加载完成后，所有的service都能去获取这些变量。

​ 2、也可以去设置环境变量，但是不能修改已经存在的环境变量。

​ 3、环境变量设置完成后，当前节点上的所有服务都能访问的到。

​ 4、环境变量设置完成后，及时服务退出了，环境变量依然存在，所以不要滥用环境变量。

例如在conf中添加：

myname = "Dmaker"
myage = 20

**示例代码：testenv.lua**

```
local skynet = require "skynet" 
​
skynet.start(function()
 --获取环境变量myname和myage的值，成功返回其值，如果该环境变量不存在返回nil
 local name = skynet.getenv("myname") 
local age = skynet.getenv("myage") 
skynet.error("My name is", name, ",", age, "years old.")

--skynet.setenv("myname", "coder") --不要尝试设置已经存在的变量值，会报错
 --skynet.setenv("myage", 21)
​
 skynet.setenv("mynewname", "coder") --设置一个新的变量
 skynet.setenv("mynewage", 21)
​
 name = skynet.getenv("mynewname") 
age = skynet.getenv("mynewage") 
skynet.error("My new name is", name, ",", age, "years old soon.")
 skynet.exit()
end)
```

## 3.4 skynet.init的使用

​ skynet.init用来注册服务初始化之前，需要执行的函数。也就是在skynet.start之前运行。

**示例代码：testinit.lua**

```
local skynet = require "skynet" 
​
skynet.init(function()
 skynet.error("service init")
end)
​
skynet.start(function()
 skynet.error("service start")
end)
```

运行结果：

[:01000009] service init #先运行skynet.init
[:01000009] service start   #运行skynet.start函数

# 4 服务类型

​ skynet中的服务分为普通服务与全局唯一服务。第3节启动方式就是一个普通服务，而全局唯一服务顾名思义就是在skynet中只能生成一个服务实例。

## 4.1 普通服务

​ 每调用一次创建接口就会创建出一个对应的服务实例，可以同时创建成千上万个，用唯一的id来区分每个服务实例。使用的创建接口是：

--[[

1. 用于启动一个新的 Lua 服务,luaServerName是脚本的名字（不用写 .lua 后缀）。

2. 只有被启动的脚本的 start 函数返回后，这个 API 才会返回启动的服务的地址，这是一个阻塞 API 。

3. 如果被启动的脚本在初始化环节抛出异常,skynet.newservice 也会执行失败。

4. 如果被启动的脚本的 start 函数是一个永不结束的循环，那么 newservice 也会被永远阻塞住。
   --]]
   skynet.newservice(luaServerName, ...)

5. 启动一个test服务
   
   testnewservice.lua

local skynet = require "skynet" 
​
--调用skynet.start接口，并定义传入回调函数
skynet.start(function()
 skynet.error("My new service")
 skynet.newservice("test")
 skynet.error("new test service")
end)

​ 启动main.lua，并且输入testnewservice

$ ./skynet examples/config #默认启动main.lua服务
testnewservice
[:01000010] LAUNCH snlua testnewservice  #通过main服务，启动testnewservice服务
[:01000010] My new service 
[:01000012] LAUNCH snlua test #再启动test服务
[:01000012] Server First Test
[:01000010] new test service

2. 启动两个test服务
   
   testnewservice2.lua

local skynet = require "skynet" 
​
--调用skynet.start接口，并定义传入回调函数
skynet.start(function()
 skynet.error("My new service")
 skynet.newservice("test")
 skynet.error("new test service 0")
 skynet.newservice("test")
 skynet.error("new test service 1")
end)

​ 启动main.lua，并且输入testnewservice

$ ./skynet examples/config  #默认启动main.lua服务
testnewservice #终端输入
[:01000010] LAUNCH snlua testnewservice  #通过main服务，启动testnewservice服务
[:01000010] My new service
[:01000012] LAUNCH snlua test     #启动一个test服务
[:01000012] Server First Test
[:01000010] new test service 0
[:01000019] LAUNCH snlua test   #再启动一个test服务
[:01000019] Server First Test
[:01000010] new test service 1

## 4.2 全局唯一服务

全局唯一的服务等同于单例，即不管调用多少次创建接口，最后都只会创建一个此类型的服务实例，且全局唯一。

- 创建接口：
  
  skynet.uniqueservice(servicename, ...) 
  skynet.uniqueservice(true, servicename, ...) 
  
  当带参数 `true` 时，则表示此服务在所有节点之间是唯一的。第一次创建唯一服，返回服务地址，第二次创建的时候不会正常创建服务，只是返回第一次创建的服务地址。

- 查询接口： 假如不清楚当前创建了此全局服务没有，可以通过以下接口来查询：
  
  skynet.queryservice(servicename, ...) 
  skynet.queryservice(true, servicename, ...) 
  
  如果还没有创建过目标服务则一直等下去，直到目标服务被(其他服务触发而)创建。
  
  当参数 带`true` 时，则表示查询在所有节点中唯一的服务是否存在。

### 4.2.1 测试skynet.uniqueservice接口

<mark>示例：testunique.lua</mark>

```
local skynet = require "skynet" 
​
local args = { ... }
if(#args == 0) then
 table.insert(args, "uniqueservice")
end
​
skynet.start(function()
 local us
 skynet.error("test unique service")
 if ( #args == 2 and args[1] == "true" )  then
 us = skynet.uniqueservice(true, args[2])
 else
 us =skynet.uniqueservice(args[1])
 end
 skynet.error("uniqueservice handler:", skynet.address(us))
end)

示例：uniqueservice.lua

local skynet = require "skynet" 
​
skynet.start(function()
 skynet.error("Server First Test")
 --skynet.exit() 不要尝试服务初始化阶段退出服务，唯一服会创建失败
end)
```

<mark>运行结果</mark>：

$ ./skynet examples/config
testunique  #终端输入
[:01000010] LAUNCH snlua testunique
[:01000010] test unique service
[:01000012] LAUNCH snlua uniqueservice #第一次创建全局唯一服uniqueservice成功
[:01000012] unique service start
[:01000010] uniqueservice handler: :01000012
testunique #终端输入
[:01000019] LAUNCH snlua testunique 
[:01000019] test unique service
[:01000019] uniqueservice handler: :01000012  #第二次创建并没有创建全局唯一服
​

### 4.2.2 测试skynet.queryservice接口

<mark>示例：testqueryservice.lua</mark>

```
local skynet = require "skynet" 
​
local args = { ... }
if(#args == 0) then
 table.insert(args, "uniqueservice")
end
skynet.start(function()
 local us
 skynet.error("start query service")
 --如果test服务未被创建，该接口将会阻塞，后面的代码将不会执行
 if ( #args == 2 and args[1] == "true" )  then
 us = skynet.queryservice(true, args[2])
 else
 us = skynet.queryservice(args[1]) 
end
 skynet.error("end query service handler:", skynet.address(us))
end)

```

<mark>运行结果</mark>：

如果不启动test全局唯一服务，直接执行查询函数

$ ./skynet examples/config
testqueryservice   #终端输入
[:01000010] LAUNCH snlua testqueryservice
[:01000010] start query service            #阻塞住，不再执行后面的代码

启动test全局唯一服务，再执行查询函数

$ ./skynet examples/config
testunique #终端输入
[:01000010] LAUNCH snlua testunique
[:01000010] test unique service
[:01000012] LAUNCH snlua uniqueservice #第一次创建全局唯一服成功
[:01000012] Server First Test
[:01000010] uniqueservice handler: :01000012
testqueryservice
[:01000019] LAUNCH snlua testqueryservice #再启动查询
[:01000019] start query service
[:01000019] end query service handler: :01000012 #skynet.queryservice将会返回

注意：

​ 当调用uniqueservice只传一个服务名时，表示创建当前skynet节点的全局唯一服务。当第一个参数传递true，第二个参数传递服务名时，则表示所有节点的全局唯一服务。

​ 调用queryservice时，也可以选择是否传递第一个参数true， 表示查询的是当前skynet节点的全局唯一服，还是所有节点的全局唯一服。这两种全局唯一服作用范围是不同，所有可以同时存在同名的但作用范围不同的全局唯一服。

## 4.3 多节点中的全局服务

### 4.3.1 启动两个skynet节点

​ 首先，我们先启动两个节点出来。copy两个份`examp/config`为`config1`与`config2`, config1中修改如下：

<mark>config1</mark>：

```
include "config.path"
​
-- preload = "./examples/preload.lua" -- run preload.lua before every lua service run
thread = 2 
logger = nil
logpath = "."
harbor = 1 --表示每个节点编号
address = "127.0.0.1:2526"
master = "127.0.0.1:2013"
start = "console" -- main script 只启动一个console.lua服务
bootstrap = "snlua bootstrap" -- The service for bootstrap
standalone = "0.0.0.0:2013" --主节点才会用到这个，绑定地址
-- snax_interface_g = "snax_g"
cpath = root.."cservice/?.so"
-- daemon = "./skynet.pid"
```

<mark>config2</mark>：

```
include "config.path"
​
-- preload = "./examples/preload.lua" -- run preload.lua before every lua service run
thread = 2 
logger = nil
logpath = "."
harbor = 2 --编号需要改
address = "127.0.0.1:2527"   --改一个跟config1不同的端口
master = "127.0.0.1:2013" --主节点地址不变
start = "console" -- main script
bootstrap = "snlua bootstrap" -- The service for bootstrap
--standalone = "0.0.0.0:2013" --作为从节点，就注释掉这里
-- snax_interface_g = "snax_g"
cpath = root.."cservice/?.so"
-- daemon = "./skynet.pid"
```

启动两个终端分别启动如下：

<mark>节点1启动</mark>：

./skynet examples/config1 
[:01000001] LAUNCH logger 
[:01000002] LAUNCH snlua bootstrap
[:01000003] LAUNCH snlua launcher
[:01000004] LAUNCH snlua cmaster  #启动主节点cmaster服务
[:01000004] master listen socket 0.0.0.0:2013  #监听端口2013
[:01000005] LAUNCH snlua cslave #主节点也要启动一个cslave，去连接cmaster节点
[:01000005] slave connect to master 127.0.0.1:2013 #cslave中一旦连接完cmaster就会启动一个harbor服务
[:01000004] connect from 127.0.0.1:47660 4 
[:01000006] LAUNCH harbor 1 16777221 #cslave启动一个Harbor服务 用于节点间通信
[:01000004] Harbor 1 (fd=4) report 127.0.0.1:2526 #报告cmaster cslave服务的地址
[:01000005] Waiting for 0 harbors #cmaster告诉cslave还有多少个其他cslave需要连接
[:01000005] Shakehand ready         #cslave与cmaster握手成功
[:01000007] LAUNCH snlua datacenterd
[:01000008] LAUNCH snlua service_mgr
[:01000009] LAUNCH snlua console
[:01000002] KILL self
[:01000004] connect from 127.0.0.1:47670 6 #cmaster收到其他cslave连接请求
[:01000004] Harbor 2 (fd=6) report 127.0.0.1:2527  #其他cslave报告地址
[:01000005] Connect to harbor 2 (fd=7), 127.0.0.1:2527 #让当前cslave去连接其他cslave

<mark>节点2启动</mark>：

./skynet examples/config2 
[:02000001] LAUNCH logger 
[:02000002] LAUNCH snlua bootstrap
[:02000003] LAUNCH snlua launcher
[:02000004] LAUNCH snlua cslave
[:02000004] slave connect to master 127.0.0.1:2013 #cslave去连接主节点的cmaster服务
[:02000005] LAUNCH harbor 2 33554436 #cslave也启动一个harbor服务
[:02000004] Waiting for 1 harbors #等待主节点的cslave来连接
[:02000004] New connection (fd = 3, 127.0.0.1:37470) #cslave与主节点cslave连接成功
[:02000004] Harbor 1 connected (fd = 3)
[:02000004] Shakehand ready   #cslave与cmaster握手成功
[:02000006] LAUNCH snlua service_mgr
[:02000007] LAUNCH snlua console
[:02000002] KILL self

### 4.3.2 测试全节点全局唯一服

在第一个节点中启动testunique.lua服务，然后第二个节点中启动testqueryservice.lua服务查询

<mark>节点1</mark>：

testunique true uniqueservice #所有节点全局唯一服方式启动
[:0100000b] LAUNCH snlua testunique true uniqueservice
[:0100000b] test unique service
[:0100000c] LAUNCH snlua uniqueservice
[:0100000c] Server First Test
[:0100000b] uniqueservice handler: :0100000c

<mark>节点2</mark>：

testqueryservice true uniqueservice
[:02000012] LAUNCH snlua testqueryservice true uniqueservice
[:02000012] start query service
[:02000012] end query service handler: :0100000b#节点1中已经启动了，所以节点2中

### 4.3.3 本地全局唯一服与全节点全局唯一服区别

节点2还可以创建一个同脚本的本地全局唯一服：

testunique uniqueservice
[:0100000c] LAUNCH snlua testunique 
[:0100000c] test unique service
[:0100000d] LAUNCH snlua uniqueservice
[:0100000d] Server First Test
[:0100000c] uniqueservice handler: :0100000d #创建了一个本地全局唯一服

但是无法创建一个新的全节点全局唯一服：

testunique true uniqueservice
[:0100000e] LAUNCH snlua testunique true uniqueservice
[:0100000e] test unique service
[:0100000e] uniqueservice handler: :0100000b #还是节点1的全局唯一服句柄

# 5 服务别名

​ 每个服务启动之后，都有一个整形数来表示id，也可以使用字符串id来表示，例如：`:01000010`，其实就是把id：`0x01000010`转换成字符串。

​ 但是这个数字的表示方式会根据服务的启动先后顺序而变化，不是一个固定的值。如果想要方便的获取某个服务，那么可以通过给服务设置别名来。

​

## 5.1 本地别名与全局别名

​ 在skynet中，服务别名可以分为两种：

- 一种是本地别名，本地别名只能在当前skynet节点使用，本地别名必须使用`.` 开头，例如：`.testalias`

- 一种是全局别名，全局别名可以在所有skynet中使用，全局别名不能以`.` 开头， 例如：`testalias`

## 5.2 别名注册与查询接口

------------------------------[[取别名]]--------------------------
local skynet = require "skynet"
require "skynet.manager"
​
--给当前服务定一个别名，可以是全局别名，也可以是本地别名
skynet.register(aliasname)
​
--给指定servicehandler的服务定一个别名，可以是全局别名，也可以是本地别名
skynet.name(aliasname, servicehandler)

-------------------------------------------------------------------

----------------------[[查询别名]]------------------------------
--查询本地别名为aliasname的服务，返回servicehandler，不存在就返回nil
skynet.localname(aliasname)
​
--[[
查询别名为aliasname的服务,可以是全局别名也可以是本地别名，
1、当查询本地别名时，返回servicehandler，不存在就返回nil
2、当查询全局别名时，返回servicehandler，不存在就阻塞等待到该服务初始化完成
]]--
local skynet = require "skynet.harbor"
harbor.queryname(aliasname)

-------------------------------------------------------------------

注意：本地别名与全局别名可以同时存在。

## 5.3 给服务注册别名

### 5.3.1给普通服取别名

示例代码：<mark>testalias.lua</mark>

```
local skynet = require "skynet" 
require "skynet.manager"
local harbor = require "skynet.harbor"
​
skynet.start(function()
​
 local handle = skynet.newservice("test")
​
 skynet.name(".testalias", handle) --给服务起一个本地别名
 skynet.name("testalias", handle)    --给服务起一个全局别名

handle = skynet.localname(".testalias") 
skynet.error("localname .testalias handle", skynet.address(handle))

handle = skynet.localname("testalias")   --只能查本地，不能查全局别名 
skynet.error("localname testalias handle", skynet.address(handle))

handle = harbor.queryname(".testalias") 
skynet.error("queryname .testalias handle", skynet.address(handle))

handle = harbor.queryname("testalias") 
skynet.error("queryname testalias handle", skynet.address(handle))

​
end)
```

- <mark>上面服务通过skynet.newservice来启动一个test.lua服务，test.lua代码如下</mark>：

```
local skynet = require "skynet" 
skynet.start(function()
 skynet.error("My new service")
end)
```

- 运行结果：

$ ./skynet examples/config
testalias #运行main.lua后在终端输入
[:0100000a] LAUNCH snlua testalias
[:0100000b] LAUNCH snlua test
[:0100000b] My new service
[:0100000a] localname .testalias handle :0100000b #skynet.localname查到本地名
[:0100000a] localname testalias handle nil #skynet.localname查不到全局名
[:0100000a] queryname .testalias handle :0100000b #harbor.queryname查到本地名
[:0100000a] queryname testalias handle :0100000b #harbor.queryname查到全局名

### 5.3.2 全局别名查询阻塞

​ 如果全局别名不存在，那么这个时候调用函数`harbor.queryname`，将会阻塞，直到全局别名的服务创建成功。

<mark>示例代码：testalias.lua</mark>

```
local skynet = require "skynet" 
require "skynet.manager"
local harbor = require "skynet.harbor"
​
skynet.start(function()

handle = skynet.localname(".testalias")   --查询本地别名不阻塞
 skynet.error("localname .testalias handle", skynet.address(handle))

handle = skynet.localname("testalias") --无法查询全局别名
 skynet.error("localname testalias handle", skynet.address(handle))

handle = harbor.queryname(".testalias") --查询本地别名不阻塞 
skynet.error("queryname .testalias handle", skynet.address(handle))

handle = harbor.queryname("testalias") --查询全局别名阻塞 
skynet.error("queryname testalias handle", skynet.address(handle))

​
end)
```

运行结果：

$ ./skynet examples/config 
testalias
[:0100000a] LAUNCH snlua testalias
[:0100000a] localname .testalias handle nil #skynet.localname查到本地名
[:0100000a] localname testalias handle nil #skynet.localname查不到全局名
[:0100000a] queryname .testalias handle nil #harbor.queryname查到本地名
 #harbor.queryname查不到全局名, 函数阻塞

### 5.3.3 多节点中的全局别名

​ 启动两个skynet节点，在节点1取别名，节点2查询别名：

- <mark>节点1，testaliasname.lua</mark>

```
local skynet = require "skynet" 
require "skynet.manager"
local harbor = require "skynet.harbor"
​
skynet.start(function()
 local handle = skynet.newservice("test")
 skynet.name(".testalias", handle) --给服务起一个本地别名
 skynet.name("testalias", handle)    --给服务起一个全局别名
end)
```

- <mark>节点2, testaliasquery.lua</mark>

```
local skynet = require "skynet" 
require "skynet.manager"
local harbor = require "skynet.harbor"
​
skynet.start(function()

handle = skynet.localname(".testalias") 
skynet.error("localname .testalias handle", skynet.address(handle))

handle = skynet.localname("testalias") 
skynet.error("localname testalias handle", skynet.address(handle))

handle = harbor.queryname(".testalias") 
skynet.error("queryname .testalias handle", skynet.address(handle))

handle = harbor.queryname("testalias") 
skynet.error("queryname testalias handle", skynet.address(handle))
​
end)
```

- 先启动节点1运行testaliasname.lua，再启动节点2运行

testaliasquery
[:0200000a] LAUNCH snlua testaliasquery
[:0200000a] localname .testalias handle nil
[:0200000a] localname testalias handle nil
[:0200000a] queryname .testalias handle nil
[:0200000a] queryname testalias handle :0100000b --查询到节点1创建的服务

### 5.3.4 杀死带别名的服务

​ 给一个服务取了别名后，杀死它，本地别名将会注销掉，但是全局别名依然存在，通过全局别名查询到的handle已经没有意义。如果通过handle进行一些操作将得到不可预知的问题。

```

```

运行结果：

testalias
[:0100000a] LAUNCH snlua testalias
[:0100000b] LAUNCH snlua test
[:0100000b] My new service
[:0100000a] localname .testalias handle :0100000b
[:0100000a] localname testalias handle nil
[:0100000a] queryname .testalias handle :0100000b
[:0100000a] queryname testalias handle :0100000b
[:0100000a] KILL :100000b
[:0100000a] localname .testalias handle nil
[:0100000a] localname testalias handle nil
[:0100000a] queryname .testalias handle nil
[:0100000a] queryname testalias handle :0100000b #全局别名还存在，但是已经杀死该服务了。

​ skynet的全局别名服务是在cslave里面实现的，现在不允许二次修改全局别名绑定关系，所以全局别名一般用来给一个永远不会退出的服务来启用。

​ 但是有些情况下，我们确实需要二次修改全局别名绑定关系，那么这个时候，我们可以尝试去修改一下`cslave.lua`文件，修改内容如下：

function harbor.REGISTER(fd, name, handle)
 --assert(globalname[name] == nil)  --将这一行注释掉
 globalname[name] = handle
 response_name(name)
 socket.write(fd, pack_package("R", name, handle))
 skynet.redirect(harbor_service, handle, "harbor", 0, "N " .. name)
end

运行一个二次修改全局别名绑定关系的服务，例如：

local skynet = require "skynet" 
require "skynet.manager"
local harbor = require "skynet.harbor"
​
skynet.start(function()
​
 local handle = skynet.newservice("test")
​
 skynet.name("testalias", handle)    --给服务起一个全局别名
​

 handle = harbor.queryname("testalias") 
 skynet.error("queryname testalias handle", skynet.address(handle))
​
​
 skynet.kill(handle) --杀死带全局别名服务
 handle = skynet.newservice("test")
 skynet.name("testalias", handle)    --全局别名给其他服务使用

 handle = harbor.queryname("testalias") 
 skynet.error("queryname testalias handle", skynet.address(handle))
​
​
end)

运行结果：

testalias
[:0100000a] LAUNCH snlua testalias
[:0100000b] LAUNCH snlua test
[:0100000b] My new service
[:0100000a] queryname testalias handle :0100000b
[:0100000a] KILL :100000b
[:0100000c] LAUNCH snlua test
[:0100000c] My new service
[:0100000a] queryname testalias handle :0100000c   #服务别名二次修改成功

## 5.4 全局别名与全局唯一服名区别

​ 全局唯一服名与这里的全局别名是两个概念的名词。

​ 全局唯一服名称: 是用来标识服务是唯一的，服务名称一般就是脚本名称，无法更改。

​ 全局别名: 是用来给服务起别名的，既可以给普通服起别名，也可以给全局唯一服起别名。

​ 他们两种名字是在不同的体系中的，有各种的起名字的方式，以及查询的方式。

​ 所以不要尝试用skynet.queryservice查询一个全局别名，也不要尝试使用harbor.queryname去查询一个全局唯一服。

例如：

 local handle = skynet.uniqueservice("test") --启动一个全局唯一服，名字为test

 handle = harbor.queryname("test")    --查不到的，会一直阻塞

或者：

 local handle = skynet.uniqueservice("test") --启动一个全局唯一服，名字为test
​
 skynet.name("testalias", handle)    --再起一个全局别名

 handle = skynet.queryservice("testalias") --查不到的，也会一直阻塞
