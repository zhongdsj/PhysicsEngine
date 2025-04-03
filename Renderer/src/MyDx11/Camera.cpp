# include <MyDx11/Camera.h>
# include <DirectXMath.h>
# include <Context.h>
# include <Persistence.h>
# include <Types.h>

ZDSJ::Camera* ZDSJ::Camera::getInstance()
{
	auto context = Context_Instance;
	static ZDSJ::Camera instance(context, context->windowRate());
	return &instance;
}

ZDSJ::Camera::Camera(ZDSJ::Context* _context, float _window_rate) : m_view_pos(new ZDSJ::Point())
{
	this->loadFromFile();
	/*_context->keyboard()->registerKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_wheel, "change camera position z, scroll up close to the object, scroll down away from the object", [&](float _data) {
		this->m_pos.z += (_data * this->m_pos_z_step);
		this->calculateViewMatrix();
	});
	_context->keyboard()->registerKeyboard(ZDSJ::Key::ctrl, ZDSJ::Key::mouse_wheel, "change camera fov", [&](float _data)
	{
		this->m_fov -= _data;
		this->calculateProjectionMatrix();
	});
	_context->keyboard()->registerKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_drag, "change camera position (x,y)", [&](float _data) {
		short x;
		short y;
		ZDSJ::Context::getInstance()->keyboard()->splitFloatToShorts(_data, x, y);
		ZDSJ::Point drag_pos = ZDSJ::Context::getInstance()->keyboard()->mouseDrag();
		x -= drag_pos.x;
		y -= drag_pos.y;
		this->m_pos.x -= x;
		this->m_pos.y += y;
		this->calculateViewMatrix();
	});
	_context->keyboard()->registerKeyboard(ZDSJ::Key::ctrl, 'R', "reset camera position (x,y,z)", [&](float _data) {
		this->loadFromFile();
		this->calculateProjectionMatrix();
		this->calculateViewMatrix();
	});
	_context->keyboard()->registerKeyboard(ZDSJ::Key::alt, ZDSJ::Key::mouse_wheel, "change camera pos_z_step", [&](float _data)
	{
		this->m_pos_z_step -= _data;
	});
	_context->command()->registerCommand("camera", "pos", "change camera position, for example camera:pos:x:[yor data], camera:pos:reset to reset camera position", [&](const std::string& _data) {
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
			this->calculateViewMatrix();
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
		this->calculateViewMatrix();
		return true;
	});
	_context->command()->registerCommand("camera", "fov", "change fov by angle", [&](const std::string& _data) {
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
	_context->command()->registerCommand("camera", "pos_z_step", "change camera pos_z_step", [&](const std::string& _data)->bool
	{
		if(_data.empty())
		{
			_context->command()->write("camera pos_z_step: " + std::to_string(this->m_pos_z_step));
			return true;
		}
		float data = 0.0f;
		try
		{
			data = std::stof(_data);

		}
		catch (const std::exception&)
		{
			return false;
		}
		this->m_pos_z_step = data;
		return true;

	});*/
	this->calculateProjectionMatrix(_window_rate);
	this->calculateViewMatrix(_window_rate);
}

ZDSJ::Point ZDSJ::Camera::viewPosToWordPos(ZDSJ::Point _pos) const
{
	ZDSJ::Point point;
	ZDSJ::Context* context = Context_Instance;
	point.x = this->viewPosSize().x * (_pos.x / context->getWindowWidth()) - this->viewPosSize().x / 2 + this->m_pos.x;
	point.y = this->viewPosSize().y * (_pos.y / context->getWindowHeight()) * -1 + this->viewPosSize().y / 2 + this->m_pos.y;
	return point;
}

float ZDSJ::Camera::fov() const
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

float ZDSJ::Camera::cameraStep() const
{
	return this->m_pos_z_step;
}

void ZDSJ::Camera::cameraAway()
{
	this->m_pos.z -= this->m_pos_z_step;
	this->calculateViewMatrix();
}

void ZDSJ::Camera::cameraApproach()
{
	this->m_pos.z += this->m_pos_z_step;
	this->calculateViewMatrix();
}

