--这是一段错误代码
function error_function(num1 ,num2)
    --print(num1 ,num2)
    a = {}
    b = {}
    return a + b
end

if pcall(error_function, 0, 1) then
  print("no error")
else
  print("error occur")
end