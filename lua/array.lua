array = {"A", "B"}

--遍历方式1
for k,v in ipairs(array) do
  print(array[k])
end

--遍历方式2
for k,v in ipairs(array) do
  print(v)
end

--遍历方式3, 从1开始
for i=1, #array do
  print(array[i])
end
