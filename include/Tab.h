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
		Delete,
		Rev,
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
	virtual const std::filesystem::path& ClassDiagPath()const = 0;
private:
	std::filesystem::path path;
};
