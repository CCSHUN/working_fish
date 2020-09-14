local coroutine_pool = {}
 
local function co_create(f)
	local co = table.remove(coroutine_pool)
	if co == nil then
		co = coroutine.create(
			function(...)
				f(...)        --2)
				while true do
					f = nil
					coroutine_pool[#coroutine_pool+1] = co
					f = coroutine.yield ("EXIT")   --3), 7)
					f(coroutine.yield())	       --8), 12)	 	  	
				end
			end)
	else
		coroutine.resume(co, f)       --6), 9)
	end
	return co     --10)
end

function fun1(var)
	print('this is ', var)
end

function fun2(var)
	print('this is ', var)
end

function fun3(var)
	print('this is ', var)
end

--lua一般用_表示占位符
co1 = co_create(fun1)
coroutine.resume(co1, "fun 1")

co2 = co_create(fun2)
co3 = co_create(fun3)
coroutine.resume(co2, "fun 2")
coroutine.resume(co3, "fun 3")

--local _, cmd = coroutine.resume(co_create(fun1), 'fun 1')    --1), 4)

--_, cmd = coroutine.resume(co_create(fun2), 'fun 2')          --5), 11)

--_, cmd = coroutine.resume(co_create(fun3), 'fun 3')
