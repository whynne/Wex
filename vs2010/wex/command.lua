--[[

Function:
This script will run once for any command inputted by the user.

Important names:
command - string containing the inputted command

--]]

function dolook()
    print(room.description .. '\n')
	print("obvious exits are: ")
	for i,v in ipairs(room.exits) do
	    print(v .. ' ')
	end
	print('...\n')
end


if command == "hello" then
    print("And hello to you too.\n")
elseif command == "look" then
    dolook()
elseif command == "test" then
	print(ff,sd,"Gonna fuck it up")
elseif command == "load" then
	loadOgg("nightandday.ogg")
elseif command == "play" then
	playOgg("nightandday.ogg")
else
    print("I don't understand that.\n")
end



