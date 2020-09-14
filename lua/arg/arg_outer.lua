---函数传参
local args = {...}

--打印所有参数
for i=1, select("#", ...) do
  print(args[i])
end

--只捕捉前两个参数
local arg1,arg2 = ... 
print(arg1, arg2)