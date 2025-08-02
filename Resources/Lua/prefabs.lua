return {
    NPC_Elira = {
        type = "NPC",
        name = "Elira",
        components = {
            Sprite = { texture = "elira.png" },
            Dialogue = { script = "fetch_quest.lua" },
            Collider = {}
        }
    },

    House = {
        type = "Building",
        components = {
            Sprite = { texture = "house.png" },
            Collider = { width = 2, height = 2 }
        }
    },

    Player = {
        type = "Player",
        name = "Hero",
        components = {
            Sprite = { texture = "player.png" },
            Input = {},
            Collider = {}
        }
    }
}
