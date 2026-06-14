# pragma once
# include <unordered_map>
# include <vector>
# include <memory>
# include <string>

struct ID3D11DeviceContext;
struct ID3D11Device;

namespace ZDSJ {

	struct Point;
	class DrawAbleData;
	class DrawAbleInterface;
	class ParticlePhysicsManager;
	enum class DrawAbleClass {
		Default,
		Triangle2D,
		Rectangle2D,
		Arc2D,
	};

	struct Vertex2D;
	typedef unsigned short UINT16;

	class BaseManager {
		friend class Persistence;
	public:
		explicit BaseManager(ID3D11Device* _device, ID3D11DeviceContext* _context);

		void add(DrawAbleData* _data);

		void render(ID3D11DeviceContext* _context);

		void physicsCalculate(float _deltaTime);

		~BaseManager() = default;
	private:
		std::vector<std::shared_ptr<DrawAbleData>> m_data;
		std::unordered_map<std::string, std::shared_ptr<DrawAbleInterface>> m_render;
		ParticlePhysicsManager* m_particlePhysicsManager = nullptr;

		bool skipCalculate(const DrawAbleData* _first, const DrawAbleData* _second) const;
	};
}