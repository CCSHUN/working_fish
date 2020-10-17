token = "(user)@(server):(password)"
local user, server, password = token:match("([^@]+)@([^:]+):(.+)")
print(user, server, password)