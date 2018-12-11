
-- Helper function for getting table length
function tablelength(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

-- local table variable for words to pull from
words = {
 {"potato", "egg", "salad", "tomato", "bisque", "yogurt", "bacon"},
 {"green", "yellow", "red"},
 {"square", "triangle", "circle", "pentagon"}
}

-- local variables specific to the script
setIndex = nil
listCount = nil
tick = 0
tickToAdd = 50
lengthToReset = 130
uiTextIndex = nil

--start function to be invoked
function start ()
	entity:setName("Speech Box")
	entity:setSize(980.0, 100.0)
	entity:setPos(20.0, 780.0 - 120.0)
	
	-- limit box creation to first ui window
	if (entity:getCount() == 1) then
		entity:addButton("Create Vertical Box", "verticalBoxButtonCallback")
		entity:addButton("Create Horizontal Box", "horizontalBoxButtonCallback")
		entity:setShouldResizeToFitContents(false)
	end
	
	-- any window and create more windows
	entity:addButton("Create new window (can have up to 20)", "createNewWindowButtonCallback")
	
	-- only let ui windows after the first destroy itself
	if (entity:getCount() > 1) then
		entity:addButton("Destroy this window", "destroyWindowButtonCallback")
		entity:setShouldResizeToFitContents(true)
	end
	
	-- set up text stuff
	local setCount = tablelength(words)
	setIndex = math.random(setCount)
	listCount = tablelength(words[setIndex])
	uiTextIndex = entity:addText(words[setIndex][math.random(listCount)])
end

--update funciton to be invoked
function update ()

	-- basic update timer tick 
	tick = tick + 1
	if tick > tickToAdd then
		tick = 0
		
		-- set or reset text
		currText = entity:getUIElementText(uiTextIndex)
		local listIndex = math.random(listCount)
		if string.len(currText) > lengthToReset then
			entity:setUIElementText(uiTextIndex, words[setIndex][listIndex])
		else
			entity:setUIElementText(uiTextIndex, string.format("%s %s", currText, words[setIndex][listIndex]))
		end
	end
end

--call back for hitting vertical button
function verticalBoxButtonCallback()
	verticalBox.new("verticalBox", true)
end

--call back for hitting horizontal button
function horizontalBoxButtonCallback()
	horizontalBox.new("horizontalBox", true)
end

-- callback for creating another speech box
function createNewWindowButtonCallback()
	if entity:getCount() < 20 then
		c = uiSpeechBox.new("speechBox", true)
		entity:setPos(entity:getPosX() + 10, entity:getPosY() - 10)
		c:setPos(entity:getPosX(), entity:getPosY())
	end
end

-- callback for destroying hitting the destroy button
function destroyWindowButtonCallback()
	if entity:getCount() > 1 then
		entity:destroy()
	end
end
