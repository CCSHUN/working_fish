--用闭包实现迭代器
function list_iter(t)
  local i=0
  local n=#t
  return function()
      i=i+1
      if i<=n then return t[i] end
  end
end
--这里的list_iter是一个工厂，每次调用都会产生一个新的闭包该闭包内部包括了upvalue(t,i,n)
--因此每调用一次该函数产生的闭包那么该闭包就会根据记录上一次的状态，以及返回list的下一个

--这里的list_iter()工厂函数只会被调用一次产生一个闭包函数，后面的每一次迭代都是用该闭包函数，而不是工厂函数
t={10,0,29}
for element in list_iter(t) do  
    print(element)
end