CMD = setmetatable({}, {})

function CMD.func3()
  print("func3")
end

function fun1()
  function CMD.func2()
    print("func2")
  end
  print("func1")
end

print (CMD["func3"])
print (CMD["func2"]) --nil
--f()