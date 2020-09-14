print(type("Hello world"))      --> string
print(type(10.4*3))             --> number
print(type(print))              --> function
print(type(type))               --> function
print(type(true))               --> boolean
print(type(nil))                --> nil
print(type(type(X)))            --> string

-- 这是需要注意的地方 x类型为nil type(x) 为string
x=nil
print(type(x))                  --> nil
print(type(type(x)))            --> string


table = {}                      --> table
print(type(table));