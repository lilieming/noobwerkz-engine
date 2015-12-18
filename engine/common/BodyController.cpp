#include "BodyController.hpp"
#include "TransformHelper.hpp"
#include "Logger.hpp"


#include <cmath>
/*
void noob::body_controller::init(btDynamicsWorld* _dynamics_world, const noob::shape* _shape, float mass, const noob::vec3& pos, const noob::versor& orient, bool ccd)
{
	dynamics_world = _dynamics_world;
	btTransform start_transform;
	start_transform.setIdentity();
	start_transform.setOrigin(btVector3(pos.v[0], pos.v[1], pos.v[2]));
	start_transform.setRotation(btQuaternion(orient.q[0], orient.q[1], orient.q[2], orient.q[3]));
	btVector3 inertia(0.0, 0.0, 0.0);
	btDefaultMotionState* motion_state = new btDefaultMotionState(start_transform);
	shape = const_cast<noob::shape*>(_shape);
	shape->inner_shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass, motion_state, shape->inner_shape, inertia);
	inner_body = new btRigidBody(ci);
	set_ccd(ccd);

	dynamics_world->addRigidBody(inner_body);
}

void noob::body_controller::init(btDynamicsWorld* _dynamics_world, const noob::shape* _shape, const noob::body_controller::info& _info)
{
	dynamics_world = _dynamics_world;
	btTransform start_transform;
	start_transform.setIdentity();
	start_transform.setOrigin(btVector3(_info.position.v[0], _info.position.v[1], _info.position.v[2]));
	start_transform.setRotation(btQuaternion(_info.orientation.q[0], _info.orientation.q[1], _info.orientation.q[2], _info.orientation.q[3]));
	btVector3 inertia(0, 0, 0);
	btDefaultMotionState* motion_state = new btDefaultMotionState(start_transform);
	shape = const_cast<noob::shape*>(_shape);
	shape->inner_shape->calculateLocalInertia(_info.mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo ci(_info.mass, motion_state, shape->inner_shape, inertia);
	inner_body = new btRigidBody(ci);
	inner_body->setFriction(_info.friction);
	inner_body->setRestitution(_info.restitution);
	inner_body->setAngularFactor(btVector3(_info.angular_factor.v[0], _info.angular_factor.v[1], _info.angular_factor.v[2]));
	inner_body->setLinearFactor(btVector3(_info.linear_factor.v[0], _info.linear_factor.v[1], _info.linear_factor.v[2]));
	inner_body->setLinearVelocity(btVector3(_info.linear_velocity.v[0], _info.linear_velocity.v[1], _info.linear_velocity.v[2]));
	inner_body->setAngularVelocity(btVector3(_info.angular_velocity.v[0], _info.angular_velocity.v[1], _info.angular_velocity.v[2]));
	self_control = _info.self_control;
	set_ccd(_info.ccd);
	dynamics_world->addRigidBody(inner_body);
}
*/
/*
   void noob::body_controller::set_position(const noob::vec3& pos)
   {
   }


   void noob::body_controller::set_orientation(const noob::versor& orient)
   {
   }
   */

void noob::body_controller::set_self_control(bool b)
{
	if (b == true)
	{
		self_control = true;
		// inner_body->setLinearSleepingThreshold(btScalar(0.8));
		// inner_body->setAngularSleepingThreshold(btScalar(1.0));
		// inner_body->setAngularFactor(btVector2(0.0, 0.0));
		// inner_body->setLinearFactor(btVector3(1.0, 1.0, 1.0));
	}
	else
	{
		self_control = false;
	}
}


