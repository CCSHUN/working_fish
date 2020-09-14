--函数返回多个值
function func()
  local a = 0
  local b = "string"
  return a,b
end

a,b = func()
print (a, b)