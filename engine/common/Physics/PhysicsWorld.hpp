#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <memory>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>

#include "NoobUtils.hpp"
#include "Mesh.hpp"
#include "PhysicsBody.hpp"
#include "PhysicsConstraintGeneric.hpp"
#include "PhysicsShape.hpp"

#define BIT(x) (1<<(x))


namespace noob
{
	class physics_world
	{
		public:

			enum collision_type
			{
				NOTHING = 0,
				CHARACTER = BIT(0),
				TERRAIN = BIT(1)
			};

			~physics_world();

			void init();
			void cleanup();
			void step(double delta);

			// TODO: Implement
			void apply_global_force(const noob::vec3& force, const noob::physics_shape& shape, const noob::vec3& origin);

			void add(const noob::physics_body& body, short collision_group, short collides_with);
			void add(const noob::physics_constraint_generic& constraint);
			
			std::shared_ptr<noob::physics_shape> sphere(float radius);
			std::shared_ptr<noob::physics_shape> box(float width, float height, float depth);
			std::shared_ptr<noob::physics_shape> cylinder(float radius, float height);
			std::shared_ptr<noob::physics_shape> capsule(float radius, float height);
			std::shared_ptr<noob::physics_shape> cone(float radius, float height);

			std::shared_ptr<noob::physics_shape> set_convex_hull(const std::vector<std::array<float, 3>>& points, const std::string& name);
			std::shared_ptr<noob::physics_shape> set_multisphere(const std::vector<std::array<float, 4>>& rad_pos_scales, const std::string& name);
			std::shared_ptr<noob::physics_shape> set_compound_shape(const std::vector<const std::shared_ptr<noob::physics_shape>&>& shapes, const std::string& name);
			std::shared_ptr<noob::physics_shape> set_trimesh(const std::tuple<std::vector<std::array<float, 3>>, std::vector<uint16_t>>& points, const std::string& name);

			std::weak_ptr<noob::physics_shape> get_convex_hull(const std::string& name);
			std::weak_ptr<noob::physics_shape> get_multisphere(const std::string& name);
			std::weak_ptr<noob::physics_shape> get_compound_shape(const std::string& name);
			std::weak_ptr<noob::physics_shape> get_trimesh(const std::string& name);

			
			void remove(const noob::physics_body& body);
			void remove(const noob::physics_constraint_generic& constraint);

			btDynamicsWorld* get_raw_ptr() const;

		protected:
			
			std::unordered_map<float, std::shared_ptr<noob::physics_shape>> spheres;
			std::map<std::array<float, 2>, std::shared_ptr<noob::physics_shape>> cylinders;
			std::map<std::array<float, 2>, std::shared_ptr<noob::physics_shape>> capsules;
			std::map<std::array<float, 2>, std::shared_ptr<noob::physics_shape>> cones;
			std::map<std::array<float, 3>, std::shared_ptr<noob::physics_shape>> boxes;
			std::unordered_map<std::string, std::shared_ptr<noob::physics_shape>> convex_hulls;
			std::unordered_map<std::string, std::shared_ptr<noob::physics_shape>> multispheres;
			std::unordered_map<std::string, std::shared_ptr<noob::physics_shape>> compound_shapes;
			std::unordered_map<std::string, std::tuple<std::shared_ptr<noob::physics_shape>, std::vector<std::array<float, 3>>, std::vector<uint16_t>>> trimeshes;
			
			btDiscreteDynamicsWorld* dynamics_world;
			btBroadphaseInterface* broadphase;
			btDefaultCollisionConfiguration* collision_configuration;
			btCollisionDispatcher* dispatcher;
			btSequentialImpulseConstraintSolver* solver;	

	};
}
