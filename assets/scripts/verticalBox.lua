
-- vars
moveLeft = true
edge = 4
increment = 0.1
child = nil

--start func
function start ()
	
	-- set up vars
	entity:setName("Basic Tank")
	entity:setMaterial("tankMat")
	entity:setMesh("tankMesh")
	
	-- variance
	delta = math.random() * 3.0
	if math.random() < 0.5 then
		delta = -delta
	end
	
	r = math.random() * (0.1)
	if math.random() > 0.5 then
		r = -r
	end
	
	-- more setting vars
	entity:setPosition(entity:getPositionX() + delta, entity:getRotationY() + delta, entity:getPositionZ())
	entity:setScale(0.25 + r, 0.25 + r, 0.25 + r)
	entity:setRotation(0, 1, 0)
	entity:setAlbedo( math.random()* 0.6 + 0.4, math.random()* 0.6 + 0.4, 0 )
end

-- update func
function update ()	
	--get pos
	posX = entity:getPositionX()
	
	-- move left or right
	if moveLeft then
		posX = posX + increment
		if posX > edge then
			moveLeft = false
			increment = increment / 2
		end
	else 
		posX = posX - increment
		if posX < -edge then
			moveLeft = true
			increment = increment * 2
		end
	end
	
	--make it fancy and apply
	posY = math.sin(posX) 
	entity:setPosition(posX, posY, entity:getPositionZ())
	entity:setRotation(posX, entity:getRotationY(), entity:getRotationZ())
end
