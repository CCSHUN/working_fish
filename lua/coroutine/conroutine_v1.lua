co = coroutine.create(function () 
  print("hi") 
 end) 
print(co) --> thread: 0x8071d98

print(coroutine.status(co)) --> suspended
coroutine.resume(co) --> hi
print(coroutine.status(co)) --> dead