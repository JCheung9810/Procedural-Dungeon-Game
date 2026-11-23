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
- Map view
- Room clear rewards

# To Do Priority List (Highest to Lowest)
  - Special rooms (Shop)
  - Lobby/main menu
  - Black layer, change opacity over time when passed through door, 
  - Make door initially closed until walked through, door is not gate, gate locks player in, door is for exploration
  - Remove camera clamp, make camera positioned at some point between player and mouse
  - Reload mouse cursor
  - Reload bar above head
  
# Distant Features
- Add shields (temp health, always takes a full 1 hit no matter the damage)
- Item inventory
- Breakable room objects
- Usable/Interactable room objects
- Door animation
- Better projectile hitbox (for guns that dont shoot circular projectiles)
- Advanced enemy AI (path finding)
- Free placed rooms instead of grid
- Different sized rooms

# Notes to Self
- can shorten end room spawning with simple empty type check
- update isBoss etc with new code
- Fix usable bools and vals, make it cleaner
- Build special spawn chance and caps into dungeon.json
- Make chest drop weapons