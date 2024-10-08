#pragma once

namespace ZDSJ {

#define Property_ReadWriteStatement(variable_type, method_name) \
				variable_type get##method_name(void) const;\
				void set##method_name(variable_type _value)

#define Property_ReadDefaultDefinition(class_name, variable_type, method_name, property_name) \
				variable_type ZDSJ::##class_name::get##method_name() const { \
					return this->##property_name;\
				}

#define Property_WriteDefaultDefinition(class_name, variable_type, method_name, property_name) \
				void ZDSJ::##class_name::set##method_name(variable_type _value) { \
					this->##property_name = _value;\
				}

#define PropertyBuilder_ReadWrite(variable_type, type_shortname, method_name, access_permission)\
				public:\
				    inline variable_type get##method_name(void)\
				    {\
				        return m_##type_shortname##method_name;\
				    }\
				    inline void set##method_name(variable_type v)\
				    {\
				        m_##type_shortname##method_name = v;\
				    }

#define PropertyBuilder_ReadOnly(variable_type, type_shortname, method_name, access_permission)\
				access_permission:\
				    variable_type m_##type_shortname##method_name;\
				public:\
				    inline variable_type get##method_name(void) const\
				    {\
				        return m_##type_shortname##method_name;\
				    }

#define PropertyBuilder_WriteOnly(variable_type, type_shortname, method_name, access_permission)\
				access_permission:\
				    variable_type m_##type_shortname##method_name;\
				public:\
				    inline void set##method_name(variable_type v)\
				    {\
				        m_##type_shortname##method_name = v;\
				    }

}