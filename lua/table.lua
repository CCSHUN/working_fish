--注意点1 table的索引从一开始
table = {"A", "B", "C"}
print(table[0]) -- 输出nil
print(table[1]) -- 输出A

--key_value写法1
key_value = {}
key_value["chen"] = 1;
key_value["chang"] = 2;
key_value["shun"] = 3;

print(key_value["chen"]) --输出1
print(key_value["chang"]) -- 输出2
print(key_value["shun"]) --输出3

--key_value写法2
key_value = {}
key_value["chen"] = "chen";
key_value["chang"] = "chang";
key_value["shun"] = "shun";

print(key_value["chen"]) --输出chen
print(key_value["chang"]) -- 输出chan
print(key_value["shun"]) --输出shun

print(key_value[1]) --输出nil
print(key_value[2]) -- 输出nil
print(key_value[3]) --输出nil