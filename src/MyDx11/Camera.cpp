# include <MyDx11/Camera.h>
# include <DirectXMath.h>
# include <MyDx11/Context.h>

ZDSJ::Camera::Camera(ZDSJ::Context* _context, float _window_rate)
{
	_context->Keyboard()->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_wheel, "change camera position z, scroll up close to the object, scroll down away from the object", [&](float _data) {
		this->m_pos.z += (_data * this->m_pos_z_step);
		this->calsulateViewMatrix();
	});
	_context->Keyboard()->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_drag, "change camera position (x,y)", [&](float _data) {
		short x;
		short y;
		ZDSJ::Context::getInstance()->Keyboard()->splitFloatToShorts(_data, x, y);
		this->m_pos.x -= x;
		this->m_pos.y += y;
		this->calsulateViewMatrix();
	});
	_context->Keyboard()->registeKeyboard(ZDSJ::Key::ctrl, 'R', "reset camera position (x,y,z)", [&](float _data) {
		this->m_pos.x = 0.0f;
		this->m_pos.y = 0.0f;
		this->m_pos.z = -200.0f;
		this->calsulateViewMatrix();
	});
	_context->command()->registeCommand("camera", "pos", "change camera position, for example camera:pos:x:[yor data], camera:pos:reset to reset camera position", [&](std::string& _data) {
		std::istringstream iss(_data);
		std::string component;
		std::string data_str;
		std::getline(iss, component, ':');
		std::getline(iss, data_str, ':');
		float data = 0.0f;
		if (component == "reset") {
			this->m_pos.x = 0.0f;
			this->m_pos.y = 0.0f;
			this->m_pos.z = -200.0f;
			this->calsulateViewMatrix();
			return true;
		}
		try
		{
			data = std::stof(data_str);

		}
		catch (const std::exception&)
		{
			return false;
		}
		if (component == "x") {
			this->m_pos.x = data;
		}
		else if (component == "y") {
			this->m_pos.y = data;
		}
		else if (component == "z") {
			this->m_pos.z = data;
		}
		else {
			return false;
		}
		this->calsulateViewMatrix();
		return true;
	});
	_context->command()->registeCommand("camera", "fov", "change fov by angle", [&](std::string& _data) {
		float data = 0.0f;
		try
		{
			data = std::stof(_data);

		}
		catch (const std::exception&)
		{
			return false;
		}
		this->m_fov = data;
		this->calculateProjectionMatrix();
		return true;
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

ZDSJ::float4 ZDSJ::Camera::cameraPos() const
{
	return this->m_pos;
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
		DirectX::XMVectorSet(this->m_pos.x, this->m_pos.y, 0.0f, 1.0f),     // 目标点（世界空间原点）  
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // 上方向（y轴）);
	));
}

ZDSJ::Camera::~Camera()
{

}
