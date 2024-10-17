# include <MyDx11/Camera.h>
# include <DirectXMath.h>
# include <MyDx11/Context.h>

ZDSJ::Camera::Camera(ZDSJ::Context* _context, float _window_rate)
{
	_context->Keyboard()->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_wheel, "change camera_pos_z", [&](float _data) {
		this->m_pos.z += (_data * this->m_pos_z_step);
		this->calsulateViewMatrix();
	});
	this->calculateProjectionMatrix(_window_rate);
	this->calsulateViewMatrix();
}

float ZDSJ::Camera::fov()
{
	return this->m_fov;
}

void ZDSJ::Camera::fov(float _value)
{
	this->m_fov = _value;
	this->calculateProjectionMatrix();
}

float ZDSJ::Camera::cameraPosZ() const
{
	return this->m_pos.z;
}

void ZDSJ::Camera::cameraAway()
{
	this->m_pos.z -= this->m_pos_z_step;
	this->calsulateViewMatrix();
}

void ZDSJ::Camera::cameraApproach()
{
	this->m_pos.z += this->m_pos_z_step;
	this->calsulateViewMatrix();
}

DirectX::XMMATRIX ZDSJ::Camera::getCarmeraMatrix()
{
	std::shared_lock<std::shared_mutex> lock(this->m_shared_mutex);
	return (*this->view_matrix) * (*this->m_projection_matrix);
}

void ZDSJ::Camera::calculateProjectionMatrix()
{
	this->calculateProjectionMatrix(ZDSJ::Context::getInstance()->windowRate());
}

void ZDSJ::Camera::calculateProjectionMatrix(float _window_rate)
{
	std::unique_lock<std::shared_mutex> lock(this->m_shared_mutex);
	delete this->m_projection_matrix;
	// 投影矩阵
	this->m_projection_matrix = new DirectX::XMMATRIX(DirectX::XMMatrixPerspectiveFovLH(this->m_fov / 180.0f * DirectX::XM_PI, _window_rate, this->m_near_plane, this->m_far_plane));

}

void ZDSJ::Camera::calsulateViewMatrix()
{
	std::unique_lock<std::shared_mutex> lock(this->m_shared_mutex);
	delete this->view_matrix;
	// 视图矩阵
	this->view_matrix = new DirectX::XMMATRIX(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(this->m_pos.x, this->m_pos.y, this->m_pos.z, this->m_pos.w), // 摄像机位置  
		DirectX::XMVectorSet(this->m_focus.x, this->m_focus.y, this->m_focus.z, this->m_focus.w),     // 目标点（世界空间原点）  
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // 上方向（y轴）);
	));
}

ZDSJ::Camera::~Camera()
{

}
