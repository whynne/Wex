consolex = 20;
consoley = 20;

wex.console.show()
wex.console.setRows(10)
wex.console.setColumns(100)
wex.console.setPosition(consolex,consoley)
wex.console.print("Welcome to Wex v0.1a\n")
wex.audio.load("nightandday.ogg")
wex.audio.play("nightandday.ogg")

coordtext = "dicks"
sprite = Sprite.new()


function wex.init()

end

function wex.handleEvents()
    if(wex.input.keyPressed(key.down) == true or wex.input.keyHeld(key.down) == true) then
	       consoley = consoley + 2
		   sprite:setHeight(100)
		   wex.console.setPosition(consolex,consoley)
	end
	if(wex.input.keyPressed(key.up) == true or wex.input.keyHeld(key.up) == true) then
	        consoley = consoley - 2
			sprite:setWidth(100)
		    wex.console.setPosition(consolex,consoley)
		    wex.console.print("Y changed to "..consoley)
	end
end

function wex.update()

end

function wex.draw()
end