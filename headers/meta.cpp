#include "meta.h"

namespace momo
{
	type_info::type_info(const std::type_info& type)
	{
		this->inner = &type;
	}

	bool type_info::operator==(const type_info& type) const
	{
		return *this->inner == *type.inner;
	}

	bool type_info::operator!=(const type_info& type) const
	{
		return !(*this == type);
	}

	const char* type_info::name() const
	{
		return inner->name();
	}

	bool type_info::before(const type_info& type) const
	{
		return inner->before(*type.inner);
	}

	std::ostream& operator<<(std::ostream& out, const type_info& type)
	{
		out << type.name();
		return out;
	}
}