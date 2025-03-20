#include "levelgenerator.h"

#include "editor/game.pb.h"

#include <fstream>
#include <iostream>

using namespace game;

void GenerateTestLevel(std::string filepath) {
    // Создаем экземпляр Level
    Level level;

    // Заполняем основные поля Level
    level.set_name("Test Level");
    level.set_version("1.0");
    level.set_description("This is a test level for the game.");

    // Заполняем Background
    Background* background = level.mutable_background();
    background->set_model_3d("background.glb");
    background->set_background_script("background_script.cpp");

    // Добавляем три ShaderPair
    ShaderPair* shader1 = background->add_shaders();
    shader1->set_fragment_shader("shader1.frag");
    shader1->set_vertex_shader("shader1.vert");

    ShaderPair* shader2 = background->add_shaders();
    shader2->set_fragment_shader("shader2.frag");
    shader2->set_vertex_shader("shader2.vert");

    ShaderPair* shader3 = background->add_shaders();
    shader3->set_fragment_shader("shader3.frag");
    shader3->set_vertex_shader("shader3.vert");

    // Заполняем LevelScript
    LevelScript* level_script = level.mutable_level_script();
    level_script->set_script("level_script.cpp");

    // Добавляем три MobBatch
    MobBatch* batch1 = level.add_mob_batches();
    batch1->set_mob_batch_id("batch1");
    batch1->add_mob_ids("mob1");
    batch1->add_mob_ids("mob2");
    batch1->set_delay_after_previous_batch(1.0f);
    batch1->set_lifespan(10.0f);

    MobBatch* batch2 = level.add_mob_batches();
    batch2->set_mob_batch_id("batch2");
    batch2->add_mob_ids("mob3");
    batch2->add_mob_ids("mob4");
    batch2->set_delay_after_previous_batch(2.0f);
    batch2->set_lifespan(15.0f);

    MobBatch* batch3 = level.add_mob_batches();
    batch3->set_mob_batch_id("batch3");
    batch3->add_mob_ids("mob5");
    batch3->add_mob_ids("mob6");
    batch3->set_delay_after_previous_batch(3.0f);
    batch3->set_lifespan(20.0f);

    // Добавляем три Mob
    Mob* mob1 = level.add_mobs();
    mob1->set_mob_id("mob1");
    mob1->set_health(100.0f);
    mob1->set_sprite("mob1.png");
    mob1->set_spawn_delay(0.0f);

    Path* path1 = mob1->mutable_path();
    Point* point1 = path1->add_key_points();
    point1->set_x(0.0f);
    point1->set_y(0.0f);
    Point* point2 = path1->add_key_points();
    point2->set_x(1.0f);
    point2->set_y(1.0f);
    path1->set_travel_time(5.0f);

    Drop* drop1 = mob1->mutable_drop();
    drop1->set_item("large P");
    drop1->set_drop_weight(1.0f);

    mob1->set_attack_id("attack1");

    Collider* collider1 = mob1->mutable_collider();
    collider1->set_offset_x(0.0f);
    collider1->set_offset_y(0.0f);
    collider1->mutable_circle_collider()->set_radius(1.0f);

    Mob* mob2 = level.add_mobs();
    mob2->set_mob_id("mob2");
    mob2->set_health(150.0f);
    mob2->set_sprite("mob2.png");
    mob2->set_spawn_delay(1.0f);

    Path* path2 = mob2->mutable_path();
    Point* point3 = path2->add_key_points();
    point3->set_x(0.0f);
    point3->set_y(0.0f);
    Point* point4 = path2->add_key_points();
    point4->set_x(2.0f);
    point4->set_y(2.0f);
    path2->set_travel_time(10.0f);

    Drop* drop2 = mob2->mutable_drop();
    drop2->set_item("P");
    drop2->set_drop_weight(0.5f);

    mob2->set_attack_id("attack2");

    Collider* collider2 = mob2->mutable_collider();
    collider2->set_offset_x(0.0f);
    collider2->set_offset_y(0.0f);
    collider2->mutable_rectangle_collider()->set_width(2.0f);
    collider2->mutable_rectangle_collider()->set_height(2.0f);

    Mob* mob3 = level.add_mobs();
    mob3->set_mob_id("mob3");
    mob3->set_health(200.0f);
    mob3->set_sprite("mob3.png");
    mob3->set_spawn_delay(2.0f);

    Path* path3 = mob3->mutable_path();
    Point* point5 = path3->add_key_points();
    point5->set_x(0.0f);
    point5->set_y(0.0f);
    Point* point6 = path3->add_key_points();
    point6->set_x(3.0f);
    point6->set_y(3.0f);
    path3->set_travel_time(15.0f);

    Drop* drop3 = mob3->mutable_drop();
    drop3->set_item("points");
    drop3->set_drop_weight(0.2f);

    mob3->set_attack_id("attack3");

    Collider* collider3 = mob3->mutable_collider();
    collider3->set_offset_x(0.0f);
    collider3->set_offset_y(0.0f);
    collider3->mutable_circle_collider()->set_radius(1.5f);

    // Добавляем три Attack
    Attack* attack1 = level.add_attacks();
    attack1->set_attack_id("attack1");
    attack1->set_bullet_id("bullet1");

    Attack* attack2 = level.add_attacks();
    attack2->set_attack_id("attack2");
    attack2->set_bullet_id("bullet2");

    Attack* attack3 = level.add_attacks();
    attack3->set_attack_id("attack3");
    attack3->set_bullet_id("bullet3");

    // Добавляем три Boss
    Boss* boss1 = level.add_bosses();
    boss1->set_boss_id("boss1");
    boss1->set_finally(false);
    boss1->set_batch_id("batch1");
    boss1->set_drop_item("bombs");

    Collider* boss_collider1 = boss1->mutable_collider();
    boss_collider1->set_offset_x(0.0f);
    boss_collider1->set_offset_y(0.0f);
    boss_collider1->mutable_circle_collider()->set_radius(5.0f);

    Phase* phase1 = boss1->add_phases();
    phase1->add_attacks()->set_attack_id("attack1");
    phase1->set_health(1000.0f);
    phase1->set_spell_card_id("spell_card1");

    Boss* boss2 = level.add_bosses();
    boss2->set_boss_id("boss2");
    boss2->set_finally(false);
    boss2->set_batch_id("batch2");
    boss2->set_drop_item("1UPs");

    Collider* boss_collider2 = boss2->mutable_collider();
    boss_collider2->set_offset_x(0.0f);
    boss_collider2->set_offset_y(0.0f);
    boss_collider2->mutable_rectangle_collider()->set_width(10.0f);
    boss_collider2->mutable_rectangle_collider()->set_height(10.0f);

    Phase* phase2 = boss2->add_phases();
    phase2->add_attacks()->set_attack_id("attack2");
    phase2->set_health(1500.0f);
    phase2->set_spell_card_id("spell_card2");

    Boss* boss3 = level.add_bosses();
    boss3->set_boss_id("boss3");
    boss3->set_finally(true);
    boss3->set_batch_id("batch3");
    boss3->set_drop_item("bombs");

    Collider* boss_collider3 = boss3->mutable_collider();
    boss_collider3->set_offset_x(0.0f);
    boss_collider3->set_offset_y(0.0f);
    boss_collider3->mutable_circle_collider()->set_radius(7.0f);

    Phase* phase3 = boss3->add_phases();
    phase3->add_attacks()->set_attack_id("attack3");
    phase3->set_health(2000.0f);
    phase3->set_spell_card_id("spell_card3");

    // Добавляем три Bullet
    Bullet* bullet1 = level.add_bullets();
    bullet1->set_bullet_id("bullet1");
    bullet1->set_damage(10.0f);

    Collider* bullet_collider1 = bullet1->mutable_collider();
    bullet_collider1->set_offset_x(0.0f);
    bullet_collider1->set_offset_y(0.0f);
    bullet_collider1->mutable_circle_collider()->set_radius(0.5f);

    Bullet* bullet2 = level.add_bullets();
    bullet2->set_bullet_id("bullet2");
    bullet2->set_damage(20.0f);

    Collider* bullet_collider2 = bullet2->mutable_collider();
    bullet_collider2->set_offset_x(0.0f);
    bullet_collider2->set_offset_y(0.0f);
    bullet_collider2->mutable_rectangle_collider()->set_width(1.0f);
    bullet_collider2->mutable_rectangle_collider()->set_height(1.0f);

    Bullet* bullet3 = level.add_bullets();
    bullet3->set_bullet_id("bullet3");
    bullet3->set_damage(30.0f);

    Collider* bullet_collider3 = bullet3->mutable_collider();
    bullet_collider3->set_offset_x(0.0f);
    bullet_collider3->set_offset_y(0.0f);
    bullet_collider3->mutable_circle_collider()->set_radius(0.75f);

    std::ofstream output_file(filepath);
    level.SerializeToOstream(&output_file);
    output_file.close();

    // Выводим результат
    std::cout << "Test Level created successfully!" << std::endl;
}
