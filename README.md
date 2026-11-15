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

# To Do Priority List (Highest to Lowest)
  - Remove stored paths and just load textures
  - Enemy lead shooting (build in intelligence)
  - Make AttackState for player (shooting, reloading, idle)
  - Make enemies spawn in the center of the spawn point (offset by size/2)
  - Add enemy getting hit effect (change their color to white (gun and entity))
  - Pickups (ammo,health)
  - Make a boss entrance
  - Minimap/map
  - Shop
  - Lobby/main menu
  - Change outline of gun box ui to be white
  - Change cursor to be silver/white
  - Black layer, change opacity over time when passed through door, 
  - Make door initially closed until walked through, door is not gate, gate locks player in, door is for exploration
  - Reload mouse cursor
  - Breakable room objects
  - Usable/Interactable room objects
  - Door animation
  
# Distant Features
- Better projectile hitbox (for guns that dont shoot circular projectiles)
- Advanced enemy AI (path finding)
- Free placed rooms instead of grid
- Different sized rooms

# Notes to Self