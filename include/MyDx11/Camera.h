# pragma once
# include <shared_mutex>

namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class Context;
	struct float4 {
		float x;
		float y;
		float z;
		float w;
	};

	class Camera {
		friend Context;
	public:

		float fov();
		void fov(float _value);

		float cameraPosZ() const;
		void cameraAway();

		void cameraApproach();
		// void 

		DirectX::XMMATRIX getCarmeraMatrix();
		
	private:
		float4 m_pos = { 0.0f, 0.0f, -200.0f, 1.0f };
		float4 m_focus = { 0.0f, 0.0f, 0.0f, 1.0f };
		float m_pos_z_step = 10.0f;
		float m_fov = 70.0f;
		float m_near_plane = 0.1f;
		float m_far_plane = 1000.0f;
		DirectX::XMMATRIX* m_projection_matrix = nullptr;
		DirectX::XMMATRIX* view_matrix = nullptr;
		std::shared_mutex m_shared_mutex;

		Camera(Context* _context, float _window_rate);

		void calculateProjectionMatrix();
		void calculateProjectionMatrix(float _window_rate);
		void calsulateViewMatrix();

		~Camera();
	};

}