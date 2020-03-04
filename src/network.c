// TODO





// -- NETWORK:
// request: attack(coord_y, coord_x)
// response: { name of hit ship, coord of hit/miss, whether it was sunk or not }
// When the attacking client C1 sends an attack message to the attacked client C2:
// - C1 checks from local radar that the attack has not already been tried, ensuring a valid attack
// - C2 checks if any ships were hit (skipping already sunk ones)
//   - If HIT, C2 checks
//     - if already hit, if it is print an error since that shouldn't happen
//     - if not, update C2 ship collection data, check if ship was sunk, collect all that data, then send a response message to C1
//   - If MISS, C2
//     - sends a response message to C1
