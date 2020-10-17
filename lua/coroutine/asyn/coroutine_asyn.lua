--使用10协程将0自增到10
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