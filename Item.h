#pragma once

#include <filesystem>

class Item
{
public:
	std::filesystem::path pathfile;
	uintmax_t size;
public:
	inline Item(const std::filesystem::directory_entry& entry)
		:
		pathfile(entry.path()),
		size(entry.file_size())
	{
	}
};