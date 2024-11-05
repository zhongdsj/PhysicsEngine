# pragma once
# include <shared_mutex>

namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class Context;
	struct Point;
	struct float4 {
		float x;
		float y;
		float z;
		float w;
	};

	class Camera {
		friend Context;
	public:

		Point viewPosToWordPos(Point _pos) const;
		float fov() const;
		void fov(float _value);

		float4 cameraPos() const;
		void cameraAway();

		void cameraApproach();
		// void 

		DirectX::XMMATRIX getCameraMatrix();
		
	private:
		Point* m_view_pos = nullptr;
		float4 m_pos = { 0.0f, 0.0f, -500.0f, 1.0f };
		// float4 m_focus = { 0.0f, 0.0f, 0.0f, 1.0f };
		float m_pos_z_step = 10.0f;
		float m_fov = 60.0f;
		float m_near_plane = 0.1f;
		float m_far_plane = 1000.0f;
		DirectX::XMMATRIX* m_projection_matrix = nullptr;
		DirectX::XMMATRIX* m_view_matrix = nullptr;
		std::shared_mutex m_shared_mutex;

		Camera(Context* _context, float _window_rate);

		void calculateProjectionMatrix();
		void calculateProjectionMatrix(float _window_rate);
		void calculateViewMatrix();
		void calculateViewMatrix(float _window_rate);
		void viewPosSize(float _window_rate);
		Point viewPosSize() const;
		void saveToFile() const;
		void loadFromFile();


		~Camera();
	};

}