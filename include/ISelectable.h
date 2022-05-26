#pragma once

struct ISelectable
{
	enum ty
	{
		connection,
		node
	};
	virtual ~ISelectable() = default;
	virtual ty XType()const noexcept = 0;
};