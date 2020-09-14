network = { 
 {name = "grauna", IP = "210.26.30.34"}, 
 {name = "arraial", IP = "210.26.30.23"}, 
 {name = "lua", IP = "210.26.23.12"}, 
 {name = "derain", IP = "210.26.23.20"}, 
}

--使用匿名函数
table.sort(network, function (a,b) 
return (a.name > b.name) 
end)

for i=1,#network do
  print(network[i]["name"].." "..network[i]["IP"])
end