void noob::body_controller::update() 
{
	if (self_control)
	{
/*		btTransform xform;

		inner_body->getMotionState()->getWorldTransform(xform);

		btVector3 down = -xform.getBasis()[1];
		btVector3 forward = xform.getBasis()[2];

		down.normalize();
		forward.normalize();

		btVector3 ray_source = xform.getOrigin();
		btVector3 ray_target = ray_source + btVector3(0.0, -((height * 0.5) + 5.0), 0.0);

		btCollisionWorld::ClosestRayResultCallback ray_callback(ray_source, ray_target);

		dynamics_world->rayTest(ray_source, ray_target, ray_callback);

		if (ray_callback.hasHit())
		{
			airborne = false;
			// if (i == 0) airborne = false;
			// if (i == 1) obstacle = false;
			// logger::log("[Character] - ray hit!");
			ray_lambda = ray_callback.m_closestHitFraction;
		}
		else
		{
			airborne = true;
			// logger::log("[Character] - no hit");
			ray_lambda = 1.0;
		}
*/
	}
}


void noob::body_controller::move(bool forward, bool backward, bool left, bool right, bool jump)
{
/*	if (self_control)
	{
		if (on_ground())
		{
			btTransform xform;
			inner_body->getMotionState()->getWorldTransform(xform);

			btVector3 linear_velocity = inner_body->getLinearVelocity();
			btScalar speed = inner_body->getLinearVelocity().length();
			btVector3 walk_direction = btVector3(0.0, 0.0, 0.0);

			btScalar walk_speed = 0.5;// / 3.0;

			btVector3 forward_dir(walk_speed, 0.0, 0.0);

			if (forward) walk_direction += forward_dir;
			if (backward) walk_direction -= forward_dir;

			if (jump)
			{
				linear_velocity += btVector3(0.0, 1.5, 0.0);
			}
			if (!forward && !backward && !left && !right && !jump && on_ground())
			{
				// Dampen when on the ground and not being moved by the player
				linear_velocity *= btScalar(0.2);
			}
			else
			{
				if (speed < max_linear_velocity)
				{
					linear_velocity = linear_velocity + walk_direction * walk_speed;
				}
			}
		}
	}
*/
}


bool noob::body_controller::on_ground() const
{
	return (!(airborne && obstacle));
	//	return ray_lambda[0] < btScalar(1.0);
}


noob::vec3 noob::body_controller::get_position() const
{
	noob::vec3 pos;
	NewtonBodyGetMatrix(inner_body, &pos.v[0]);
	return pos;
}


noob::versor noob::body_controller::get_orientation() const
{

	noob::versor orient;
	NewtonBodyGetRotation(inner_body, &orient.q[0]);
	return orient;
}


noob::vec3 noob::body_controller::get_linear_velocity() const
{
	noob::vec3 vel;
	NewtonBodyGetVelocity(inner_body, &vel.v[0]);
	return vel;	
}


noob::vec3 noob::body_controller::get_angular_velocity() const
{
	noob::vec3 omega;
	NewtonBodyGetOmega(inner_body, &omega.v[0]);
	return omega;
}



noob::mat4 noob::body_controller::get_transform() const
{
	noob::mat4 xform;
	NewtonBodyGetMatrix(inner_body, &xform.m[0]);
	return xform;
}


std::string noob::body_controller::get_debug_string() const
{
	fmt::MemoryWriter w;
	w << "[Body] position " << get_position().to_string() << ", orientation " << get_orientation().to_string() << ", linear velocity " << get_linear_velocity().to_string() << ", angular velocity " << get_angular_velocity().to_string() << ", on ground? " << on_ground() << ", ray lambda  = " << ray_lambda; //<< " ray lambda # 2 = " << ray_lambda[1];
	return w.str();
}


void noob::body_controller::set_ccd(bool b)
{
	if (b == true)
	{
		// btVector3 center;
		// btScalar radius;
		// shape->inner_shape->getBoundingSphere(center, radius);
		// inner_body->setCcdMotionThreshold(radius);
	}
}


void noob::body_controller::sphere(float mass, float r, const noob::vec3& pos, const NewtonWorld* const world)
{
	NewtonCollision* const collision = NewtonCreateSphere(world, r, 0, NULL);

	noob::mat4 matrix = noob::identity_mat4();

	matrix = noob::translate(matrix, pos);
	
	inner_body = NewtonCreateDynamicBody(world, collision, &matrix.m[0]);

	// set the force callback for applying the force and torque
	NewtonBodySetForceAndTorqueCallback(inner_body, noob::body_controller::apply_gravity);

	NewtonBodySetMassProperties(inner_body, mass, collision);

	// NewtonBodySetLinearDamping (body, 0.0f);

	NewtonDestroyCollision(collision);
}


