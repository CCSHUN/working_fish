father = {
	house=1
}
son = {
	car=1
}
setmetatable(son, father) --把son的metatable设置为father
print(son.house)


--需要设置元表__index 为self
father = {
	house=1
}
son = {
	car=1
}
setmetatable(son, father) --把son的metatable设置为father
father.__index = father
print(son.house)

--元表__index 为函数
mytable = setmetatable({key1 = "value1"}, {
  __index = function(mytable, key)
    if key == "key2" then
      return "metatablevalue"
    else
      return nil
    end
  end
})
 
print(mytable.key1,mytable.key2)


--简化代码
mytable = setmetatable({key1 = "value1"}, { __index = { key2 = "metatablevalue" } })
print(mytable.key1,mytable.key2)