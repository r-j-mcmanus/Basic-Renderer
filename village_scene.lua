return {
    name = "Village",
    children = {
        {
            type = "Player",
            position = { x = 0, y = 0 }
        },
        {
            type = "NPC_Elira",
            position = { x = 5, y = 2 }
        },
        {
            type = "House",
            name = "NorthHouse", -- override prefab name
            position = { x = -3, y = 0 }
        }
    }
}
