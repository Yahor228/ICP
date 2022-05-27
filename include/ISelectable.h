#pragma once

struct ISelectable
{
	enum ty
	{
		connection,
		node,
		element
	};
	virtual ~ISelectable() = default;
	virtual ty XType()const noexcept = 0;
};