for i=1,10 do
  if (i & 1) == 0 then
        print("偶数：",i)
	    goto continue
	end
	print("奇数：",i)
::continue::
end