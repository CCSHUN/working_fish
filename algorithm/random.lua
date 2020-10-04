--实现一个洗牌算法
local table_old={"value1", "value2", "value3","value4"}

--返回一个随机值
function adjust(t)
  math.randomseed(os.time())
  local len = #t
  
  for i=1, len do
    index = math.random(1, len)
    temp = t[len]
    t[len] = t[index]
    t[index] = temp
    len = len - 1
  end
  return t[math.random(1, #t)]
end

function show(t)
  local len = #t
  for i=1, len do
    print (t[i])
  end
end

cur = adjust(table_old)
print ("cur = ", cur)
show(table_old)