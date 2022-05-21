#pragma once
#include <filesystem>
#include <QWidget>

class Tab : public QWidget
{
public:
	enum request
	{
		None,
		Save,
		SaveAs,
		Close,
		Delete
	};
public:
	Tab(std::filesystem::path xpath): path(std::move(xpath)) {};
public:
	void SetPath(std::filesystem::path xpath)
	{
		path = std::move(xpath);
	}
	auto& Path()const noexcept { return path;}

	virtual void Request(request rq){}
private:
	std::filesystem::path path;
};
