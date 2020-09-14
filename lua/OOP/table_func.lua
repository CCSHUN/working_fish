local kv = {}
local command = {} 

function command.get(key)
  return kv[key]
end

function command.set(key, value)
  kv[key] = value
end

--第一种写法
my_command = command
my_command.set("key1", "value1")
print(my_command.get("key1")) -- 输出value1

--第二种写法 调用command函数
func_set = my_command["set"] --记得加双引号
func_get = my_command["get"]
func_set("key2", "value2")
print(func_get("key2"))


