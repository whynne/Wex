--[[

Function:
This script will run only once during the game's startup.

Important names:
showConsole()           - shows console
hideConsole()           - hides console
setConsoleRows(num)     - sets number of rows (height) on console
setConsoleColumns(num)  - sets number of columns (width) on console
setConsolePosition(x,y) - sets the position on the screen of the console

--]]


room = {}
room.name = "First room"
room.description = "This is the first room of the game.  There's nothing particularly special about it."
room.exits = {"north","south","east","west"}

showConsole()
setConsoleRows(34)
setConsoleColumns(30)
setConsolePosition(170,170)

print("Hello!  Let's test out some things\n\n\n\n\n")
print("\n")