DirectX::XMMATRIX ZDSJ::Camera::getCameraMatrix()
{
	std::shared_lock<std::shared_mutex> lock(this->m_shared_mutex);
	return (*this->m_view_matrix) * (*this->m_projection_matrix);
}

void ZDSJ::Camera::calculateProjectionMatrix()
{
	this->calculateProjectionMatrix(Context_Instance->windowRate());
}

void ZDSJ::Camera::calculateProjectionMatrix(float _window_rate)
{
	std::unique_lock<std::shared_mutex> lock(this->m_shared_mutex);
	delete this->m_projection_matrix;
	// 投影矩阵
	this->m_projection_matrix = new DirectX::XMMATRIX(DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->m_fov), _window_rate, this->m_near_plane, this->m_far_plane));
	this->viewPosSize(_window_rate);
}

void ZDSJ::Camera::calculateViewMatrix()
{
	this->calculateViewMatrix(Context_Instance->windowRate());
}

void ZDSJ::Camera::calculateViewMatrix(float _window_rate)
{
	std::unique_lock<std::shared_mutex> lock(this->m_shared_mutex);
	delete this->m_view_matrix;
	// 视图矩阵
	this->m_view_matrix = new DirectX::XMMATRIX(DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(this->m_pos.x, this->m_pos.y, this->m_pos.z, this->m_pos.w), // 摄像机位置  
	                                                                      DirectX::XMVectorSet(this->m_pos.x, this->m_pos.y, 0.0f, 1.0f),     // 目标点（世界空间原点）  
	                                                                      DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)   // 上方向（y轴）);
	));
	this->viewPosSize(_window_rate);
}

void ZDSJ::Camera::viewPosSize(float _window_rate)
{
	float tan_fov_2 = DirectX::XMVectorGetX(DirectX::XMVectorTan(DirectX::XMVectorSet(DirectX::XMConvertToRadians(this->m_fov / 2), 0, 0, 0)));
	this->m_view_pos->x = tan_fov_2 * (-this->m_pos.z) * 2 * _window_rate;
	this->m_view_pos->y = tan_fov_2 * (-this->m_pos.z) * 2;
}

ZDSJ::Point ZDSJ::Camera::viewPosSize() const
{
	return *this->m_view_pos;
}

void ZDSJ::Camera::saveToFile() const
{
	ZDSJ::Persistence persistence("init.ini", true);
	size_t size = 0;
	size_t offset = 0;
	size += sizeof(this->m_pos);
	size += sizeof(this->m_fov);
	size += sizeof(this->m_pos_z_step);
	uint8_t* data = new uint8_t[size];

	memcpy_s(data + offset, sizeof(this->m_pos), &this->m_pos, sizeof(this->m_pos));
	offset += sizeof(this->m_pos);
	memcpy_s(data + offset, sizeof(this->m_fov), &this->m_fov, sizeof(this->m_fov));
	offset += sizeof(this->m_fov);
	memcpy_s(data + offset, sizeof(this->m_pos_z_step), &this->m_pos_z_step, sizeof(this->m_pos_z_step));
	offset += sizeof(this->m_pos_z_step);

	persistence.save(data, size);
	delete[] data;
}

void ZDSJ::Camera::loadFromFile()
{
	ZDSJ::Persistence persistence("init.ini");
	size_t size = 0;
	size_t offset = 0;
	size += sizeof(this->m_pos);
	size += sizeof(this->m_fov);
	size += sizeof(this->m_pos_z_step);
	uint8_t* data = new uint8_t[size];
	persistence.load(data, size);
	memcpy_s(&this->m_pos, sizeof(this->m_pos), data + offset, sizeof(this->m_pos));
	offset += sizeof(this->m_pos);
	memcpy_s(&this->m_fov, sizeof(this->m_fov), data + offset, sizeof(this->m_fov));
	offset += sizeof(this->m_fov);
	memcpy_s(&this->m_pos_z_step, sizeof(this->m_pos_z_step), data + offset, sizeof(this->m_pos_z_step));
	offset += sizeof(this->m_pos_z_step);
	delete[] data;
}

ZDSJ::Camera::~Camera()
{
	this->saveToFile();
	delete this->m_view_pos;
}
