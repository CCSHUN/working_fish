--package.cpath = "luaclib/?.so"
--package.path = "lualib/?.lua;examples/?.lua"
package.path = "./lua/?.lua;./lua/module/?.lua"
local m = require("module")

print(m.constant) 
m.func3()