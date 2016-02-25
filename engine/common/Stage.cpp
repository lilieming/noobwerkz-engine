#include "Stage.hpp"
#include <fstream>
#include <boost/filesystem.hpp>


noob::stage::~stage()
{
	delete dynamics_world;
	delete solver;
	delete collision_configuration;
	delete collision_dispatcher;
	delete broadphase;
}


void noob::stage::init()
{
	broadphase = new btDbvtBroadphase();
	collision_configuration = new btDefaultCollisionConfiguration();
	collision_dispatcher = new btCollisionDispatcher(collision_configuration);
	solver = new btSequentialImpulseConstraintSolver();
	dynamics_world = new btDiscreteDynamicsWorld(collision_dispatcher, broadphase, solver, collision_configuration);
	dynamics_world->setGravity(btVector3(0, -10, 0));

	renderer.init();

	// draw_graph.reserveNode(NUM_RESERVED_NODES);
	// draw_graph.reserveArc(NUM_RESERVED_ARCS);

	root_node = draw_graph.addNode();

	noob::bodies_holder::handle temp = body(noob::body_type::STATIC, globals::unit_sphere_shape, 0.0, noob::vec3(0.0, 0.0, 0.0), noob::versor(0.0, 0.0, 0.0, 1.0), false);

	fmt::MemoryWriter ww;
	ww << "[Stage] init first body. Handle = " << temp.get_inner();
	logger::log(ww.str());

	logger::log("[Stage] Done init.");
}


// TODO: Implement
void noob::stage::tear_down()
{
	bodies.empty();
	// bodies_to_shapes.empty();
}


void noob::stage::update(double dt)
{
	dynamics_world->stepSimulation(1.0/60.0, 10);
}


void noob::stage::draw(float window_width, float window_height) const
{

	bgfx::setViewTransform(0, &view_mat.m[0], &projection_mat.m[0]);
	bgfx::setViewRect(0, 0, 0, window_width, window_height);
	bgfx::setViewTransform(1, &view_mat.m[0], &projection_mat.m[0]);
	bgfx::setViewRect(1, 0, 0, window_width, window_height);

	for (lemon::ListDigraph::OutArcIt model_it(draw_graph, root_node); model_it != lemon::INVALID; ++model_it)
	{
		lemon::ListDigraph::Node model_node = draw_graph.target(model_it);
		size_t model_h = basic_models_mapping[model_node];

		// fmt::MemoryWriter w;
		// w << "[Stage] draw() - Iterating over model " << model_h;
		// logger::log(w.str());

		// if (lemon::countOutArcs(draw_graph, model_node) > 0)
		// {
		for (lemon::ListDigraph::OutArcIt shading_it(draw_graph, model_node); shading_it != lemon::INVALID; ++shading_it)
		{
			lemon::ListDigraph::Node shading_node = draw_graph.target(shading_it);
			size_t shader_h = shaders_mapping[shading_node];

			// fmt::MemoryWriter ww;
			// ww << "[Stage] draw() - Iterating over shader " << shader_h;
			// logger::log(ww.str());

			// if (lemon::countOutArcs(draw_graph, shading_node) > 0)
			// {
			for (lemon::ListDigraph::OutArcIt body_it(draw_graph, shading_node); body_it != lemon::INVALID; ++body_it)
			{
				lemon::ListDigraph::Node body_node = draw_graph.target(body_it);
				size_t body_h = bodies_mapping[body_node];

				// fmt::MemoryWriter www;
				// www << "[Stage] draw() - model " << model_h << " shading " << shader_h << " body " << body_h;
				// logger::log(www.str());

				noob::body* body_ptr = bodies.get(bodies.make_handle(body_h));
				mat4 world_mat = bodies.get(bodies.make_handle(body_h))->get_transform();
				noob::mat4 normal_mat = noob::transpose(noob::inverse((view_mat * world_mat)));
				renderer.draw(noob::globals::basic_models.get(noob::globals::basic_models.make_handle(model_h)), noob::globals::shaders.get(noob::globals::shaders.make_handle(shader_h)), world_mat, normal_mat, basic_lights, 0);
			}
			// }
		}
		// }
	}

	if (show_origin)
	{
		// noob::mat4 world_mat = noob::scale(noob::identity_mat4(), shapes.get(bodies_to_shapes[b.get_inner()])->get_scales());
		// world_mat = bodies.get(b)->get_transform() * world_mat;
		// renderer.draw(basic_models.get(m), shaders.get(s), world_mat);
		// bgfx::setViewTransform(view_id, view_matrix, ortho);
		// bgfx::setViewRect(view_id, 0, 0, window_width, window_height);
		noob::mat4 normal_mat = noob::identity_mat4();

		// renderer.draw(basic_models.get(unit_cube_model), shaders.get(debug_shader), noob::scale(noob::identity_mat4(), noob::vec3(10.0, 10.0, 10.0)), normal_mat, basic_lights);
	}
}


noob::bodies_holder::handle noob::stage::body(const noob::body_type b_type, const noob::shapes_holder::handle shape_h, float mass, const noob::vec3& pos, const noob::versor& orient, bool ccd)
{
	std::unique_ptr<noob::body> b = std::make_unique<noob::body>();
	b->init(dynamics_world, b_type, globals::shapes.get(shape_h), mass, pos, orient, ccd);
	return bodies.add(std::move(b));
	// bodies_to_shapes.insert(std::make_pair(bod_h.get_inner(), shape_h));
	// return bod_h;
}


