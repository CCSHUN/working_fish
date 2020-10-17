local function fun2(...)
  local arg = { ... }  --5.2版本以后需要手动转化
  print(table.unpack(arg))          -- 1	nil	3
  print(table.unpack(arg, 1, #arg)) -- 1	nil	3
  print(table.unpack(arg, 2, #arg)) -- nil	3
end
fun2(1,nil,3) --1,nil,3