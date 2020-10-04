# 1.skynet socket api

参考：[skynet框架应用 (九) socket网络服务_qq769651718的专栏-CSDN博客](https://blog.csdn.net/qq769651718/article/details/79434989)

```
--这样就可以在你的服务中引入这组 api 。
local socket = require "skynet.socket"
​
--建立一个 TCP 连接。返回一个数字 id 。
socket.open(address, port)      
--连接到服务器
int skynet_socket_connect(struct skynet_context *ctx, 
const char *host, int port);
​
--关闭一个连接，这个 API 有可能阻塞住执行流。因为如果有其它 coroutine 
--正在阻塞读这个 id 对应的连接，会先驱使读操作结束，close 操作才返回。
socket.close(id)
​
--在极其罕见的情况下，需要粗暴的直接关闭某个连接，而避免 socket.close 的阻塞等待流程，可以使用它。
socket.close_fd(id)
​
--强行关闭一个连接。和 close 不同的是，它不会等待可能存在的其它 coroutine 的读操作。
--一般不建议使用这个 API ，但如果你需要在 __gc 元方法中关闭连接的话，
--shutdown 是一个比 close 更好的选择（因为在 gc 过程中无法切换 coroutine）。与close_fd类似
socket.shutdown(id)
​
--[[
    从一个 socket 上读 sz 指定的字节数。
    如果读到了指定长度的字符串，它把这个字符串返回。
    如果连接断开导致字节数不够，将返回一个 false 加上读到的字符串。
    如果 sz 为 nil ，则返回尽可能多的字节数，但至少读一个字节（若无新数据，会阻塞）。
--]]
socket.read(id, sz)
​
--从一个 socket 上读所有的数据，直到 socket 主动断开，或在其它 coroutine 用 socket.close 关闭它。
socket.readall(id)
​
--从一个 socket 上读一行数据。sep 指行分割符。默认的 sep 为 "\n"。读到的字符串是不包含这个分割符的。
--如果另外一端就关闭了，那么这个时候会返回一个nil，如果buffer中有未读数据则作为第二个返回值返回。
socket.readline(id, sep) 
​
--等待一个 socket 可读。
socket.block(id) 
​
 
--把一个字符串置入正常的写队列，skynet 框架会在 socket 可写时发送它。
socket.write(id, str) 
​
--把字符串写入低优先级队列。如果正常的写队列还有写操作未完成时，低优先级队列上的数据永远不会被发出。
--只有在正常写队列为空时，才会处理低优先级队列。但是，每次写的字符串都可以看成原子操作。
--不会只发送一半，然后转去发送正常写队列的数据。
socket.lwrite(id, str) 
​
--监听一个端口，返回一个 id ，供 start 使用。
socket.listen(address, port)   --lua
int skynet_socket_listen(struct skynet_context *ctx, 
const char *host, int port, int backlog);  --c
​
--[[
    accept 是一个函数。每当一个监听的 id 对应的 socket 上有连接接入的时候，都会调用 accept 函数。
这个函数会得到接入连接的 id 以及 ip 地址。你可以做后续操作。
    每当 accept 函数获得一个新的 socket id 后，并不会立即收到这个 socket 上的数据。
这是因为，我们有时会希望把这个 socket 的操作权转让给别的服务去处理。accept(id, addr)
]]--
socket.start(id , accept)   --lua
void skynet_socket_start(struct skynet_context *ctx, int id); --c
​
--[[
    任何一个服务只有在调用 socket.start(id) 之后，才可以读到这个 socket 上的数据。
向一个 socket id 写数据也需要先调用 start 。
    socket 的 id 对于整个 skynet 节点都是公开的。也就是说，你可以把 id 这个数字
通过消息发送给其它服务，其他服务也可以去操作它。skynet 框架是根据调用 start 这个 
api 的位置来决定把对应 socket 上的数据转发到哪里去的。
--]]
socket.start(id)
​
--清除 socket id 在本服务内的数据结构，但并不关闭这个 socket 。
--这可以用于你把 id 发送给其它服务，以转交 socket 的控制权。
socket.abandon(id) 
​
--[[
    当 id 对应的 socket 上待发的数据超过 1M 字节后，系统将回调 callback 以示警告。
function callback(id, size) 回调函数接收两个参数 id 和 size ，size 的单位是 K 。
    如果你不设回调，那么将每增加 64K 利用 skynet.error 写一行错误信息。
--]]
socket.warning(id, callback) 
```



# 2.服务API

## 2.1基本API

```
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
require "skynet.manager"   --除了需要引入skynet包以外还要再引入skynet.manager包。
skynet.abort()

--新建一个服务
skynet.newservice --lua
struct skynet_context * skynet_context_new(const char * name, 
const char * parm);​ --c api


--强制杀死其他服务
skynet.kill(address) --可以用来强制关闭别的服务。但强烈不推荐这样做。因为对象会在任意一条消息处理完毕后，毫无征兆的退出。所以推荐的做法是，发送一条消息，让对方自己善后以及调用 skynet.exit 。注：skynet.kill(skynet.self()) 不完全等价于 skynet.exit() ，后者更安全。
```

## 2.2别名服务与注册

```
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
​//查本节点服务，返回服务handle,不存在则返回0
uint32_t skynet_queryname(struct skynet_context * context, const char * name);
--[[
查询别名为aliasname的服务,可以是全局别名也可以是本地别名，
1、当查询本地别名时，返回servicehandler，不存在就返回nil
2、当查询全局别名时，返回servicehandler，不存在就阻塞等待到该服务初始化完成
]]--
local skynet = require "skynet.harbor"
harbor.queryname(aliasname)
-------------------------------------------------------------------
```
