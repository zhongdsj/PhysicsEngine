# pragma once
# include <DirectXMath.h>
# include <memory>
# include <string>
# include <SignalSender.h>
# include <unordered_map>
# include <Slot.h>
# include <sstream>

namespace ZDSJ
{
	class MovementInterface;
	class DrawAbleData: public SignalSender
	{
	public:
		DrawAbleData(const std::string& _render_type = "", const DirectX::XMFLOAT3& _position = { 0.0f, 0.0f, 0.0f }, const DirectX::XMFLOAT3& _size = { 20.0f, 20.0f, 1.0f },
		             const DirectX::XMFLOAT3& _rotation = { 0.0f, 0.0f, 0.0f });

		Slot* connect(const std::string& signal, Slot* slot) override;

		template <class... Args>
		void emit(const std::string& signal, Args&&... args)
		{
			auto pair = this->m_slots.find(signal);
			if (pair != this->m_slots.end())
			{
				std::vector<std::shared_ptr<Slot>>& temp = pair->second;
				for (auto iterator = temp.begin(); iterator != temp.end(); ) {
					if ((*iterator)->useful())
					{
						(*iterator)->emit(std::forward<Args>(args)...);
						++iterator;
					}
					else
					{
						iterator = temp.erase(iterator);
					}
				}
			}
		}

		void applyForce(const DrawAbleData* _other) const;

		void update(float _magnification);

		DrawAbleData* addVelocity(const DirectX::XMFLOAT3& _velocity);

		const std::string& renderType() const
		{
			return this->m_render_type;
		}

		DrawAbleData* renderType(const std::string& _render_type)
		{
			this->m_render_type = _render_type;
			return this;
		}

		const DirectX::XMFLOAT3& size() const
		{
			return this->m_size;
		}

		DrawAbleData* size(const DirectX::XMFLOAT3& _size)
		{
			this->m_size = _size;
			return this;
		}

		const DirectX::XMFLOAT3& position() const
		{
			return this->m_position;
		}

		DrawAbleData* position(const DirectX::XMFLOAT3& _position)
		{
			this->m_position = _position;
			this->emit("position");
			return this;
		}

		const DirectX::XMFLOAT3& rotation() const
		{
			return this->m_rotation;
		}

		const DirectX::XMFLOAT3& renderSize() const
		{
			return this->m_renderSize;
		}

		const DirectX::XMFLOAT3& renderPosition() const
		{
			return this->m_renderPosition;
		}

		const DirectX::XMFLOAT3& renderRotation() const
		{
			return this->m_renderRotation;
		}

		DrawAbleData* rotation(const DirectX::XMFLOAT3& _rotation)
		{
			this->m_rotation = _rotation;
			return this;
		}

		const MovementInterface* movement() const
		{
			return this->m_movement.get();
		}

		std::string printProperties() const
		{
			std::ostringstream oss;
			oss << "size: (" << this->m_size.x << ", " << this->m_size.y << ", " << this->m_size.z << ")\n";
			oss << "position: (" << this->m_position.x << ", " << this->m_position.y << ", " << this->m_position.z << ")\n";
			oss << "rotation: (" << this->m_rotation.x << ", " << this->m_rotation.y << ", " << this->m_rotation.z << ")\n";
			return std::move(oss.str());
		}

		~DrawAbleData() override = default;
	private:
		std::string m_render_type = "";
		DirectX::XMFLOAT3 m_size = {20.0f, 20.0f, 1.0f};
		DirectX::XMFLOAT3 m_position = {0.0f, 0.0f, 0.0f};
		DirectX::XMFLOAT3 m_rotation = {0.0f, 0.0f, 0.0f};
		DirectX::XMFLOAT3 m_renderSize = { 20.0f, 20.0f, 1.0f };
		DirectX::XMFLOAT3 m_renderPosition = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 m_renderRotation = { 0.0f, 0.0f, 0.0f };
		std::shared_ptr<MovementInterface> m_movement = nullptr;
		std::unordered_map<std::string, std::vector<std::shared_ptr<Slot>>> m_slots;
	protected:
		void registerSignal() override;
	};
}
