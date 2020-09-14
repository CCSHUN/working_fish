Person = {sex="男", birth="1995", name="chen"}

function Person:new(o, sex, birth, name) 
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  self.sex = sex or ""
  self.birth = birth or ""
  self.name = name or ""
  return o;
end


function Person.behavior() 
  print(self) --不接受self 输出nil
  print("我是老师")
end

teacher = Person:new(nil, "女", "1996", "lingling")
teacher.behavior()

