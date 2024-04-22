cmake_minimum_required(VERSION 3.5)
project(sentry_behavior_tree)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

find_package(rclcpp REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(nav2_util REQUIRED)
find_package(sentry_interfaces REQUIRED)
find_package(sentry_srvs REQUIRED)

include_directories(include)

set(dependencies
  rclcpp
  behaviortree_cpp_v3
  nav2_util
  sentry_interfaces
  sentry_srvs
)

add_library(go_back_bt_node SHARED plugins/service/go_back_service.cpp)
list(APPEND plugin_libs go_back_bt_node)

add_library(rmuc_go_enemy_outpost_bt_node SHARED plugins/service/rmuc_go_enemy_outpost_service.cpp)
list(APPEND plugin_libs rmuc_go_enemy_outpost_bt_node)

add_library(rmuc_go_outpost_bt_node SHARED plugins/service/rmuc_go_outpost_service.cpp)
list(APPEND plugin_libs rmuc_go_outpost_bt_node)

add_library(rmuc_go_supply_bt_node SHARED plugins/service/rmuc_go_supply_service.cpp)
list(APPEND plugin_libs rmuc_go_supply_bt_node)

add_library(rmuc_go_left_bt_node SHARED plugins/service/rmuc_go_left_service.cpp)
list(APPEND plugin_libs rmuc_go_left_bt_node)

add_library(rmuc_go_right_bt_node SHARED plugins/service/rmuc_go_right_service.cpp)
list(APPEND plugin_libs rmuc_go_right_bt_node)

add_library(rmuc_is_save_bt_node SHARED plugins/service/rmuc_is_save_service.cpp)
list(APPEND plugin_libs rmuc_is_save_bt_node)

add_library(rmuc_base_patrol_bt_node SHARED plugins/service/rmuc_base_patrol_service.cpp)
list(APPEND plugin_libs rmuc_base_patrol_bt_node)

add_library(rmuc_patrol_bt_node SHARED plugins/service/rmuc_patrol_service.cpp)
list(APPEND plugin_libs rmuc_patrol_bt_node)

add_library(rmuc_center_patrol_bt_node SHARED plugins/service/rmuc_center_patrol_service.cpp)
list(APPEND plugin_libs rmuc_center_patrol_bt_node)

add_library(rmuc_enemy_area_patrol_bt_node SHARED plugins/service/rmuc_enemy_area_patrol_service.cpp)
list(APPEND plugin_libs rmuc_enemy_area_patrol_bt_node)

add_library(rmul_patrol_bt_node SHARED plugins/service/rmul_patrol_service.cpp)
list(APPEND plugin_libs rmul_patrol_bt_node)

add_library(rmul_patrol_conserv_bt_node SHARED plugins/service/rmul_patrol_conserv_service.cpp)
list(APPEND plugin_libs rmul_patrol_conserv_bt_node)

add_library(rmul_defense_bt_node SHARED plugins/service/rmul_defense_service.cpp)
list(APPEND plugin_libs rmul_defense_bt_node)

add_library(rmul_go_enemy_base_bt_node SHARED plugins/service/rmul_go_enemy_base_service.cpp)
list(APPEND plugin_libs rmul_go_enemy_base_bt_node)

add_library(rmul_go_our_base_bt_node SHARED plugins/service/rmul_go_our_base_service.cpp)
list(APPEND plugin_libs rmul_go_our_base_bt_node)

add_library(rmul_go_supply_bt_node SHARED plugins/service/rmul_go_supply_service.cpp)
list(APPEND plugin_libs rmul_go_supply_bt_node)

add_library(buy_bullets_bt_node SHARED plugins/service/buy_bullets_service.cpp)
list(APPEND plugin_libs buy_bullets_bt_node)

add_library(set_nav_target_bt_node SHARED plugins/service/set_nav_target_service.cpp)
list(APPEND plugin_libs set_nav_target_bt_node)

add_library(set_static_nav_target_bt_node SHARED plugins/service/set_static_nav_target_service.cpp)
list(APPEND plugin_libs set_static_nav_target_bt_node)

add_library(set_left_target_bt_node SHARED plugins/service/set_left_target_service.cpp)
list(APPEND plugin_libs set_left_target_bt_node)

add_library(set_right_target_bt_node SHARED plugins/service/set_right_target_service.cpp)
list(APPEND plugin_libs set_right_target_bt_node)

add_library(set_rmos_target_bt_node SHARED plugins/service/set_rmos_target_service.cpp)
list(APPEND plugin_libs set_rmos_target_bt_node)

# condition
add_library(air_force_condition_bt_node SHARED plugins/condition/air_force_condition.cpp)
list(APPEND plugin_libs air_force_condition_bt_node)

add_library(in_supply_condition_bt_node SHARED plugins/condition/in_supply_condition.cpp)
list(APPEND plugin_libs in_supply_condition_bt_node)

add_library(in_patrol_condition_bt_node SHARED plugins/condition/in_patrol_condition.cpp)
list(APPEND plugin_libs in_patrol_condition_bt_node)

add_library(base_unfolds_condition_bt_node SHARED plugins/condition/base_unfolds_condition.cpp)
list(APPEND plugin_libs base_unfolds_condition_bt_node)

add_library(base_failed_condition_bt_node SHARED plugins/condition/base_failed_condition.cpp)
list(APPEND plugin_libs base_failed_condition_bt_node)

add_library(base_winned_condition_bt_node SHARED plugins/condition/base_winned_condition.cpp)
list(APPEND plugin_libs base_winned_condition_bt_node)

add_library(can_buy_bullets_condition_bt_node SHARED plugins/condition/can_buy_bullets_condition.cpp)
list(APPEND plugin_libs can_buy_bullets_condition_bt_node)

add_library(center_patrol_condition_bt_node SHARED plugins/condition/center_patrol_condition.cpp)
list(APPEND plugin_libs center_patrol_condition_bt_node)

add_library(counter_outpost_condition_bt_node SHARED plugins/condition/counter_outpost_condition.cpp)
list(APPEND plugin_libs counter_outpost_condition_bt_node)

add_library(enemy_area_patrol_condition_bt_node SHARED plugins/condition/enemy_area_patrol_condition.cpp)
list(APPEND plugin_libs enemy_area_patrol_condition_bt_node)

add_library(game_start_condition_bt_node SHARED plugins/condition/game_start_condition.cpp)
list(APPEND plugin_libs game_start_condition_bt_node)

add_library(game_about_over_condition_bt_node SHARED plugins/condition/game_about_over_condition.cpp)
list(APPEND plugin_libs game_about_over_condition_bt_node)

add_library(have_target_condition_bt_node SHARED plugins/condition/have_target_condition.cpp)
list(APPEND plugin_libs have_target_condition_bt_node)

add_library(low_hp_condition_bt_node SHARED plugins/condition/low_hp_condition.cpp)
list(APPEND plugin_libs low_hp_condition_bt_node)

add_library(low_bullets_condition_bt_node SHARED plugins/condition/low_bullets_condition.cpp)
list(APPEND plugin_libs low_bullets_condition_bt_node)

add_library(leave_too_long_condition_bt_node SHARED plugins/condition/leave_too_long_condition.cpp)
list(APPEND plugin_libs leave_too_long_condition_bt_node)

add_library(enough_hp_condition_bt_node SHARED plugins/condition/enough_hp_condition.cpp)
list(APPEND plugin_libs enough_hp_condition_bt_node)

add_library(force_back_condition_bt_node SHARED plugins/condition/force_back_condition.cpp)
list(APPEND plugin_libs force_back_condition_bt_node)

add_library(need_unlock_condition_bt_node SHARED plugins/condition/need_unlock_condition.cpp)
list(APPEND plugin_libs need_unlock_condition_bt_node)

add_library(outpost_low_hp_condition_bt_node SHARED plugins/condition/outpost_low_hp_condition.cpp)
list(APPEND plugin_libs outpost_low_hp_condition_bt_node)

add_library(enemy_outpost_survives_condition_bt_node SHARED plugins/condition/enemy_outpost_survives_condition.cpp)
list(APPEND plugin_libs enemy_outpost_survives_condition_bt_node)

add_library(outpost_survives_condition_bt_node SHARED plugins/condition/outpost_survives_condition.cpp)
list(APPEND plugin_libs outpost_survives_condition_bt_node)

add_library(target_can_follow_condition_bt_node SHARED plugins/condition/target_can_follow_condition.cpp)
list(APPEND plugin_libs target_can_follow_condition_bt_node)

# rmul
add_library(rmul_can_supply_condition_bt_node SHARED plugins/condition/rmul_can_supply_condition.cpp)
list(APPEND plugin_libs rmul_can_supply_condition_bt_node)

foreach(bt_plugin ${plugin_libs})
  ament_target_dependencies(${bt_plugin} ${dependencies} )
  target_compile_definitions(${bt_plugin} PRIVATE BT_PLUGIN_EXPORT)
endforeach()

install(TARGETS ${plugin_libs}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include/
)

install(FILES sentry_tree_nodes.xml DESTINATION share/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(
  include
)

ament_export_libraries(
  ${plugin_libs}
)

ament_export_dependencies(${dependencies})

ament_package()
