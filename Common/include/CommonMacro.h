# pragma once

# define Delete_Copy_Construction(_class_name) \
	_class_name(const _class_name&) = delete

# define Delete_Copy_Assignment(_class_name) \
	_class_name& operator=(const _class_name&) = delete

# define Delete_Move_Construction(_class_name) \
	_class_name(const _class_name&&) = delete

# define Delete_Move_Assignment(_class_name) \
	_class_name& operator=(const _class_name&&) = delete

# define Delete_Copy(_class_name) \
	Delete_Copy_Construction(_class_name); \
	Delete_Copy_Assignment(_class_name)

# define Delete_Move(_class_name) \
	Delete_Move_Construction(_class_name); \
	Delete_Move_Assignment(_class_name)

# define Delete_Copy_And_Move(_class_name) \
	Delete_Copy(_class_name); \
	Delete_Move(_class_name)

# define ReadOnly(type, fields_name, default_value, func_name) type m_##fields_name = default_value; \
	public: inline type get##func_name() const { return this->m_##fields_name; } \
	private: \

# define ReadWrite(type, fields_name, default_value, func_name) type m_##fields_name = default_value; \
	public: inline Context* set##func_name(type _value){ this->m_##fields_name = _value; return this; } \
	inline type get##func_name() const { return this->m_##fields_name; } \
	private: \
