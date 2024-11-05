# pragma once
# include <fstream>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ZDSJ {
	class DrawAbleManager;

	class Persistence {
	public:
		Persistence(std::string _filename, bool _out = false);
		bool load(ID3D11Device* _device, ID3D11DeviceContext* _context, DrawAbleManager*& _manager);
		bool save(const DrawAbleManager* _manager);
		bool save(const uint8_t* _data, const size_t _size) const;
		bool load(uint8_t* _data, const size_t _size) const;
		~Persistence();
	private:
		std::ofstream* m_output_stream = nullptr;
		std::ifstream* m_input_stream = nullptr;
	};

}