void noob::stage::actor(const noob::bodies_holder::handle body_h, const noob::animated_models_holder::handle model_h, const noob::shaders_holder::handle shader_h)
{

}


void noob::stage::actor(const noob::bodies_holder::handle body_h, const noob::globals::scaled_model& model_info, const noob::shaders_holder::handle shader_h)
{
	auto body_results = bodies_to_nodes.find(body_h.get_inner());
	if (body_results != bodies_to_nodes.end())
	{
		logger::log("[Stage] Warning: Attempting to use duplicate body. Aborted.");
		return;
	}

	lemon::ListDigraph::Node model_node, shader_node, body_node;

	// auto model_results = basic_models_to_nodes.find(model_info.model_h.get_inner());
	// if (model_results != basic_models_to_nodes.end())
	// {
	// 	model_node = model_results->second;
	// }
	// else
	// {
	model_node = draw_graph.addNode();
	basic_models_mapping[model_node] = model_info.model_h.get_inner();

	fmt::MemoryWriter ww_0;
	ww_0 << "[Stage] Creating actor - model " << model_info.model_h.get_inner() << " not found in drawgraph. Creating node and connecting to root node.";
	logger::log(ww_0.str());

	basic_models_to_nodes.insert(std::make_pair(model_info.model_h.get_inner(), model_node));
	// }
	draw_graph.addArc(root_node, model_node);

	// auto shader_results = shaders_to_nodes.find(shader_h.get_inner());
	// if (shader_results != shaders_to_nodes.end())
	// {
	// 	shader_node = shader_results->second;
	// }
	// else
	// {
	shader_node = draw_graph.addNode();
	shaders_mapping[shader_node] = shader_h.get_inner();

	fmt::MemoryWriter ww_1;
	ww_1 << "[Stage] Creating actor - shader " << shader_h.get_inner() << " not found in drawgraph. Creating node and connecting to model node " << model_info.model_h.get_inner();
	logger::log(ww_1.str());

	shaders_to_nodes.insert(std::make_pair(shader_h.get_inner(), shader_node));
	// }
	draw_graph.addArc(model_node, shader_node);

	fmt::MemoryWriter ww_2;
	ww_2 << "[Stage] Creating actor - body " << body_h.get_inner() << " Creating node and connecting to shader node " << shader_h.get_inner();
	logger::log(ww_2.str());

	body_node = draw_graph.addNode();
	bodies_mapping[body_node] = body_h.get_inner();
	draw_graph.addArc(shader_node, body_node);

	bodies_to_nodes.insert(std::make_pair(body_h.get_inner(), body_node));

	fmt::MemoryWriter ww_3;
	ww_3 << "[Stage] Created actor - shader " << shader_h.get_inner() << ", model " << model_info.model_h.get_inner() << " , body " << body_h.get_inner(); 
	logger::log(ww_3.str());
}


void noob::stage::scenery(const noob::basic_mesh& m, const noob::vec3& pos, const noob::versor& orient, const noob::shaders_holder::handle shader_h, const std::string& name)
{
	noob::shapes_holder::handle shape_h = globals::static_trimesh(m, name);
	noob::bodies_holder::handle body_h = body(noob::body_type::STATIC, shape_h, 0.0, pos, orient);
	noob::globals::scaled_model model_info;
	model_info.model_h = noob::globals::basic_model(m);
	actor(body_h, model_info, shader_h);
}


void noob::stage::set_basic_light(unsigned int i, const noob::vec4& light)
{
	if (i > basic_lights.size()) basic_lights[basic_lights.size()-1] = light;
	else basic_lights[i] = light;
}


noob::vec4 noob::stage::get_basic_light(unsigned int i) const
{
	if (i > 1) return basic_lights[1];
	else return basic_lights[i];
}
///ListDigraph digraph;
///ListDigraph::ArcMap<int> cap(digraph);
///ListDigraph::Node src, trg;
///  // Setting the capacity map and source and target nodes
///digraphWriter(digraph, std::cout).
///  arcMap("capacity", cap).
///  node("source", src).
///  node("target", trg).
///  run();
void noob::stage::write_graph(const std::string& filename) const
{
	logger::log("About to write graph");
	boost::filesystem::path p("temp/");
	if (boost::filesystem::exists(p))
	{
		// p /= boost::filesystem::path(filename.c_str());
		
		// std::ofstream of;
		//of.open(p.generic_string(), std::ofstream::out | std::ofstream::trunc);
		//if (of)
		//{
		std::string full_path_str = p.generic_string() + filename;
		lemon::digraphWriter(draw_graph, full_path_str).nodeMap("model", basic_models_mapping).nodeMap("body", bodies_mapping).nodeMap("shader", shaders_mapping).node("root", root_node).run();

		//}
		//else
		//{
		//	fmt::MemoryWriter ww;
		//	ww << "[Stage] Could not write graph snapshot. Failed to open " << p.generic_string();
		//	logger::log(ww.str());
		//}
	}
	else logger::log("[Stage] Could not write graph snapshot - temp directory not found.");
}