void noob::body_controller::box(float mass, float x, float y, float z, const noob::vec3& pos, const NewtonWorld* const world)
{
	NewtonCollision* const collision = NewtonCreateBox(world, x, y, z, 0, NULL);

	noob::mat4 matrix = noob::identity_mat4();

	matrix = noob::translate(matrix, pos);
	
	inner_body = NewtonCreateDynamicBody(world, collision, &matrix.m[0]);

	// set the force callback for applying the force and torque
	NewtonBodySetForceAndTorqueCallback(inner_body, noob::body_controller::apply_gravity);

	NewtonBodySetMassProperties(inner_body, mass, collision);

	// NewtonBodySetLinearDamping (body, 0.0f);

	NewtonDestroyCollision(collision);
	
}


void noob::body_controller::cone(float mass, float r, float h, const noob::vec3& pos, const NewtonWorld* const world)
{
	NewtonCollision* const collision = NewtonCreateCone(world, r, h, 0, NULL);

	noob::mat4 matrix = noob::identity_mat4();

	matrix = noob::translate(matrix, pos);
	
	inner_body = NewtonCreateDynamicBody(world, collision, &matrix.m[0]);

	// set the force callback for applying the force and torque
	NewtonBodySetForceAndTorqueCallback(inner_body, noob::body_controller::apply_gravity);

	NewtonBodySetMassProperties(inner_body, mass, collision);

	// NewtonBodySetLinearDamping (body, 0.0f);

	NewtonDestroyCollision(collision);

}


void noob::body_controller::cylinder(float mass, float r, float h, const noob::vec3& pos, const NewtonWorld* const world)
{
	NewtonCollision* const collision = NewtonCreateCylinder(world, r, h, 0, NULL);

	noob::mat4 matrix = noob::identity_mat4();

	matrix = noob::translate(matrix, pos);
	
	inner_body = NewtonCreateDynamicBody(world, collision, &matrix.m[0]);

	// set the force callback for applying the force and torque
	NewtonBodySetForceAndTorqueCallback(inner_body, noob::body_controller::apply_gravity);

	NewtonBodySetMassProperties(inner_body, mass, collision);

	// NewtonBodySetLinearDamping (body, 0.0f);

	NewtonDestroyCollision(collision);

}


void noob::body_controller::static_mesh(const noob::basic_mesh& m, const noob::vec3& pos, const NewtonWorld* const world)
{
	NewtonCollision* const collision = NewtonCreateTreeCollision(world, 0);

	NewtonTreeCollisionBeginBuild (collision);

	for (size_t i = 0; i < m.indices.size(); i += 3)
	{
		std::array<noob::vec3, 3> current_face;
		current_face[0] = m.vertices[i];
		current_face[1] = m.vertices[i+1];
		current_face[2] = m.vertices[i+2];
		NewtonTreeCollisionAddFace (collision, 1, &current_face[0].v[0], 3 * sizeof (float), 0);
	}
	NewtonTreeCollisionEndBuild (collision, 1);

	noob::mat4 matrix = noob::identity_mat4();
	matrix = noob::translate(matrix, pos);
	inner_body = NewtonCreateDynamicBody(world, collision, &matrix[0]);

	NewtonDestroyCollision(collision);

}


void noob::body_controller::apply_gravity(const NewtonBody* const bod, float dt, int thread_index)
{
	float mass;
	float Ixx;
	float Iyy;
	float Izz;

	NewtonBodyGetMassMatrix(bod, &mass, &Ixx, &Iyy, &Izz);
	noob::vec4 gravity_force (0.0f, -9.8f * mass, 0.0f, 0.0f);
	NewtonBodySetForce(bod, &gravity_force.v[0]);
}
