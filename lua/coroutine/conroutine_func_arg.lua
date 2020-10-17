co = coroutine.create(function (a,b,c) 
  print("co", a,b,c) 
 end) 
coroutine.resume(co, 1, 2, 3) --> co 1 2 3
print(coroutine.status(co)) --dead

-- yield
co = coroutine.create(function (a,b) 
  coroutine.yield(a + b, a - b) 
 end) 
print(coroutine.resume(co, 20, 10)) --> true 30 10

