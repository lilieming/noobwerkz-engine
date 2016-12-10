#pragma once

// std
#include <string>

// External libs
#include <noob/component/component.hpp>
#include <noob/singleton/singleton.hpp>
#include <noob/math/math_funcs.hpp>

// Project-local
#include "BasicMesh.hpp"
#include "Model.hpp"
#include "GpuBuffer.hpp"
#include "Texture.hpp"
#include "Attrib.hpp"
#include "ReturnType.hpp"
#include "Shader.hpp"
#include "TextureLoader.hpp"

namespace noob
{
	class graphics
	{
		public:
			typedef noob::handle<noob::shader> shader_handle;
			typedef noob::handle<noob::linked_shader> program_handle;

			// Call before using.
			void init(const std::array<uint32_t, 2> Dims) noexcept(true);

			// Call this every frame...
			void frame(const std::array<uint32_t, 2>) const noexcept(true);

			// Call before killing.
			void destroy() noexcept(true);

			void use_program(noob::graphics::program_handle) noexcept(true);

			// Call this to draw a given model.
			void draw_instanced(const noob::model_handle, uint32_t NumInstances) const noexcept(true);
			void draw_terrain(uint32_t Verts) const noexcept(true);

			// Currently, instanced models only support the basic vertex colours. This is to change quite soon.
			noob::model_handle model_instanced(const noob::basic_mesh&, uint32_t) noexcept(true);
			void resize_instanced_data_buffers(noob::model_handle, uint32_t) noexcept(true);
	
			// Currently implemented as a triplanar-shaded, single-buffer model.
			// Due to the flexibility of the shaders used, it is easy to support peeling visible faces off objects and sending them to video buffer to keep drawcalls down to one.
			// It should run smoothly using compressed textures because although texture reads are done three times, those values get reused to recreate all needed maps.
			void set_num_terrain_verts(uint32_t NumVerts) noexcept(true);
			void resize_terrain(uint32_t MaxVerts) noexcept(true);
			void set_terrain_uniforms(const noob::terrain_shading) noexcept(true);
			// void draw_terrain() const noexcept(true);

			// TODO: Replace with more generic uniform setting method(s)
			void set_eye_pos(const noob::vec3&) const noexcept(true);
			void set_light_direction(const noob::vec3&) const noexcept(true);

			// These are VBO buffer mapping/unmapping methods
			// TODO: Split into several functions
			noob::gpu_write_buffer map_instanced_data_buffer(noob::model_handle, noob::model::instanced_data_type, uint32_t Min, uint32_t Max) const noexcept(true);
			noob::gpu_write_buffer map_terrain_buffer(uint32_t MinVert, uint32_t MaxVert) const noexcept(true); // uint32_t Min, uint32_t Max) const noexcept(true);			
			// NOTE: MUST be called as soon as you're finished using a mapped buffer!
			void unmap_buffer() const noexcept(true);

			// Texture storage reservers
			// noob::texture_1d_handle reserve_texture_1d(uint32_t length, bool compressed, noob::texture_channels, noob::attrib::unit_type) noexcept(true); // TODO
			noob::texture_2d_handle reserve_texture_2d(const std::array<uint32_t, 2> Dims, const noob::texture_info) noexcept(true);
			noob::texture_array_2d_handle reserve_texture_array_2d(const std::array<uint32_t, 2> Dims, uint32_t Indices, const noob::texture_info) noexcept(true);
			noob::texture_3d_handle reserve_texture_3d(const std::array<uint32_t, 3> Dims, const noob::texture_info) noexcept(true);
			// noob::texture_handle reserve_texture_cube(uint32_t dims, bool mips, noob::texture_channels, noob::attrib::unit_type, const std::string& data) noexcept(true); // TODO
			
			void bind_texture(noob::texture_2d_handle) const noexcept(true);
			void bind_texture(noob::texture_array_2d_handle) const noexcept(true);
			void bind_texture(noob::texture_3d_handle) const noexcept(true);

