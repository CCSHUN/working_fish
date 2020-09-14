# 1.协程

   思考协程如何解决以下几类问题

    （1）异步回调如何用协程解决（目前没看懂）

                [Lua 协程应用 - 简书](https://www.jianshu.com/p/50ec48948cb9)

    （2）生产者消费者模式如何用协程解决

               例如生产者消费者模式，可根据业务情况，单次处理

    （3）协程池如何设计

            [lua协程池_专注于网络编程,游戏后台,高并发-CSDN博客](https://blog.csdn.net/zxm342698145/article/details/79728925?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.add_param_isCf&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.add_param_isCf)

            [lua 的协程池写法_weixin_42445065的博客-CSDN博客](https://blog.csdn.net/weixin_42445065/article/details/88117795)

参考：

[深入理解lua的协程coroutine_papaya的博客-CSDN博客](https://blog.csdn.net/yzf279533105/article/details/79982139)

协程样例1：

```
--云风协程调度样例
function foo(a)
    print("foo", a)
    return coroutine.yield(2 * a)
end

co = coroutine.create(function ( a, b )
    print("co-body", a, b)
    local r = foo(a + 1)
    print("co-body", r)
    local r, s = coroutine.yield(a + b, a - b)
    print("co-body", r, s)
    return b, "end"
end)

print("main", coroutine.resume(co, 1, 10))
print("main", coroutine.resume(co, "r"))
print("main", coroutine.resume(co, "x", "y"))
print("main", coroutine.resume(co, "x", "y"))
```

协程样例2：

```
--使用协程将0自增到10
local count = 0

--匿名函数
func = function (index)
  repeat 
    count = count + 1
    --print("index "..index .." count:"..count)
    coroutine.yield(count)
  until count == 10
end

co = coroutine.create(func)

repeat
  status, value = coroutine.resume(co)
  print(status, value)
until count == 10
print(coroutine.status( co )) --suspended
```

# 2.闭包

## 2.1unpack函数

```
local function fun2(...)
  local arg = { ... }  --5.2版本以后需要手动转化
  print(table.unpack(arg))          -- 1	nil	3
  print(table.unpack(arg, 1, #arg)) -- 1	nil	3
  print(table.unpack(arg, 2, #arg)) -- nil	3
end
fun2(1,nil,3) --1,nil,3
```

运行结果：

![](/Users/chen/Library/Application%20Support/marktext/images/2020-09-02-23-59-07-image.png)

用途：

用于传递闭包，例如skynet timeout传参

```
--timeout传参

local skynet = require "skynet"

function task1(arg)
    skynet.error("task",arg)
    skynet.timeout(100, function() task1(arg)
    end)
end

function task2(...)
    local n = select("#", ...)

    local args = {...}
    skynet.error("task", ...)
    skynet.timeout(100, function() task2(table.unpack(args, 1, n))
    end)
end

skynet.start(function ()
    skynet.error("start", coroutine.running())
    skynet.timeout(100, function() task1(":taskname1")
    end) --5秒钟之后运行task函数,只是注册一下回调函数，并不会阻塞
    skynet.timeout(100, function() task2(":taskname2")
    end)
end
```

输出：

    ![](/Users/chen/Library/Application%20Support/marktext/images/2020-09-03-00-18-41-image.png)
