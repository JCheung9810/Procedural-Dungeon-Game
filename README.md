# Procedural-Dungeon-Game
A 1 player dungeon game where each playthrough features a uniquely generated dungeon. The player will be able to explore the dungeon, fight AI-controlled enemies, and progress through increasingly challenging areas.

# Implemented Features
- Random dungeon generation
  - Dead end generation
  - Room types and grid based generation
  - Boss room selection and generation
- Wall collision detection
- Room to room camera control
- 8 direction movement with animation
- Health
- Damage
- Enemy dummy
- Simple enemy ai
- Dodge/roll animation and functionality
- Death
- Different weapon types and functionality
- UI shows current weapon, magSize, bullets left, reload on counter
- LDTK level loading
- Room enemy encounter trigger

# To Do
- COMBAT
  - Better projectile hitbox 
  - Intermediate enemy AI
  - Enemy lead shooting
- UI
  - Reload mouse cursor 
  - Minimap/map 
  - Door animation
- MECHANICS 
  - Pickups (ammo,health) 
  - Breakable room objects
  - Usable/Interactable room objects
- OTHER
  - Lobby/Main menu 
  - Shop 

# Priority List (Highest to Lowest)
  - Intermediate enemy AI (state changing, different behaviors)
  - Enemy lead shooting (build in intelligence)
  - Make enemies spawn in the center of the spawn point (offset by size/2)
  - Add enemy getting hit effect (change their color to white (gun and entity))
  - Pickups (ammo,health)
  - Reload mouse cursor
  - Minimap/map
  - Shop
  - Lobby/main menu
  - Breakable room objects
  - Usable/Interactable room objects
  - Door animation
  
# Distant Features
- Better projectile hitbox (for guns that dont shoot circular projectiles)
- Advanced enemy AI (path finding)
- Free placed rooms instead of grid
- Different sized rooms

# Notes to Self
-change outline of gun box ui to be white
-change cursor to be silver/white