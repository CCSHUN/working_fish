function routine_func() 
  for i=1, 10 do
    print(i)

  end
end

co = coroutine.wrap(routine_func)
co()
