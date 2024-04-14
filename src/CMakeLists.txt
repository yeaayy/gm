
target_sources(run PUBLIC
    renderer/renderable_object.cpp
    renderer/renderable_townie.cpp

    texture/animated_sprite.cpp
    texture/atlas.cpp
    texture/renderable_sprite.cpp
    texture/simple_sprite.cpp
    texture/sprite.cpp

    building/abs_house.cpp
    building/building.cpp
    building/castle.cpp
    building/lumberjack.cpp
    building/production.cpp
    building/small_house.cpp
    building/storage.cpp
    building/wood.cpp

    # game/build_menu.cpp
    game/enterable.cpp
    game/game_view.cpp
    game/object.cpp
    game/storage.cpp
    game/summer_world_renderer.cpp
    game/townie.cpp
    game/world_data.cpp
    game/world_editor.cpp
    game/world_renderer.cpp

    townie/action_complain.cpp
    townie/action_drop.cpp
    townie/action_hide.cpp
    townie/action_pickup.cpp
    townie/action_rest.cpp
    townie/action_routine.cpp
    townie/action_start_work.cpp
    townie/action_wait.cpp
    townie/action_walk.cpp
    townie/action_work.cpp
    townie/carrier_work.cpp
    townie/lumberjack_work.cpp

    dev/bulding_test.cpp
    dev/rand_walk.cpp
    dev/simple_world_renderer.cpp
    dev/storage_view.cpp
    dev/townie_action_carry.cpp
    dev/townie_action_wait.cpp
    dev/townie_action_walk.cpp
    dev/townie_controller.cpp
    dev/world.cpp

    main.cpp
    # dev/test_texture.cpp
    # dev/test_pfa.cpp
)

target_include_directories(run PUBLIC .)

target_link_libraries(run PRIVATE math)
target_link_libraries(run PRIVATE utils)
target_link_libraries(run PRIVATE glview)
target_link_libraries(run PRIVATE pfa)
target_link_libraries(run PRIVATE -ltinyxml2)