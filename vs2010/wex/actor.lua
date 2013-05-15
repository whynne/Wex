Actor = {}
Actor.__index = Actor

function Actor.new(name)
   local actr = {}             -- our new object
   setmetatable(actr,Actor)    -- make Actor handle lookup
   actr.name = name      -- initialize our object
   actr.quad = Quad.new()
   actr.quad:setHeight(64)
   actr.quad:setWidth(64)
   actr.quad:setColor(1,1,1,1)
   actr.xvel = 0
   actr.yvel = 0
   actr.speed = 10
   actr.x = 50
   actr.y = 50
   actr.dvx = 0
   actr.dvy = 0
   actr.maxv = 20

   return actr
end

function Actor:accelUp()
	self.dvy = -self.speed * wex.delta
end

function Actor:accelDown()
	self.dvy = self.speed * wex.delta
end

function Actor:accelLeft()
    self.dvx = -self.speed * wex.delta
end

function Actor:accelRight()
    self.dvx = self.speed * wex.delta
end

function Actor:update()

    if(wex.input.keyPressed(key.down) == true or wex.input.keyHeld(key.down) == true) then
        player:accelDown()
	end
	if(wex.input.keyPressed(key.up) == true or wex.input.keyHeld(key.up) == true) then
        player:accelUp()
	end
	if(wex.input.keyPressed(key.right) == true or wex.input.keyHeld(key.right) == true) then
        player:accelRight()
	end
	if(wex.input.keyPressed(key.left) == true or wex.input.keyHeld(key.left) == true) then
        player:accelLeft()
	end

    if(self.yvel > self.maxv) then
	    self.yvel = self.maxv
		self.dvy = 0
	end
	if(self.yvel < -self.maxv) then
	    self.yvel = -self.maxv
		self.dvy = 0
	end
	if(self.xvel > self.maxv) then
	    self.xvel = self.maxv
		self.dvx = 0
	end
	if(self.xvel < -self.maxv) then
	    self.xvel = -self.maxv
		self.dvx = 0
	end

	self.xvel = self.xvel + self.dvx
	self.yvel = self.yvel + self.dvy
    self.x = self.x + self.xvel
	self.y = self.y + self.yvel
	self.yvel = self.yvel * .95
	self.xvel = self.xvel * .95
	self.dvy = 0
	self.dvx = 0
end

