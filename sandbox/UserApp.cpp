#include "Application.hpp"

#include <random>
// TODO: Insert the callback function here
std::vector<std::tuple<noob::keyboard::keys, noob::keyboard::mod_keys, std::string>> keystrokes;

void noob::application::user_init()
{
	view_mat = noob::look_at(noob::vec3(0.0, 100.0, -100.0), noob::vec3(0.0, 0.0, 0.0), noob::vec3(0.0, 1.0, 0.0)); //look_at(const vec3& cam_pos, vec3 targ_pos, const vec3& up)
	noob::triplanar_gradient_map_renderer::uniform_info moon_shader;
	moon_shader.colours[0] = noob::vec4(1.0, 1.0, 1.0, 1.0);
	moon_shader.colours[1] = noob::vec4(0.0, 0.0, 0.0, 1.0);
	moon_shader.colours[2] = noob::vec4(0.0, 0.0, 0.0, 1.0);
	moon_shader.colours[3] = noob::vec4(0.0, 0.0, 0.0, 1.0);
	moon_shader.mapping_blends = noob::vec3(0.0, 0.0, 1.0);
	moon_shader.scales = noob::vec3(1.0/10.0, 1.0/10.0, 1.0/10.0);
	moon_shader.colour_positions = noob::vec2(0.4, 0.6);
	// moon_shader.scales = noob::vec3(1.0/100.0, 1.0/100.0, 1.0/100.0);

	stage.set_shader(moon_shader, "moon");
	
	// auto actor_id = stage.actor(stage.basic_models.get_handle("unit-sphere"), stage.skeletons.get_handle("null"), noob::vec3(0.0, 80.0, 0.0));

	// This shader isn't really blue, but bear with me :P
	noob::triplanar_gradient_map_renderer::uniform_info purple_shader;
	purple_shader.colours[0] = noob::vec4(1.0, 1.0, 1.0, 1.0);
	purple_shader.colours[1] = noob::vec4(1.0, 0.0, 1.0, 1.0);
	purple_shader.colours[2] = noob::vec4(1.0, 0.0, 1.0, 1.0);
	purple_shader.colours[3] = noob::vec4(0.0, 0.0, 1.0, 1.0);
	purple_shader.mapping_blends = noob::vec3(0.2, 0.0, 0.5);
	// purple_shader.scales = noob::vec3(1.0,1.0,1.0);
	// purple_shader.scales = noob::vec3(1.0, 1.0, 1.0);
	// purple_shader.scales = noob::vec3(1.0, 1.0, 1.0);
	purple_shader.scales = noob::vec3(1.0/100.0, 1.0/100.0, 1.0/100.0);
	purple_shader.colour_positions = noob::vec2(0.2, 0.7);
	stage.set_shader(purple_shader, "purple");

	// Make a basic scenery
	noob::basic_mesh temp_1 = noob::mesh_utils::box(100.0, 20.0, 100.0);
	// temp_1.translate(noob::vec3(0.0, 10.0, 0.0));
	noob::basic_mesh temp_2 = noob::mesh_utils::cone(50.0, 100.0);
	temp_1.translate(noob::vec3(0.0, 10.0, 0.0));
	noob::basic_mesh scene_mesh = noob::mesh_utils::csg(temp_1, temp_2, noob::csg_op::UNION);
	
	auto scenery_h = stage.scenery(stage.add_mesh(scene_mesh), noob::vec3(0.0, 0.0, 0.0), "moon");

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-10.0, 10.0);

	for (size_t i = 0 ; i < 500; ++i )
	{
		// std::vector<noob::vec3> points;
		// for (size_t k = 0; k < 5; ++k)
		// {	
		// 	points.push_back(noob::vec3(dis(gen), dis(gen), dis(gen)));
		// }
		// auto h = stage.hull(points);

		auto h = stage.box(2.0, 2.0, 2.0); 
		auto temp_body = stage.body(noob::body_type::DYNAMIC, h, 1.0, noob::vec3(dis(gen), 250.0, dis(gen)), noob::versor(0.0, 0.0, 0.0, 1.0)); //, true);

		stage.bodies.get(temp_body); //->set_self_controlled(false);
		stage.prop(temp_body, "purple");
	}

	keystrokes.push_back(std::make_tuple(noob::keyboard::keys::NUM_5, noob::keyboard::mod_keys::NONE, "switch view"));
}


void noob::application::user_update(double dt)
{
	gui.text("Noobwerkz Editor", 50.0, 50.0, noob::gui::font_size::HEADER);
	// player_character->move(true, false, false, false, true);
	// logger::log(player_character->get_debug_info());
	fmt::MemoryWriter ww;
	for (auto k : keystrokes)
	{
		ww << std::get<2>(k) << ": " << noob::keyboard::to_string(std::get<1>(k));
		if (noob::keyboard::to_string(std::get<1>(k)) != "")
		{
			ww << " ";
		}
		ww << noob::keyboard::to_string(std::get<0>(k)) << "\n";
	}

	gui.text(ww.str(), static_cast<float>(window_width - 500), static_cast<float>(window_height - 50), noob::gui::font_size::HEADER);
}
