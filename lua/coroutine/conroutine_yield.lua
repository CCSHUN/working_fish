--新创建为suspended状态
co = coroutine.create(function () 
  for i=1,10 do
    print("co", i) 
    coroutine.yield() 
   end 
end)

coroutine.resume(co) --> co 1 
print(coroutine.status(co)) --> suspended

---
for i=2,10 do
  coroutine.resume(co)
end

coroutine.resume(co) -- prints nothing
print(coroutine.status(co)) --dead
print(coroutine.resume(co)) --false	cannot resume dead coroutine

