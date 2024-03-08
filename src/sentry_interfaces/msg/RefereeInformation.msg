#game_start
bool game_start
#robot hp
uint32 red_outpost_hp
uint32 blue_outpost_hp
#map point 
geometry_msgs/Point map_point
#client point
geometry_msgs/Point client_point
#force autoaim
bool force_autoaim
#tactics:outpost first/ignore outpost...
uint32 tactic
#forced change isInvincible  0根据读取前哨站判断 1强制无敌 2强制不无敌（开始小陀螺）
uint32 invincible_mode