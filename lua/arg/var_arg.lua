function average(...)
  result = 0
  local arg={...}    --> arg 为一个表，局部变量
  for i,v in ipairs(arg) do
     result = result + v
  end
  print("总共传入 " .. #arg .. " 个数")
  return result/#arg
end

--用select 获取参数数量
function average_v1(...)
  result = 0;
  args_num = select("#",...)
  for i=1, args_num do
    --读取参数
    local arg = select(i, ...)
    result = result + arg
  end
  print("v1总共传入 " .. args_num .. " 个数")
  return result/args_num
end

print("平均值为",average(10,5,3,4,5,6))
print("v1平均值为",average_v1(10,5,3,4,5,6))