			// Texture data uploaders
			// void texture_data(noob::texture_1d_handle, const std::string&) const noexcept(true);	// TODO
			void texture_data(noob::texture_2d_handle, uint32_t Mip, const std::array<uint32_t, 2> Offsets, const std::array<uint32_t, 2> Dims, const uint8_t* DataPtr) const noexcept(true);
			void texture_data(noob::texture_array_2d_handle, uint32_t Mip, uint32_t Index, const std::array<uint32_t, 2> Offsets, const std::array<uint32_t, 2> Dims, const uint8_t* DataPtr) const noexcept(true);
			void texture_data(noob::texture_3d_handle, uint32_t Mip, const std::array<uint32_t, 3> Offsets, const std::array<uint32_t, 3> Dims, const uint8_t* DataPtr) const noexcept(true);

			// Texture parameter setters, made typesafe, at the cost of more overloads. Ah well, tradeoffs...
			// Params for 2d textures
			void texture_base_level(uint32_t Mip = 0) const noexcept(true);
			void texture_compare_mode(noob::tex_compare_mode) const noexcept(true);
			void texture_compare_func(noob::tex_compare_func) const noexcept(true);
			void texture_min_filter(noob::tex_min_filter) const noexcept(true);
			void texture_mag_filter(noob::tex_mag_filter) const noexcept(true);
			void texture_min_lod(int32_t Mip = -1000) const noexcept(true);
			void texture_max_lod(int32_t Mip = 1000) const noexcept(true);
			void texture_swizzle(const std::array<noob::tex_swizzle, 2>) const noexcept(true);
			void texture_swizzle(const std::array<noob::tex_swizzle, 3>) const noexcept(true);			
			void texture_swizzle(const std::array<noob::tex_swizzle, 4>) const noexcept(true);
			void texture_wrap_mode(const std::array<noob::tex_wrap_mode, 2>) const noexcept(true);			
			void texture_wrap_mode(const std::array<noob::tex_wrap_mode, 3>) const noexcept(true);

			// Texture packing and unpacking alignments.
			// Fun fact: The GLES3 spec gives all the other pixel packing/unpacking attributes an initial value of 0 and a range of 0, so they aren't even exposed here. How simple!
			void texture_pack_alignment(uint32_t) const noexcept(true);
			void texture_unpack_alignment(uint32_t) const noexcept(true);

			// Note: These have no effect on compressed textures, which have to be mipped offline as a preprocessing step.
			void generate_mips(noob::texture_2d_handle) const noexcept(true);
			void generate_mips(noob::texture_array_2d_handle) const noexcept(true);
			void generate_mips(noob::texture_3d_handle) const noexcept(true);

			noob::graphics::program_handle get_instanced_shader() const noexcept(true);

		protected:
			std::vector<noob::model> models;
			noob::model terrain_model;
			
			std::vector<noob::texture_1d> textures_1d;
			std::vector<noob::texture_2d> textures_2d;
			std::vector<noob::texture_array_2d> texture_arrays_2d;			
			std::vector<noob::texture_3d> textures_3d;

			noob::graphics::program_handle instanced_shader;
			noob::graphics::program_handle terrain_shader;
			
			// These will soon be replaced by proper UBO's and made typesafe. The only reason they're here is to serve as a stable, well-understood prior case example.
			uint32_t u_eye_pos, u_light_directional;
			uint32_t u_eye_pos_terrain, u_light_directional_terrain, u_texture_0, u_colour_0, u_colour_1, u_colour_2, u_colour_3, u_blend_0, u_blend_1, u_tex_scales, u_model_scales;
		
			noob::vec3 eye_pos, light_direction;
			bool initialized = false;
			bool terrain_initialized = false;
			
			uint32_t max_terrain_verts = 0;
	};

	static noob::singleton<noob::graphics> gfx_instance;
	static noob::graphics& get_graphics() { return gfx_instance.get(); }
}
