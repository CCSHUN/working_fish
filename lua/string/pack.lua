--字符串压缩，>表示大端序
str1 = string.pack(">s2", "123abc") 
print (str1, string.len(str1)) --123abc  8

local len = str1:byte(1) * 256 + str1:byte(2)
print ("len="..len) --len=6

--长度
len_str = str1:sub(3,8)
print ("len1="..len_str) --len1=123abc