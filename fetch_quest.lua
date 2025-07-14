-- Quest State

states = {
    not_started = 0
    in_progress = 1
    complete = 2
}

quest = {
    id = "herb_for_elira",
    name = "Herb for Elira",
    description = "Find the magic herb and return it to Elira in the village.",
    state = states.not_started
}



-- Called when the player talks to Elira
function onTalkToElira(player)
    if quest.state ==states.not_started then
        quest.state = states.in_progress
        return "Hello! Can you find a magic herb for me? It grows in the forest."
    elseif quest.state == states.in_progress then
        if player:hasItem("magic_herb") then
            player:removeItem("magic_herb")
            player:addGold(50)
            quest.state = states.complete
            return "You brought the herb! Here's 50 gold as a thank-you."
        else
            return "Still no herb? Please hurry!"
        end
    elseif quest.state == states.complete then
        return "Thank you again for the herb!"
    end
end

-- Function to get the current state (optional for saving)
function getQuestState()
    return quest.state
end

-- Function to load state from C++ side (optional for loading saved progress)
function setQuestState(state)
    quest.state = state
end
