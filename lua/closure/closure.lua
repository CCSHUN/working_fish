names = {"Peter", "Paul", "Mary"} 
grades = {Mary = 10, Paul = 7, Peter = 8}

--按分数从大刀小排序
function sortbygrade (names, grades) 
  table.sort(names, function (n1, n2) 
  return grades[n1] > grades[n2] -- compare the grades 
  end) 
end

sortbygrade(names,grades)
--排序后遍历
for k,v in ipairs(names) do
  print(v)
end